# Phase 2 - Materials and VFX Implementation Guide
# 火箭装配系统 Phase 2 材质和特效实现指南

## 📋 Overview 概述

This guide provides detailed specifications for creating materials and Niagara VFX for the VR Rocket Assembly Educational System.

本指南提供了为 VR 火箭装配教育系统创建材质和 Niagara 特效的详细规范。

---

## Materials System 材质系统

### General Material Guidelines 通用材质指南

**VR Optimization VR 优化**
- Keep shader complexity low (< 200 instructions)
- Use Material Instances for variations
- Limit texture samples (< 4 per material)
- Avoid costly operations (WorldPositionOffset in VR)

**PBR Workflow PBR 工作流**
- Use Metallic/Roughness workflow
- Base Color: Albedo map (no lighting info)
- Metallic: 0 = dielectric, 1 = metal
- Roughness: 0 = mirror, 1 = matte
- Normal: For surface detail

---

## 1. M_RocketMetal - 火箭金属材质

### Purpose 目的
Base material for metal rocket parts with customizable properties.
金属火箭零件的基础材质，具有可定制属性。

### Location 位置
`Content/RocketAssembly/Materials/M_RocketMetal.uasset`

### Material Type
Master Material (for creating instances)

### Material Properties 材质属性

#### Shading Model
- PBR (Physically Based Rendering)
- Blend Mode: Opaque
- Shading Model: Default Lit

#### Texture Inputs 纹理输入

```
Base Color Map (RGB):
- Resolution: 2048x2048 or 1024x1024
- Format: BC1 (DXT1) for no alpha
- Content: Metal albedo color
- Default: Light gray (0.7, 0.7, 0.7)

Normal Map (RGB):
- Resolution: 2048x2048
- Format: BC5 (Normal map compression)
- Content: Surface detail (scratches, dents)
- Intensity: Adjustable via parameter (0-1)
- Default: Flat normal (0.5, 0.5, 1)

Metallic Map (R):
- Resolution: 1024x1024
- Format: BC4 (single channel)
- Content: Metal vs non-metal areas
- Default: 1.0 (fully metallic)

Roughness Map (R):
- Resolution: 1024x1024
- Format: BC4
- Content: Surface roughness variation
- Default: 0.4 (slightly shiny)

Ambient Occlusion Map (R, optional):
- Resolution: 1024x1024
- Format: BC4
- Content: Cavity shadows
- Default: 1.0 (no AO)
```

#### Material Parameters 材质参数

```
Scalar Parameters:
- MetallicMultiplier (0-1, default 1.0)
- RoughnessMultiplier (0-1, default 0.4)
- NormalIntensity (0-2, default 1.0)
- AOIntensity (0-1, default 0.5)

Vector Parameters:
- ColorTint (RGB, default white)
- EmissiveColor (RGB, default black)

Scalar Parameters (for highlighting):
- EmissiveIntensity (0-10, default 0)
- FresnelIntensity (0-5, default 0)
```

### Material Graph Setup 材质图设置

```
Base Color:
[Texture Sample: BaseColorMap] → Multiply → [ColorTint] → Base Color Output

Normal:
[Texture Sample: NormalMap] → Multiply Intensity → [NormalIntensity] → Normal Output

Metallic:
[Texture Sample: MetallicMap] → Multiply → [MetallicMultiplier] → Metallic Output

Roughness:
[Texture Sample: RoughnessMap] → Multiply → [RoughnessMultiplier] → Roughness Output

Ambient Occlusion:
[Texture Sample: AOMap] → Lerp → [AOIntensity] → AO Output

Emissive (for highlighting):
[EmissiveColor] → Multiply → [EmissiveIntensity] → Emissive Color Output

Fresnel (edge glow):
[Fresnel] → Multiply → [FresnelIntensity] → Add to Emissive
```

### Material Instances 材质实例

Create instances for different parts:

**MI_RocketMetal_Silver 银色**
```
ColorTint: (0.85, 0.85, 0.9)
Roughness: 0.3
Metallic: 1.0
Use for: Structural parts, tanks
```

**MI_RocketMetal_DarkGray 深灰**
```
ColorTint: (0.3, 0.3, 0.35)
Roughness: 0.5
Metallic: 0.9
Use for: Engine nozzles, support structures
```

