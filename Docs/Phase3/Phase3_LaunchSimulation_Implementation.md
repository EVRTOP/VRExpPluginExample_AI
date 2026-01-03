# Phase 3: 发射模拟系统实现指南
# Phase 3: Launch Simulation System Implementation Guide

## 目录 Table of Contents
1. [发射序列概述 Launch Sequence Overview](#1-发射序列概述)
2. [发射动画系统 Launch Animation System](#2-发射动画系统)
3. [特效系统集成 VFX System Integration](#3-特效系统集成)
4. [相机序列设计 Camera Sequence Design](#4-相机序列设计)
5. [物理模拟 Physics Simulation](#5-物理模拟)
6. [环境交互 Environment Interaction](#6-环境交互)
7. [发射控制台 Launch Control Console](#7-发射控制台)
8. [多级分离系统 Multi-Stage Separation](#8-多级分离系统)
9. [性能优化 Performance Optimization](#9-性能优化)

---

## 1. 发射序列概述 Launch Sequence Overview

### 1.1 发射流程 Launch Sequence Flow

**Complete Launch Timeline 完整发射时间线**:

```
T-60s:  Pre-launch checks complete 发射前检查完成
T-30s:  Final countdown begins 最终倒计时开始
T-10s:  Voice countdown starts 语音倒计时开始
        Background music intensifies 背景音乐增强
T-5s:   Controller vibration begins 手柄震动开始
T-3s:   Ignition sequence initiated 点火序列启动
T-0s:   Main engine ignition 主发动机点火
        ├─ Massive flame VFX 巨大火焰特效
        ├─ Camera shake 相机震动
        ├─ Ground smoke/steam 地面烟雾/蒸汽
        └─ Audio: Ignition sound 音频:点火声
T+2s:   Liftoff begins 开始升空
        ├─ Rocket starts ascending 火箭开始上升
        ├─ Camera follows rocket 相机跟随火箭
        └─ Continued engine effects 持续发动机特效
T+10s:  Max dynamic pressure (Max-Q) 最大动压
        └─ Camera shake intensifies 相机震动加剧
T+30s:  First stage burnout 一级燃烧完成
T+32s:  Stage separation 级间分离
        ├─ Explosive bolts fire 爆炸螺栓触发
        ├─ Separation VFX 分离特效
        ├─ First stage falls away 一级下落
        └─ Second stage ignites 二级点火
T+45s:  Fairing separation 整流罩分离
        └─ Fairing halves split and fall 整流罩两半分离下落
T+60s:  Second stage continues 二级继续飞行
T+90s:  Sequence ends 序列结束
        └─ Fade to mission success screen 淡出到任务成功界面
```

### 1.2 用户视角选项 User Perspective Options

**Perspective 1: Ground Observer 地面观察者**
- Fixed position on launch pad 发射台固定位置
- Watch from safe distance (100-500m) 安全距离观看
- Feel ground rumble through haptics 通过震动感受地面震动
- Realistic sound delay based on distance 基于距离的真实声音延迟

**Perspective 2: Following Camera 跟随相机**
- Camera follows rocket ascent 相机跟随火箭上升
- Cinematic angles and movements 电影化角度和运动
- Dynamic switching between views 视角动态切换
- Highlight key events (separation, fairing) 突出关键事件

**Perspective 3: On-Board (Optional) 舱内视角(可选)**
- Inside payload fairing 载荷整流罩内部
- G-force simulation effects G力模拟效果
- HUD displays (altitude, velocity, acceleration) HUD显示(高度,速度,加速度)
- View out windows during flight 飞行期间窗外视野

---

## 2. 发射动画系统 Launch Animation System

### 2.1 BP_LaunchSequenceManager

**File**: `Content/RocketAssembly/Blueprints/BP_LaunchSequenceManager.uasset`

**Parent Class**: Actor

#### 变量 Variables

```cpp
// 火箭引用 Rocket References
AActor* AssembledRocket;              // 完整装配的火箭
USkeletalMeshComponent* RocketMesh;   // 如果使用骨骼网格
TArray<AInteractiveRocketPart*> RocketParts;  // 所有火箭零件

// 发射台 Launch Pad
AActor* LaunchPad;                    // 发射台Actor
UStaticMeshComponent* LaunchClamps;   // 发射夹持器

// 动画状态 Animation State
ELaunchPhase CurrentPhase = PreLaunch;
// Enum: PreLaunch, Countdown, Ignition, Liftoff, Ascent, 
//       StageSeparation, SecondStage, FairingJettison, Completed

float LaunchTime = 0.0f;              // 发射后经过的时间
float CountdownTimer = 10.0f;         // 倒计时计时器

// 运动参数 Motion Parameters
FVector InitialPosition;              // 初始位置
FVector CurrentVelocity;              // 当前速度
float CurrentAltitude = 0.0f;         // 当前高度
float CurrentAcceleration = 20.0f;    // 当前加速度 (m/s²)

// 相机 Cameras
ACameraActor* GroundCamera;           // 地面相机
ACameraActor* FollowCamera;           // 跟随相机
USpringArmComponent* CameraArm;       // 弹簧臂
ECameraMode CurrentCameraMode = Ground;

// 特效 VFX
UNiagaraComponent* EngineExhaustVFX;  // 发动机尾焰
UNiagaraComponent* LaunchSmokeVFX;    // 发射烟雾
UNiagaraComponent* SeparationVFX;     // 分离特效
UNiagaraComponent* FairingVFX;        // 整流罩分离特效

// 音频 Audio
UBP_AdvancedAudioManager* AudioManager;
bool bCountdownAudioPlaying = false;

// 配置 Configuration
bool bUsePhysicsSimulation = false;   // 是否使用物理模拟
float MaxAltitude = 50000.0f;         // 最大高度 (cm)
float StageSeparationAltitude = 10000.0f;
float FairingJettisonAltitude = 15000.0f;

// 事件 Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLaunchPhaseChanged, ELaunchPhase, NewPhase);
FOnLaunchPhaseChanged OnLaunchPhaseChanged;
```

#### 主要函数 Key Functions

##### InitializeLaunch()
```
功能: 初始化发射序列
调用时机: 玩家完成装配后

逻辑:
1. Get reference to assembled rocket from PartManager
2. Verify all required parts are assembled
3. Move rocket to launch pad position
4. Attach to launch clamps
5. Create and position cameras:
   - Ground camera: 200m away, angled up
   - Follow camera: Attached to spring arm on rocket
6. Load VFX systems (engine exhaust, smoke)
7. Get Audio Manager reference
8. Set CurrentPhase = PreLaunch
9. Enable launch UI (countdown display)
10. Ready for StartLaunch() call
```

##### StartLaunch()
```
功能: 开始发射倒计时
输入: None (called by UI button or auto after assembly)

逻辑:
1. If CurrentPhase != PreLaunch, return (already launched)
2. Set CurrentPhase = Countdown
3. Fire OnLaunchPhaseChanged event
4. Play countdown music (fade in)
5. Set CountdownTimer = 10.0
6. Start BeginPlay tick enable
7. Begin countdown UI animation
8. Play voice countdown (VO_CN/EN_Launch_T10)
9. Each second:
   - Update UI countdown display
   - Play countdown tick SFX
   - At T-5s: Start haptic rumble (light)
10. At T-0: Call LaunchIgnition()
```

##### LaunchIgnition()
```
功能: 点火序列
调用时机: CountdownTimer reaches 0

逻辑:
1. Set CurrentPhase = Ignition
2. Fire OnLaunchPhaseChanged
3. Stop countdown music
4. Play ignition sound cue (SC_Launch_Sequence)
5. Start EngineExhaustVFX at engine nozzles
   - Spawn rate: 10000 particles/s
   - Initial velocity: -5000 cm/s (downward)
6. Start LaunchSmokeVFX at pad base
   - Large volume smoke (steam)
7. Camera shake: Heavy shake (intensity 2.0, duration 3s)
8. Haptic feedback: HE_EngineRumble (both controllers)
9. Delay 2 seconds (engine spool up)
10. Call LiftOff()
```

##### LiftOff()
```
功能: 火箭离开发射台
调用时机: 2 seconds after ignition

逻辑:
1. Set CurrentPhase = Liftoff
2. Release launch clamps (animate opening)
3. Play launch music (epic orchestral)
4. Record InitialPosition
5. Set CurrentVelocity = (0, 0, 50) cm/s (slow start)
6. Set CurrentAcceleration = 20 m/s² (2000 cm/s²)
7. Switch camera if auto-follow enabled:
   - Smooth transition to FollowCamera
8. Begin ascending (in Tick update)
9. Continue exhaust VFX
10. Adjust smoke VFX (follows rocket)
```

##### Tick(DeltaTime)
```
功能: 每帧更新发射状态
执行条件: CurrentPhase >= Liftoff

逻辑:
1. Increment LaunchTime += DeltaTime

2. Update rocket physics:
   If bUsePhysicsSimulation:
     - Apply thrust force to rocket
     - Apply gravity
   Else (cinematic mode):
     - CurrentVelocity.Z += CurrentAcceleration * DeltaTime
     - NewPosition = CurrentPosition + CurrentVelocity * DeltaTime
     - SetActorLocation(NewPosition, Smooth)
     - Slight rotation for realism (pitch towards horizon)

3. Update CurrentAltitude:
   - Altitude = (CurrentPosition - InitialPosition).Z

4. Update VFX:
   - EngineExhaustVFX: Scale with velocity
   - Update particle spawn rate based on thrust

5. Update camera:
   If FollowCamera active:
     - Smoothly follow rocket (lag for cinematic feel)
     - Adjust FOV based on velocity
     - Spring arm length adjusts with altitude

6. Check for phase transitions:
   - At Altitude > StageSeparationAltitude: Call StageSeparation()
   - At Altitude > FairingJettisonAltitude: Call FairingJettison()
   - At LaunchTime > 90s or Altitude > MaxAltitude: Call EndSequence()

7. Update HUD:
   - Display altitude, velocity, acceleration
   - Update trajectory visualization
```

##### StageSeparation()
```
功能: 一级与二级分离
调用时机: 到达分离高度

逻辑:
1. Set CurrentPhase = StageSeparation
2. Fire OnLaunchPhaseChanged
3. Play separation SFX (SFX_Stage_Separation)
4. Spawn SeparationVFX at interstage location
5. Detach first stage parts:
   - Find all parts tagged "FirstStage"
   - Detach from rocket
   - Apply physics:
     - Impulse: Backward and downward
     - Angular velocity: Random tumble
6. Enable physics simulation on separated parts
7. Brief pause in acceleration (0.5s)
8. Second stage ignition:
   - New exhaust VFX (smaller nozzle)
   - Reduced particle count
   - Increase acceleration (lighter mass)
9. Play haptic pulse (HE_PartSnap)
10. Camera shake (medium intensity)
11. Update HUD: "Stage 1 Separation Complete"
```

##### FairingJettison()
```
功能: 整流罩抛离
调用时机: 大气层外

逻辑:
1. Set CurrentPhase = FairingJettison
2. Play SFX (if any)
3. Find fairing parts (tagged "Fairing")
4. Animate fairing split:
   - Two halves open like clamshell
   - Use Timeline for smooth animation
5. Detach fairing halves
6. Apply physics:
   - Impulse: Outward from rocket (left/right)
   - Slight upward component
   - Gentle tumble
7. Spawn FairingVFX (small puffs at separation points)
8. Camera briefly focuses on payload reveal
9. Update HUD: "Fairing Jettison Complete"
10. Return to normal camera follow
```

##### EndSequence()
```
功能: 结束发射序列
调用时机: 序列完成或到达最大高度

逻辑:
1. Set CurrentPhase = Completed
2. Fire OnLaunchPhaseChanged
3. Fade out launch music (3 seconds)
4. Play success music (MUS_Success)
5. Stop all VFX (gradual fade)
6. Stop haptic feedback
7. Camera: Zoom out to show rocket at altitude
8. Display mission success overlay
9. Show stats:
   - Max altitude reached
   - Max velocity
   - Total flight time
   - Stage separation success
10. Enable UI button: "Return to Workshop"
11. Disable Tick updates
```

---

### 2.2 动画曲线 Animation Curves

#### Velocity Curve 速度曲线

**Use UE5 Curve Float Asset**: `Content/RocketAssembly/Data/Curves/FC_LaunchVelocity.uasset`

```
Time (s) | Velocity (cm/s) | Notes
---------|-----------------|------------------
0        | 0               | 发射台静止
2        | 50              | 缓慢离开发射台
5        | 500             | 加速
10       | 2000            | Max-Q approaching
20       | 5000            | 高速上升
30       | 8000            | 一级接近燃尽
32       | 6000            | 分离减速
35       | 10000           | 二级加速
60       | 15000           | 持续加速
90       | 20000           | 接近轨道速度
```

#### Camera Distance Curve 相机距离曲线

```
Altitude (m) | Camera Distance (m) | Notes
-------------|---------------------|------------------
0            | 20                  | 近距离发射台
100          | 30                  | 开始拉远
500          | 100                 | 中距离
2000         | 300                 | 远景
5000         | 500                 | 全景
```

---

## 3. 特效系统集成 VFX System Integration

### 3.1 发动机尾焰 Engine Exhaust

**File**: `Content/RocketAssembly/VFX/NS_RocketExhaust.uasset`

**Niagara System Configuration**:

```
Emitter: Main Exhaust Plume 主尾焰
├─ Spawn Rate: 5000 particles/second
├─ Lifetime: 1.5 - 2.5 seconds
├─ Initial Velocity: -3000 to -5000 cm/s (downward from nozzle)
├─ Color: 
│   ├─ Base: Bright orange/yellow (2000K)
│   └─ Gradient: Yellow → Orange → Dark red → Smoke gray
├─ Size: 
│   ├─ Initial: 50-100 cm
│   └─ Growth: Scale up to 300 cm over lifetime
├─ Material: M_Fire (additive, HDR glow)
├─ Light Emission: Enable (high intensity)
└─ Turbulence: Medium (adds realistic fluctuation)

Emitter: Exhaust Smoke 尾烟
├─ Spawn Rate: 1000 particles/second
├─ Lifetime: 3-5 seconds
├─ Initial Velocity: -1000 cm/s + random spread
├─ Color: Gray to black (cooler exhaust)
├─ Size: 100-200 cm, growing to 500 cm
├─ Material: M_Smoke (translucent)
└─ Affected by wind (slight drift)

Emitter: Shockwave Diamonds 激波菱形
├─ Spawn Rate: 50/s (creates diamond pattern in exhaust)
├─ Lifetime: 0.5s
├─ Position: Regular intervals along exhaust plume
├─ Color: Bright white/blue (supersonic shockwave)
├─ Size: 100 cm rings
└─ Material: M_Shockwave (additive)
```

**Dynamic Parameters 动态参数**:
- `ThrustPower` (0-1): Controls particle spawn rate and velocity
- `NozzleTemperature` (0-1): Controls color intensity
- `AtmosphericPressure` (0-1): Affects plume expansion (wider at lower pressure)

**Attachment**:
- Attach to rocket socket: "Engine_Exhaust_Point"
- Offset: (0, 0, -100) cm below nozzle
- Rotation: (0, 0, 180) pointing down

---

### 3.2 发射台烟雾 Launch Pad Smoke

**File**: `Content/RocketAssembly/VFX/NS_LaunchSmoke.uasset`

```
Emitter: Ground Steam/Smoke 地面蒸汽/烟雾
├─ Spawn Rate: 3000 particles/second (first 5 seconds)
├─ Lifetime: 5-10 seconds
├─ Initial Velocity: Upward 200-500 cm/s, radial spread
├─ Size: Very large (500-1000 cm)
├─ Color: White (steam) transitioning to gray (smoke)
├─ Material: M_Smoke_Volumetric
├─ Collision: Enable (bounces off launch pad structures)
└─ Buoyancy: Upward force (hot gases rise)

Emitter: Debris and Sparks 碎片与火花
├─ Spawn Rate: 500/s (first 2 seconds only)
├─ Lifetime: 1-3 seconds
├─ Initial Velocity: Random radial
├─ Size: Small (5-20 cm)
├─ Color: Orange/yellow (hot debris)
├─ Material: M_Spark (additive)
├─ Physics: Enable (falls with gravity)
└─ Collision: Enable
```

**Trigger Behavior**:
- Activate on ignition (T-0)
- Peak intensity: 0-5 seconds
- Gradual fade: 5-15 seconds
- Deactivate after 15 seconds

---

### 3.3 级间分离特效 Stage Separation VFX

**File**: `Content/RocketAssembly/VFX/NS_StageSeparation.uasset`

```
Emitter: Explosive Bolts 爆炸螺栓
├─ Spawn: One-shot burst (100 particles)
├─ Lifetime: 0.5 seconds
├─ Position: Ring around interstage
├─ Velocity: Radial outward, high speed
├─ Color: Bright orange/white flash
├─ Size: 20-50 cm
└─ Material: M_Explosion (additive)

Emitter: Separation Gas Puffs 分离气体
├─ Spawn: Burst (50 particles)
├─ Lifetime: 1-2 seconds
├─ Velocity: Outward from separation plane
├─ Color: White/gray
├─ Size: 50-100 cm
└─ Material: M_Smoke

Emitter: Shockwave Ring 冲击波环
├─ Spawn: Single particle
├─ Lifetime: 0.3 seconds
├─ Behavior: Expand from center (scale 0 → 500 cm)
├─ Color: White fade to transparent
└─ Material: M_Shockwave (additive)
```

---

### 3.4 整流罩分离特效 Fairing Jettison VFX

**File**: `Content/RocketAssembly/VFX/NS_FairingJettison.uasset`

```
Emitter: Separation Pyro 分离火工品
├─ Spawn: Small burst at hinge points
├─ Lifetime: 0.3s
├─ Color: Yellow/white flash
└─ Material: M_Flash

Emitter: Cold Gas Thrusters 冷气推进器
├─ Spawn: Small jets from fairing edges
├─ Purpose: Push fairing halves away
├─ Color: White vapor
├─ Duration: 1 second
└─ Direction: Outward from rocket
```

---

## 4. 相机序列设计 Camera Sequence Design

### 4.1 Sequencer 编排

**File**: `Content/RocketAssembly/Cinematics/SEQ_LaunchCinematic.uasset`

**Use UE5 Sequencer 使用 UE5 Sequencer**:

#### Track 1: Master Camera 主相机
```
00:00 - 00:10  Ground Camera - Wide Shot 地面相机-广角
                └─ Position: 200m from pad, slight angle
                └─ Rocket in frame center
                └─ Countdown overlay visible

00:10 - 00:12  Ground Camera - Close Up 近景
                └─ Zoom to engine nozzles
                └─ Show ignition flames starting

00:12 - 00:20  Ground Camera - Liftoff 升空
                └─ Pull back to show full rocket ascending
                └─ Camera tilts up to follow

00:20 - 00:30  Tracking Camera - Side View 侧面跟踪
                └─ Camera on moving rail (cinematic)
                └─ Tracks rocket ascending
                └─ Shows exhaust trail

00:30 - 00:35  Onboard Camera (optional) 舱内相机(可选)
                └─ Inside payload fairing
                └─ G-force effects on screen

00:35 - 00:40  Wide Angle - Stage Separation 广角-级间分离
                └─ Pull back to show full separation event
                └─ Both stages visible

00:40 - 00:50  Follow Camera - Second Stage 跟随相机-二级
                └─ Close follow of second stage
                └─ Fairing jettison visible

00:50 - 01:30  Distance Camera - Orbital View 远景-轨道视角
                └─ Zoom out to show rocket against Earth
                └─ Peaceful, triumphant mood
```

#### Track 2: Camera Shake 相机震动
```
00:00  Idle (no shake) 静止
00:10  Heavy shake (ignition) 强烈震动(点火)
00:15  Medium shake (liftoff) 中等震动(升空)
00:25  Gradual decrease 逐渐减弱
00:32  Sharp pulse (separation) 短脉冲(分离)
00:35  Return to smooth 恢复平滑
```

#### Track 3: Post Process Effects 后期处理效果
```
00:00 - 00:10  Normal settings 正常设置
00:10 - 00:12  Bloom increase (bright flames) 泛光增加(明亮火焰)
00:20 - 00:30  Motion blur (speed effect) 运动模糊(速度效果)
00:50 - 01:30  God rays (atmospheric scattering) 上帝之光(大气散射)
```

---

### 4.2 VR 相机考虑 VR Camera Considerations

**Challenge**: VR players cannot have camera forcibly moved (causes nausea)

**Solutions**:

#### Option 1: Observ Location (Teleport) 观察位置(传送)
```
1. Player remains in workshop initially
2. On launch, offer teleport to viewing platforms:
   - Platform A: 100m from pad (close view)
   - Platform B: 500m from pad (safe wide view)
   - Platform C: Observation deck (elevated, far)
3. Player can freely look around (6DOF preserved)
4. No forced camera movement
```

#### Option 2: Virtual Screen 虚拟屏幕
```
1. Player stays in workshop
2. Large virtual screen appears (TV/monitor)
3. Screen shows cinematic launch sequence
4. Player can watch comfortably
5. No VR motion, no nausea risk
```

#### Option 3: Hybrid Approach 混合方式
```
1. Teleport to viewing platform
2. Provide "Free Look" (player controls view)
3. Optional: AI Director highlights (gentle visual cues, not forced camera)
4. UI markers show key events ("Look Up!", "Look Right!")
```

---

## 5. 物理模拟 Physics Simulation

### 5.1 简化物理模型 Simplified Physics Model

**For cinematic mode (recommended) 电影化模式(推荐)**:

#### Vertical Motion 垂直运动
```
Variables:
- Mass (m): Rocket total mass (kg)
- Thrust (F): Engine thrust force (Newtons)
- Gravity (g): 9.81 m/s²
- Drag Coefficient (Cd): 0.5 (aerodynamic)

Equation:
Acceleration = (Thrust - Mass * Gravity - Drag) / Mass

Simplified (no drag for easy implementation):
Acceleration = (Thrust / Mass) - Gravity

Blueprint Implementation:
1. Current Velocity += Acceleration * DeltaTime
2. Position.Z += Velocity * DeltaTime
3. Reduce Mass over time (fuel consumption):
   Mass -= FuelBurnRate * DeltaTime
4. When Mass reduced, acceleration increases (lighter rocket)
```

#### Staging Physics 分级物理
```
On Stage Separation:
1. Calculate momentum conservation:
   - Total Momentum = (M1 + M2) * V
   - After separation:
     - First Stage: M1 * V1
     - Second Stage: M2 * V2
   - V1 < V (first stage slows, falls back)
   - V2 > V (second stage continues)

2. Apply separation impulse:
   - First Stage: Backward impulse (-1000 N·s)
   - Second Stage: Forward impulse (+500 N·s)

3. Enable physics on separated parts:
   - SetSimulatePhysics(true)
   - AddImpulse(SeparationVector)
   - Add random angular velocity (tumble effect)
```

---

### 5.2 高级物理模拟 (可选) Advanced Physics (Optional)

**For realistic simulation enthusiasts 面向真实模拟爱好者**:

#### Tsiolkovsky Rocket Equation 齐奥尔科夫斯基火箭方程
```
Delta-V = Ve * ln(M0 / Mf)

Where:
- Ve: Exhaust velocity (effective)
- M0: Initial mass (fully fueled)
- Mf: Final mass (empty)
- ln: Natural logarithm

Implementation:
1. Define rocket stages with:
   - Dry mass (structure)
   - Propellant mass (fuel + oxidizer)
   - Specific impulse (Isp) → Ve = Isp * g

2. Calculate available delta-V per stage
3. Simulate fuel consumption over time
4. Update mass, thrust, and acceleration dynamically
```

#### Atmospheric Drag 大气阻力
```
Drag Force = 0.5 * Cd * ρ * A * V²

Where:
- Cd: Drag coefficient (~0.5 for rocket)
- ρ: Air density (decreases with altitude)
- A: Cross-sectional area (m²)
- V: Velocity (m/s)

Air Density by Altitude:
- Sea level: 1.225 kg/m³
- 10 km: 0.4135 kg/m³
- 30 km: 0.0184 kg/m³
- 50 km: ~0 (vacuum)

Blueprint:
1. Calculate altitude
2. Look up air density (curve or formula)
3. Calculate drag force
4. Subtract from thrust in acceleration equation
```

#### Gravity Variation 重力变化
```
As rocket ascends, gravity decreases:
g(h) = g0 * (R / (R + h))²

Where:
- g0: Surface gravity (9.81 m/s²)
- R: Earth radius (6371 km)
- h: Altitude (km)

For game simplification, can use constant g unless very high altitude.
```

---

## 6. 环境交互 Environment Interaction

### 6.1 发射台环境 Launch Pad Environment

**Assets Required 所需资产**:

#### Launch Pad Structure 发射台结构
```
Static Mesh: SM_LaunchPad
├─ Base Platform (20m x 20m concrete)
├─ Flame Trench (channels exhaust away)
├─ Launch Mount (holds rocket)
├─ Umbilical Towers (2x, 30m tall)
│   └─ Swing Arms (retract at ignition)
├─ Water Deluge System (pipes and nozzles)
└─ Lightning Rod (tall mast)
```

#### Launch Clamps 发射夹持器
```
Blueprint: BP_LaunchClamp (x4 around rocket base)

States:
1. Clamped: Holding rocket firmly
2. Released: Opening animation at liftoff

Animation:
- Trigger: On LiftOff() call
- Duration: 0.5 seconds
- Movement: Swing outward 90 degrees
- SFX: Metal clang as clamps release
```

#### Umbilical Arms 脐带臂
```
Blueprint: BP_UmbilicalArm (x6-8 arms)

Function: Provide fuel, power, data to rocket before launch

Animation:
- Trigger: T-10 seconds (before ignition)
- Duration: 2 seconds each (staggered)
- Movement: Swing away from rocket
- SFX: Hydraulic hiss, mechanical sounds
- VFX: Small gas venting as disconnects occur
```

---

### 6.2 天气与大气效果 Weather and Atmospheric Effects

#### Wind Effects 风效果
```
Component: UWindDirectionalSourceComponent

Settings:
- Wind Speed: 5-15 km/h (light wind)
- Direction: Variable
- Affects: Smoke VFX, exhaust plume (slight drift)
- Does NOT significantly affect rocket (too powerful)
```

#### Clouds 云层
```
Use UE5 Volumetric Clouds:
- Rocket passes through cloud layer at ~2km altitude
- Clouds part/wisp around rocket (optional advanced VFX)
- Sound muffled inside clouds
```

#### Atmospheric Scattering 大气散射
```
Post Process Volume settings:
- Enable Sky Atmosphere
- Rayleigh Scattering: Creates blue sky
- Mie Scattering: Hazy atmosphere near horizon
- As rocket ascends, sky darkens (less atmosphere)
- At high altitude (>30km), sky becomes black (space)
```

---

### 6.3 地面效果 Ground Effects

#### Camera Shake 地面震动
```
Shake Class: CS_LaunchShake

Ignition:
- Amplitude: 2.0 (strong)
- Frequency: 8 Hz (rumble)
- Duration: 3 seconds
- Falloff: Begins immediately

Liftoff:
- Amplitude: 1.5 (moderate)
- Frequency: 5 Hz
- Duration: 10 seconds
- Falloff: Gradual over 10s

Trigger:
- All players within 1000m radius feel shake
- Intensity decreases with distance
```

#### Water Deluge System 喷水系统
```
Purpose: Suppress sound and heat at launch pad

VFX: NS_WaterDeluge
- Activates: T-10 seconds
- Effect: Large volume of water sprays onto launch pad
- Interaction: Creates massive steam clouds when exhaust hits water
- Sound: Rushing water + intense sizzling

Implementation:
- Niagara system: Water spray particles
- Collision: Water particles detect exhaust VFX
- On collision: Convert to steam particles
- Result: Huge billowing steam clouds
```

---

## 7. 发射控制台 Launch Control Console

### 7.1 BP_LaunchControlConsole

**File**: `Content/RocketAssembly/Blueprints/BP_LaunchControlConsole.uasset`

**Physical Console 物理控制台**:

```
Static Mesh: SM_ControlConsole
├─ Console Panel (angled for VR interaction)
├─ Buttons and Switches (interactive)
├─ Screen Display (UMG widget on mesh)
└─ Lights and Indicators

Interactive Elements:
1. Main Launch Button (big red button)
   - State: Locked until assembly complete
   - Interaction: Press to initiate countdown
   - Feedback: Glow when ready, blink during countdown

2. Abort Button (yellow)
   - Function: Cancel countdown (T-10 to T-3 only)
   - Interaction: Press to abort

3. Camera Selection Switch
   - Options: Ground / Follow / Onboard
   - Interaction: Toggle switch

4. Volume Slider
   - Function: Adjust audio mix
   - Interaction: VR hand slide

Screen Display (WBP_LaunchConsoleUI):
├─ Countdown Timer (large, centered)
├─ System Status Indicators:
│   ├─ Engine: ● GO / ● NO-GO
│   ├─ Fuel: ● GO / ● NO-GO
│   ├─ Weather: ● GO / ● NO-GO
│   └─ Flight Computer: ● GO / ● NO-GO
├─ Telemetry (during flight):
│   ├─ Altitude: XXX km
│   ├─ Velocity: XXXX m/s
│   ├─ Acceleration: XX G
│   └─ Stage: 1st / 2nd
└─ Event Log:
    "T-10: Countdown started"
    "T-0: Ignition"
    "T+2: Liftoff"
    "T+32: Stage Separation"
```

---

### 7.2 系统检查 System Checks

**Pre-Launch Checklist 发射前检查清单**:

```
Function: PerformSystemChecks()

Checks:
1. Assembly Complete:
   - Query BP_RocketPartManager
   - Verify all required parts assembled
   - Check assembly quality > 80%

2. Engine Systems:
   - Main engine part present: ✓
   - Turbopump installed: ✓
   - Fuel lines connected: ✓

3. Fuel Systems:
   - LOX tank installed: ✓
   - Fuel tank installed: ✓
   - Pressurization system ready: ✓

4. Control Systems:
   - Attitude control installed: ✓
   - Flight computer initialized: ✓

5. Payload:
   - Fairing present: ✓
   - Payload secured: ✓

Result:
- All GO: Enable launch button, set console to READY
- Any NO-GO: Keep button locked, display error message
```

---

## 8. 多级分离系统 Multi-Stage Separation

### 8.1 零件标记系统 Part Tagging System

**In BP_InteractiveRocketPart or Data Table**:

```
Add Tag field: EStageTag
Enum:
- FirstStage: Parts that separate first (base, main engine, tanks)
- SecondStage: Parts that continue (upper stage, payload)
- Fairing: Payload fairing (separate independently)
- Persistent: Never separates (payload, spacecraft)

Examples:
- Main Engine: FirstStage
- First Stage Fuel Tank: FirstStage
- Interstage: FirstStage (separates with stage 1)
- Second Stage Body: SecondStage
- Second Stage Engine: SecondStage
- Fairing: Fairing
- Payload: Persistent
```

---

### 8.2 分离逻辑 Separation Logic

**In BP_LaunchSequenceManager::StageSeparation()**:

```
Step 1: Identify Parts to Separate
   - Loop through RocketParts array
   - Filter parts with StageTag == FirstStage
   - Store in SeparatingParts array

Step 2: Calculate Separation Point
   - Find geometric center of first stage parts
   - This is the separation plane

Step 3: Play Effects
   - Spawn NS_StageSeparation at separation point
   - Play SFX_Stage_Separation
   - Camera focuses on separation (brief)

Step 4: Physical Separation
   For each part in SeparatingParts:
     - Detach from parent (break weld/attachment)
     - Enable physics simulation
     - Apply impulse:
       - Direction: Backward from rocket velocity
       - Magnitude: -500 to -1000 N·s
     - Apply angular velocity (random tumble):
       - X rotation: -10 to +10 deg/s
       - Y rotation: -10 to +10 deg/s
       - Z rotation: -5 to +5 deg/s
     - Set collision: Ignore rocket (prevent re-collision)

Step 5: Rocket Continues
   - Remaining parts (SecondStage + Persistent) continue
   - Calculate new rocket mass (lighter now)
   - Increase acceleration (less mass, same/more thrust)
   - Enable second stage engine VFX
   - Update exhaust particle system (different nozzle)

Step 6: First Stage Falls
   - Separated parts affected by gravity
   - Begin descent
   - Optional: Add atmospheric drag
   - Optional: Parachute deploy at low altitude (advanced feature)
   - Despawn after 30 seconds or out of view range (optimization)
```

---

### 8.3 整流罩分离逻辑 Fairing Jettison Logic

**In BP_LaunchSequenceManager::FairingJettison()**:

```
Step 1: Identify Fairing Parts
   - Find parts with StageTag == Fairing
   - Should be 2 parts (left and right halves)

Step 2: Animation Setup
   - Use Timeline for smooth animation
   - Duration: 2 seconds
   - Curve: Ease-out (starts fast, slows down)

Step 3: Animate Separation
   Timeline (0 → 1 over 2 seconds):
     Alpha value drives separation
     
     For Left Half:
       - Rotate: +45 degrees around hinge axis (Y-axis)
       - Translate: -100 cm outward (X-axis)
     
     For Right Half:
       - Rotate: -45 degrees
       - Translate: +100 cm outward
   
   Visual: Fairing opens like a clamshell

Step 4: Detach and Physics
   At Timeline end (Alpha = 1.0):
     - Detach both halves
     - Enable physics simulation
     - Apply gentle impulse:
       - Left half: Velocity (-200, 0, +50) cm/s
       - Right half: Velocity (+200, 0, +50) cm/s
     - Small tumble rotation

Step 5: Reveal Payload
   - Camera briefly focuses on payload
   - UI message: "Payload Revealed"
   - Optional: Zoom in on payload model
   - VFX: Gentle sparkle effect around payload (cinematic)

Step 6: Cleanup
   - Fairing parts despawn after 20 seconds
```

---

## 9. 性能优化 Performance Optimization

### 9.1 粒子系统优化 Particle System Optimization

**Problem**: Rocket exhaust creates millions of particles, heavy GPU load

**Solutions**:

#### LOD System for VFX LOD系统
```
Distance-based quality:

Close (< 50m):
- Max particle count: 10,000
- High resolution textures
- Complex material (HDR glow, distortion)

Medium (50-200m):
- Particle count: 3,000
- Standard textures
- Simplified material

Far (> 200m):
- Particle count: 500
- Low res textures
- Simple additive material
- Consider replacing with static mesh trail (optimization)
```

#### Particle Culling 粒子剔除
```
Niagara Settings:
- Enable bounds calculation
- Culling: Frustum culling (don't render off-screen particles)
- Max Draw Distance: 10,000 cm (100m) for smoke
- Max Draw Distance: 50,000 cm (500m) for exhaust
```

---

### 9.2 物理优化 Physics Optimization

**Problem**: Separated parts with physics are expensive

**Solutions**:

#### Simplified Collision 简化碰撞
```
For separated rocket parts:
- Use simple collision (box, sphere, capsule)
- Avoid complex collision meshes
- Disable collision after 10 seconds (parts are far from player)
```

#### Despawn Strategy 销毁策略
```
Conditions for despawning:
1. Part has fallen below altitude = -1000m (crashed)
2. Part is > 2000m from player camera
3. Part has been separated for > 30 seconds
4. Part is not in camera frustum for > 5 seconds

Implementation:
- Check conditions in Tick (every 1 second, not every frame)
- Call DestroyActor() when conditions met
```

#### Physics Sleeping 物理休眠
```
For falling parts:
- If velocity < 10 cm/s for 3 seconds: Put to sleep
- Sleep = no physics updates (CPU saved)
- Wake if disturbed (unlikely in this scenario)
```

---

### 9.3 渲染优化 Rendering Optimization

#### Dynamic Resolution 动态分辨率
```
UE5 Feature: Screen Percentage (Dynamic Resolution)

Settings:
- Min Screen Percentage: 50%
- Max Screen Percentage: 100%
- Target FPS: 90 (PC VR), 72 (Quest)

Behavior:
- If FPS drops below target: Lower resolution
- If FPS above target: Increase resolution (up to 100%)
- Seamless for user (mostly imperceptible)
```

#### Instanced Rendering 实例化渲染
```
For repeated elements (exhaust particles, separated parts):
- Use Instanced Static Meshes when possible
- Niagara supports GPU instancing automatically
- Reduces draw calls significantly
```

---

### 9.4 音频优化 Audio Optimization

**Refer to Phase3_AudioSystem_Implementation.md, Section 10**

Key points:
- Sound concurrency limits
- Distance-based attenuation (far sounds muted)
- Stream music from disk (don't load into memory)

---

### 9.5 性能目标 Performance Targets

**PC VR (High-End)**:
- Target: 90 FPS (11.1 ms/frame)
- Resolution: 2160 x 2400 per eye
- Particle Budget: 15,000 active particles
- Draw Calls: < 2000

**Meta Quest (Standalone)**:
- Target: 72 FPS (13.9 ms/frame)
- Resolution: 1832 x 1920 per eye
- Particle Budget: 3,000 active particles
- Draw Calls: < 500
- Aggressive LOD and culling required

**Profiling Tools**:
```
Console Commands:
- stat fps: Show FPS
- stat unit: Show frame time breakdown
- stat particles: Show particle counts
- stat scenerendering: Show draw calls
- profilegpu: GPU profiling
```

---

## 10. 测试与验证 Testing and Validation

### 10.1 发射序列测试清单 Launch Sequence Testing Checklist

#### 功能测试 Functional Testing
- [ ] Countdown starts correctly when launch button pressed
- [ ] Voice countdown plays in correct language
- [ ] Music transitions smoothly (countdown → launch → success)
- [ ] Ignition VFX triggers at T-0
- [ ] Rocket begins ascending at T+2
- [ ] Camera follows rocket smoothly (if enabled)
- [ ] Haptic feedback synchronizes with audio and events
- [ ] Stage separation occurs at correct altitude/time
- [ ] Second stage ignites after separation
- [ ] Fairing jettisons at correct time
- [ ] Sequence ends at completion (T+90s or max altitude)
- [ ] UI displays correct telemetry (altitude, velocity)
- [ ] Return to workshop button works

#### VR 舒适度测试 VR Comfort Testing
- [ ] No forced camera movement (unless opted in)
- [ ] Camera transitions are smooth (no sudden cuts)
- [ ] Acceleration effects don't cause nausea
- [ ] Haptic feedback not too intense
- [ ] Player can abort/skip if uncomfortable

#### 视觉质量测试 Visual Quality Testing
- [ ] Exhaust VFX looks realistic
- [ ] Smoke effects are convincing
- [ ] Stage separation is dramatic and clear
- [ ] Lighting is appropriate (bright flames, darkening sky)
- [ ] No visual glitches or clipping

#### 性能测试 Performance Testing
- [ ] Maintains target FPS (90 PC VR, 72 Quest)
- [ ] No hitching or stuttering during launch
- [ ] Memory usage stable (no leaks)
- [ ] GPU usage within acceptable range (< 90%)
- [ ] Particle count stays within budget

---

### 10.2 边缘情况测试 Edge Case Testing

- [ ] Launch during incorrect assembly (should be blocked)
- [ ] Abort countdown mid-way (should return to ready state)
- [ ] Player teleports away during launch (sequence continues)
- [ ] Multiple launches in one session (no state corruption)
- [ ] Language switch during launch (should complete current language)

---

## 11. 实施时间线 Implementation Timeline

### Week 1: 基础发射逻辑 Basic Launch Logic
- Day 1-2: Implement BP_LaunchSequenceManager (countdown, ignition, liftoff)
- Day 3: Basic rocket motion (cinematic mode)
- Day 4: Camera system (ground, follow)
- Day 5: Test basic launch sequence
- Deliverable: Rocket launches and ascends

### Week 2: 特效与音频 VFX and Audio
- Day 1: Create NS_RocketExhaust (engine exhaust)
- Day 2: Create NS_LaunchSmoke (launch pad effects)
- Day 3: Integrate audio (ignition, countdown, music)
- Day 4: Haptic feedback integration
- Day 5: Test audio-visual synchronization
- Deliverable: Launch sequence with full audio and VFX

### Week 3: 分离系统 Separation Systems
- Day 1-2: Implement stage separation logic
- Day 3: Implement fairing jettison
- Day 4: Create separation VFX
- Day 5: Test separation events
- Deliverable: Multi-stage rocket with separations

### Week 4: 控制台与环境 Console and Environment
- Day 1-2: Create BP_LaunchControlConsole
- Day 3: Build launch pad environment
- Day 4: Implement system checks and UI
- Day 5: Test complete workflow (assembly → launch)
- Deliverable: Full launch experience

### Week 5: 优化与润色 Optimization and Polish
- Day 1-2: Performance profiling and optimization
- Day 3: VR comfort testing and adjustments
- Day 4: Visual polish and bug fixes
- Day 5: Final testing and validation
- Deliverable: Optimized, polished launch simulation

---

## 12. 总结与交付 Summary and Deliverables

### 12.1 完成标准 Completion Criteria

Launch Simulation is complete when:
- ✅ Full launch sequence implemented (countdown → ascent → separations → completion)
- ✅ Cinematic camera system working
- ✅ VFX systems operational (exhaust, smoke, separations)
- ✅ Audio fully integrated (countdown, ignition, music, haptics)
- ✅ Stage separation logic functional
- ✅ Fairing jettison working
- ✅ Launch control console interactive
- ✅ VR-comfortable (no forced movement unless opted in)
- ✅ Performance targets met (90 FPS PC VR, 72 FPS Quest)
- ✅ Tested thoroughly in VR

### 12.2 交付清单 Deliverables Checklist

**Blueprints 蓝图**:
- [ ] BP_LaunchSequenceManager
- [ ] BP_LaunchControlConsole
- [ ] BP_LaunchClamp
- [ ] BP_UmbilicalArm

**VFX 特效**:
- [ ] NS_RocketExhaust
- [ ] NS_LaunchSmoke
- [ ] NS_StageSeparation
- [ ] NS_FairingJettison
- [ ] NS_WaterDeluge (optional)

**Sequencer 序列**:
- [ ] SEQ_LaunchCinematic

**Environment 环境**:
- [ ] SM_LaunchPad
- [ ] SM_ControlConsole
- [ ] L_LaunchSite (level)

**UI Widgets UI小部件**:
- [ ] WBP_LaunchConsoleUI
- [ ] WBP_LaunchTelemetry (HUD overlay)

**Audio Integration 音频集成**:
- [ ] Launch audio triggers implemented
- [ ] Haptic feedback synchronized

---

### 12.3 下一步 Next Steps

After completing the launch simulation:
1. Proceed to **Phase 3 Document 3**: Achievement System Implementation
2. Track player accomplishments during launch
3. Award certificates and achievements
4. Prepare for final Phase 3 integration

---

**文档版本 Document Version**: 1.0.0
**最后更新 Last Updated**: 2026-01-03
**作者 Author**: VRExpPluginExample_AI Team
**状态 Status**: Complete and Ready for Implementation

---

**Phase 3 Launch Simulation Implementation Guide Complete!**
**Phase 3 发射模拟系统实现指南完成！🚀🔥**