**MI_RocketMetal_Orange 橙色（涂层）**
```
ColorTint: (1.0, 0.4, 0.1)
Roughness: 0.6
Metallic: 0.2
Use for: Fuel tanks, insulation
```

**MI_RocketMetal_White 白色（涂层）**
```
ColorTint: (0.95, 0.95, 1.0)
Roughness: 0.7
Metallic: 0.1
Use for: Fairing, external panels
```

---

## 2. M_PartHighlight - 零件高亮材质

### Purpose 目的
Material for highlighting selected/hovered parts with customizable glow effects.
用于高亮选中/悬停零件的材质，具有可定制发光效果。

### Location 位置
`Content/RocketAssembly/Materials/M_PartHighlight.uasset`

### Material Type
Material (applied as overlay or lerped with base material)

### Material Properties 材质属性

#### Shading Model
- Blend Mode: Translucent or Additive
- Shading Model: Unlit (for pure emissive)

#### Material Parameters 材质参数

```
Vector Parameters:
- HighlightColor (RGB, default yellow: 1, 1, 0)
- EdgeColor (RGB, default white: 1, 1, 1)

Scalar Parameters:
- Intensity (0-10, default 2.0)
- EdgeThickness (0-1, default 0.1)
- PulseSpeed (0-10, default 2.0)
- EdgeSoftness (0-1, default 0.3)
```

### Material Graph Setup 材质图设置

```
Fresnel Edge Detection:
[Fresnel] → Power → [EdgeThickness] → Multiply → [EdgeColor]

Pulse Animation:
[Time] → Multiply → [PulseSpeed] → Sine → Multiply → 0.5 → Add 0.5

Combined Emissive:
[HighlightColor] → Multiply → [Intensity] → Multiply → [Pulse] → Add [Edge] → Emissive Color

Opacity (if Translucent):
[Pulse] → Multiply → [Intensity] → Opacity Output
```

### Usage 使用方法

**Method 1: Material Instance Dynamic (Recommended)**
```cpp
// In C++ InteractiveRocketPart::EnableHighlight
UMaterialInstanceDynamic* DynamicMaterial = 
    StaticMeshComponent->CreateDynamicMaterialInstance(0, M_PartHighlight);
DynamicMaterial->SetVectorParameterValue("HighlightColor", FLinearColor::Green);
DynamicMaterial->SetScalarParameterValue("Intensity", 3.0f);
```

**Method 2: Material Layer**
```
Apply M_PartHighlight as an overlay layer on top of base material
Use in Material Parameter Collection for global control
```

---

## 3. M_SocketIndicator - 装配点指示器材质

### Purpose 目的
Material for assembly socket visualization with dynamic color states.
用于装配点可视化的材质，具有动态颜色状态。

### Location 位置
`Content/RocketAssembly/Materials/M_SocketIndicator.uasset`

### Material Properties 材质属性

#### Shading Model
- Blend Mode: Translucent
- Shading Model: Unlit

#### Material Parameters 材质参数

```
Vector Parameters:
- IndicatorColor (RGB, default yellow)

Scalar Parameters:
- Opacity (0-1, default 0.6)
- EmissiveIntensity (0-10, default 5.0)
- PulseSpeed (0-5, default 1.5)
- PulseAmplitude (0-1, default 0.3)
- RingThickness (0-1, default 0.2)
```

### Material Graph Setup 材质图设置

```
Ring Shape:
[Radial Gradient] → Remap (0.4-0.6 to 0-1) → Multiply → [RingThickness]

Pulse:
[Time] → Multiply → [PulseSpeed] → Sine → Multiply → [PulseAmplitude] → Add (1 - Amplitude)

Emissive:
[IndicatorColor] → Multiply → [EmissiveIntensity] → Multiply → [Pulse] → Emissive

Opacity:
[Ring Shape] → Multiply → [Opacity] → Multiply → [Pulse] → Opacity Output
```

### Color States 颜色状态

```
Idle State (Waiting):
- Color: Yellow (1, 1, 0)
- Opacity: 0.5
- Pulse: Slow (Speed 1.0)

Ready State (Correct part nearby):
- Color: Green (0, 1, 0)
- Opacity: 0.8
- Pulse: Fast (Speed 2.5)

Error State (Wrong part):
- Color: Red (1, 0, 0)
- Opacity: 0.8
- Pulse: Rapid blink (Speed 5.0)

Occupied State (Assembled):
- Color: Gray (0.5, 0.5, 0.5)
- Opacity: 0.3
- Pulse: None (Speed 0)
```

---

## 4. M_Hologram - 全息预览材质

### Purpose 目的
Holographic material for part preview showing correct assembly position.
用于零件预览的全息材质，显示正确装配位置。

### Location 位置
`Content/RocketAssembly/Materials/M_Hologram.uasset`

### Material Properties 材质属性

#### Shading Model
- Blend Mode: Translucent
- Shading Model: Unlit

#### Material Parameters 材质参数

```
Vector Parameters:
- HologramColor (RGB, default cyan: 0, 1, 1)

Scalar Parameters:
- Opacity (0-1, default 0.3)
- ScanlineSpeed (0-10, default 2.0)
- ScanlineScale (1-100, default 20.0)
- FlickerSpeed (0-10, default 5.0)
- FlickerAmount (0-1, default 0.1)
- EdgeGlowIntensity (0-5, default 2.0)
```

### Material Graph Setup 材质图设置

```
Scanline Effect:
[WorldPosition.Z] → Add [Time * ScanlineSpeed] → Divide → [ScanlineScale] → Frac → 
Multiply → 2 → Subtract 1 → Abs → OneMinus → Power 4

Edge Glow:
[Fresnel] → Power 3 → Multiply → [EdgeGlowIntensity]

Flicker:
[Time] → Multiply → [FlickerSpeed] → Sine → Multiply → [FlickerAmount] → Add (1 - Amount)

Emissive:
[HologramColor] → Multiply → [Scanline + Edge + Flicker] → Emissive Color

Opacity:
[Scanline] → Multiply → [Opacity] → Multiply → [Flicker] → Opacity Output

Refraction (optional):
[Noise] → Multiply → 0.05 → Refraction Output
```

### Visual Features 视觉特征

- Animated scanlines moving upward
- Edge glow (Fresnel effect)
- Subtle flicker for holographic feel
- Semi-transparent
- Bright cyan/blue color
- Optional: Slight refraction distortion

---

## 5. M_UI_Background - UI 背景材质

### Purpose 目的
Semi-transparent background for UI panels in VR space.
VR 空间中 UI 面板的半透明背景。

### Location 位置
`Content/RocketAssembly/Materials/M_UI_Background.uasset`

### Material Properties 材质属性

#### Shading Model
- Blend Mode: Translucent
- Shading Model: Unlit
- Domain: User Interface

#### Material Parameters 材质参数

```
Vector Parameters:
- BackgroundColor (RGB, default dark blue: 0.05, 0.1, 0.2)

Scalar Parameters:
- Opacity (0-1, default 0.8)
- BlurAmount (0-10, default 2.0)
- VignetteIntensity (0-1, default 0.3)
```

### Material Graph Setup 材质图设置

```
Base Color:
[BackgroundColor] → Base Color Output

Opacity:
[Opacity] → Multiply by optional gradient/vignette → Opacity Output

Optional Blur:
Scene Texture: PostProcessInput0 → Custom blur → Add to base

Vignette:
[UV] → Distance from Center → Power 2 → Multiply → [VignetteIntensity] → 
Subtract from Opacity
```

### Usage Notes 使用说明

- Applied to UMG Image widgets
- Can use gradient for depth
- Blur requires PostProcess input (may not work in all UI contexts)
- Keep simple for performance

---

## Niagara VFX System 特效系统

### General VFX Guidelines 通用特效指南

**VR Optimization VR 优化**
- Keep particle counts low (< 1000 active at once)
- Use GPU particles (Niagara default)
- Cull particles outside view
- Use simpler materials for VFX

**Visual Feedback 视觉反馈**
- Effects should be clear but not overwhelming
- Duration: 0.5-2 seconds for feedback effects
- Color code: Green = success, Red = error, Yellow = warning

---

## 1. NS_AssemblySuccess - 装配成功特效

### Purpose 目的
Celebration effect when a part is successfully assembled.
零件成功装配时的庆祝特效。

### Location 位置
`Content/RocketAssembly/VFX/NS_AssemblySuccess.uasset`

### Effect Description 效果描述

**Visual Elements 视觉元素**
- Green particle burst (radial expansion)
- Upward-floating sparkles
- Ring shockwave expanding outward
- Brief bright flash

**Duration 持续时间**: 1.5 seconds

### Emitter Setup 发射器设置

#### Emitter 1: Burst Particles 爆发粒子
```
Spawn Rate: Burst
- Count: 50 particles
- Time: 0.0s

Initialize Particle:
- Lifetime: 1.0s
- Color: Green gradient (bright to dark)
- Sprite Size: 10-20 cm
- Velocity: Radial (sphere), Speed 100-300 cm/s

Update:
- Drag: 2.0
- Color over Life: Fade out
- Scale over Life: Shrink to 50%

Rendering:
- Sprite Renderer
- Material: Simple emissive particle
- Blend Mode: Additive
```

#### Emitter 2: Sparkles 火花
```
Spawn Rate: 20/second
Duration: 0.5s

Initialize:
- Lifetime: 0.8s
- Color: Green + Yellow
- Size: 5 cm
- Velocity: Random cone (upward), Speed 50-150 cm/s

Update:
- Gravity: -980 cm/s²
- Rotation: Random spin
- Color: Fade out

Rendering:
- Sprite with glow texture
```

#### Emitter 3: Ring Shockwave 环形冲击波
```
Spawn Rate: Burst (1 particle)

Initialize:
- Lifetime: 0.5s
- Color: Green with transparency
- Mesh: Ring mesh (or Sprite)
- Initial Scale: 10 cm

Update:
- Scale over Life: 10 cm → 200 cm
- Opacity: 1.0 → 0.0
- No velocity (stays at spawn point)

Rendering:
- Material: Emissive ring
- Blend Mode: Additive
```

---

## 2. NS_AssemblyFail - 装配失败特效

### Purpose 目的
Error indication when assembly attempt fails.
装配尝试失败时的错误指示。

### Location 位置
`Content/RocketAssembly/VFX/NS_AssemblyFail.uasset`

### Effect Description 效果描述

**Visual Elements 视觉元素**
- Red "X" symbol particles
- Shaking particles
- Error flash (red pulse)

**Duration 持续时间**: 0.5 seconds (quick feedback)

### Emitter Setup 发射器设置

#### Emitter 1: Error Symbols 错误符号
```
Spawn Rate: Burst (3-5 particles)

Initialize:
- Lifetime: 0.5s
- Color: Red
- Sprite: X symbol texture
- Size: 30 cm
- Position: Random offset around spawn point

Update:
- No velocity (floating)
- Scale: Pulse (large → small)
- Rotation: Slight wobble
- Opacity: Fade out

Rendering:
- Billboard sprite
- Material: X symbol
- Blend Mode: Translucent
```

#### Emitter 2: Flash 闪光
```
Spawn Rate: Burst (1 particle)

Initialize:
- Lifetime: 0.2s
- Color: Bright red
- Size: 100 cm sphere
- Position: Spawn point

Update:
- Scale: Expand 100 → 150 cm
- Opacity: 1.0 → 0.0

Rendering:
- Sprite (soft circle)
- Blend Mode: Additive
```

---

## 3. NS_PartHighlight - 零件高亮特效

### Purpose 目的
Subtle particle effect around highlighted parts.
被高亮零件周围的微妙粒子效果。

### Location 位置
`Content/RocketAssembly/VFX/NS_PartHighlight.uasset`

### Effect Description 效果描述

**Visual Elements 视觉元素**
- Glowing particles orbiting part
- Gentle pulsing light
- Optional scanline sweep

**Duration 持续时间**: Continuous (looping) while highlighted

### Emitter Setup 发射器设置

#### Emitter: Orbital Particles 轨道粒子
```
Spawn Rate: 5/second
Looping: Yes

Initialize:
- Lifetime: 2.0s
- Color: Yellow/gold
- Size: 8 cm
- Position: On mesh surface (Use Mesh Location module)

Update:
- Orbit around mesh center
- Rotation speed: 360°/2s
- Vertical bobbing: Sine wave
- Opacity: Pulse (0.3 - 1.0)

Rendering:
- Sprite with glow
- Blend Mode: Additive
```

---

## 4. NS_SocketIndicator - 装配点指示特效

### Purpose 目的
Particle effect at assembly socket locations to draw attention.
装配点位置的粒子效果以吸引注意力。

### Location 位置
`Content/RocketAssembly/VFX/NS_SocketIndicator.uasset`

### Effect Description 效果描述

**Visual Elements 视觉元素**
- Upward-flowing particles
- Circular pulse on ground
- Color changes based on socket state

**Duration 持续时间**: Continuous (looping)

### Emitter Setup 发射器设置

#### Emitter 1: Upward Flow 向上流动
```
Spawn Rate: 10/second
Looping: Yes

Initialize:
- Lifetime: 2.0s
- Color: [Dynamic - based on socket state]
- Size: 5 cm
- Position: Ring around socket (radius 30 cm)
- Velocity: Upward (0, 0, 50) cm/s

Update:
- Acceleration: Slow upward
- Opacity: Fade in then out (0 → 1 → 0)
- Size: Shrink over life

Rendering:
- Sprite (soft dot)
- Blend Mode: Additive
```

#### Emitter 2: Ground Pulse 地面脉冲
```
Spawn Rate: 1 every 0.5s

Initialize:
- Lifetime: 0.5s
- Color: [Dynamic - based on socket state]
- Position: Socket location (ground level)
- Size: 10 cm (ring shape)

Update:
- Scale: 10 cm → 80 cm
- Opacity: 1.0 → 0.0

Rendering:
- Sprite (ring texture)
- Blend Mode: Additive
- Aligned to ground plane
```

### Color Parameter 颜色参数

```
User Parameter: SocketStateColor (Vector3)
- Idle: Yellow (1, 1, 0)
- Ready: Green (0, 1, 0)
- Error: Red (1, 0, 0)
- Occupied: Gray (0.5, 0.5, 0.5)

Set via Blueprint:
NiagaraComponent->SetVectorParameter("SocketStateColor", FVector(0, 1, 0));
```

---

## Material Parameter Collections 材质参数集合

### Purpose 目的
Global material parameters controllable from Blueprint for synchronized effects.
可从蓝图控制的全局材质参数，用于同步效果。

### Location 位置
`Content/RocketAssembly/Materials/MPC_GlobalEffects.uasset`

### Parameters 参数

```
Scalar Parameters:
- GlobalHighlightIntensity (default 1.0)
- GlobalEmissiveBoost (default 1.0)
- TimeScale (default 1.0)

Vector Parameters:
- GlobalTintColor (default white)
- ErrorFlashColor (default red)

Usage in Materials:
[Material Parameter Collection] → Get Parameter → Use in material graph

Usage in Blueprint:
Set Scalar Parameter Value in Material Parameter Collection
Parameter Name: "GlobalHighlightIntensity"
Value: 2.0
```

---

## Testing Checklist 测试清单

### Materials
- [ ] M_RocketMetal looks realistic in lighting
- [ ] Material instances have correct colors
- [ ] M_PartHighlight visible and attractive
- [ ] M_SocketIndicator changes color correctly
- [ ] M_Hologram looks futuristic and clear
- [ ] All materials perform well in VR (60+ FPS)

### VFX
- [ ] NS_AssemblySuccess plays on correct event
- [ ] NS_AssemblyFail indicates errors clearly
- [ ] NS_PartHighlight doesn't obstruct view
- [ ] NS_SocketIndicator visible from distance
- [ ] All effects perform well (< 1000 particles)
- [ ] Colors are distinct and meaningful

### Integration
- [ ] Materials change at correct times
- [ ] VFX spawn at correct locations
- [ ] Audio-visual synchronization
- [ ] No memory leaks from repeated spawning
- [ ] Effects look good in both lit and dark areas

---

**Implementation Order Recommendation**:
1. M_RocketMetal (core visual)
2. M_PartHighlight (interaction feedback)
3. M_SocketIndicator (gameplay guidance)
4. NS_AssemblySuccess (positive feedback)
5. NS_AssemblyFail (error feedback)
6. M_Hologram (tutorial aid)
7. NS_PartHighlight (polish)
8. NS_SocketIndicator (polish)

**实施顺序建议**：
1. M_RocketMetal（核心视觉）
2. M_PartHighlight（交互反馈）
3. M_SocketIndicator（游戏指导）
4. NS_AssemblySuccess（正面反馈）
5. NS_AssemblyFail（错误反馈）
6. M_Hologram（教学辅助）
7. NS_PartHighlight（润色）
8. NS_SocketIndicator（润色）
