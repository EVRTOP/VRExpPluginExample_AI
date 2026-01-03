# Phase 3: 音频系统集成指南
# Phase 3: Audio System Integration Guide

## 目录 Table of Contents
1. [空间音效系统 Spatial Audio System](#1-空间音效系统)
2. [音效资产需求清单 Sound Assets Requirements](#2-音效资产需求清单)
3. [语音系统实现 Voice System Implementation](#3-语音系统实现)
4. [背景音乐系统 Background Music System](#4-背景音乐系统)
5. [Blueprint 音频管理器 Blueprint Audio Manager](#5-blueprint-音频管理器)
6. [Sound Cue 配置指南 Sound Cue Configuration](#6-sound-cue-配置指南)
7. [3D 空间音效设置 3D Spatial Audio Settings](#7-3d-空间音效设置)
8. [手柄震动反馈集成 Haptic Feedback Integration](#8-手柄震动反馈集成)
9. [音效触发时机 Sound Triggering](#9-音效触发时机)

---

## 1. 空间音效系统

### 1.1 技术架构 Technical Architecture

**UE5 Audio Engine Components:**
- **Audio Engine**: UE5's core audio system
- **Sound Attenuation Settings**: 3D spatialization and falloff
- **Sound Class Mix**: Volume category management
- **Audio Volume**: Scene-based reverb and acoustics

### 1.2 音效分类 Audio Categories

#### Interaction SFX (交互音效)
- **Purpose**: Player interaction feedback
- **Priority**: High
- **3D Audio**: Enabled
- **Attenuation**: 3-10m range

#### UI SFX (界面音效)
- **Purpose**: Menu and UI feedback
- **Priority**: High
- **3D Audio**: Disabled (2D)
- **Volume**: Moderate

#### Ambient SFX (环境音效)
- **Purpose**: Scene atmosphere
- **Priority**: Medium
- **3D Audio**: Enabled
- **Attenuation**: 10-30m range

#### Launch SFX (发射音效)
- **Purpose**: Launch sequence effects
- **Priority**: High
- **3D Audio**: Enabled
- **Volume**: Maximum

#### Voice (语音)
- **Purpose**: Tutorial instructions
- **Priority**: Highest
- **3D Audio**: Optional
- **Ducking**: Lowers other audio

#### Music (音乐)
- **Purpose**: Background ambience
- **Priority**: Low
- **3D Audio**: Disabled
- **Volume**: Background level

---

## 2. 音效资产需求清单 Sound Assets Requirements

### 2.1 交互音效 Interaction SFX

**Directory**: `Content/RocketAssembly/Audio/SFX/Interaction/`

#### SFX_Grab_Metal.wav - 抓取金属零件 Grab Metal Part
- **Duration**: 0.2 - 0.5 seconds
- **Format**: WAV, 48kHz, 16-bit, Mono
- **Characteristics**: Metallic contact sound, mid-frequency dominant
- **3D Settings**: Spatial audio enabled
- **Attenuation**: 
  - Inner Radius: 5m
  - Outer Radius: 10m
  - Falloff: Natural (2.0)
- **Volume Range**: -6 dB to 0 dB
- **Reference**: Tool grab, metal handling
- **Usage**: Triggered on `GripActor` event

#### SFX_Release_Metal.wav - 放下零件 Release Part
- **Duration**: 0.3 - 0.6 seconds
- **Format**: WAV, 48kHz, 16-bit, Mono
- **Characteristics**: Light metal impact with reverb tail
- **3D Settings**: Spatial audio enabled
- **Attenuation**:
  - Inner Radius: 3m
  - Outer Radius: 8m
- **Volume**: -3 dB to 0 dB
- **Usage**: Triggered on `DropActor` event

#### SFX_Snap_Success.wav - 装配成功 Assembly Success
- **Duration**: 0.5 - 1.0 seconds
- **Format**: WAV, 48kHz, 16-bit, Mono
- **Characteristics**: Crisp snap + positive confirmation tone
- **Frequency**: 1000-2000 Hz (bright, pleasant)
- **3D Settings**: Spatial audio enabled
- **Attenuation**: 5m-12m
- **Emotional Impact**: Satisfying, rewarding
- **Usage**: Triggered on successful validation

#### SFX_Snap_Fail.wav - 装配失败 Assembly Failure
- **Duration**: 0.3 - 0.5 seconds
- **Format**: WAV, 48kHz, 16-bit, Mono
- **Characteristics**: Low-pitched error tone
- **Frequency**: 200-500 Hz (warning)
- **Volume**: -6 dB
- **Usage**: Triggered on validation failure

#### SFX_Part_Collision.wav - 零件碰撞 Part Collision
- **Duration**: 0.2 - 0.4 seconds
- **Format**: WAV, 48kHz, 16-bit, Mono
- **Characteristics**: Metal-on-metal impact
- **Dynamic Volume**: Scales with collision velocity (0.1 - 1.0)
- **Variants**: Need 3+ variations for randomization
- **Usage**: Triggered on `OnHit` event with impact velocity check

#### SFX_Magnetic_Snap.wav - 磁吸效果 Magnetic Snap
- **Duration**: 0.3 seconds
- **Format**: WAV, 48kHz, 16-bit, Mono
- **Characteristics**: Smooth magnetic pull, sci-fi feel
- **Fade In**: 0.1 seconds
- **Usage**: Triggered when part enters snap range

---

### 2.2 UI 音效 UI SFX

**Directory**: `Content/RocketAssembly/Audio/SFX/UI/`

#### SFX_Button_Click.wav - 按钮点击 Button Click
- **Duration**: 0.1 - 0.2 seconds
- **Characteristics**: Crisp, clear click
- **2D Audio**: No spatialization
- **Volume**: -3 dB
- **Usage**: All button OnClicked events

#### SFX_Button_Hover.wav - 按钮悬停 Button Hover
- **Duration**: 0.1 seconds
- **Characteristics**: Subtle UI feedback
- **Volume**: -12 dB
- **Usage**: Button OnHovered events

#### SFX_Menu_Open.wav - 菜单打开 Menu Open
- **Duration**: 0.3 seconds
- **Characteristics**: Rising tone, swoosh
- **Usage**: Menu panel appears

#### SFX_Menu_Close.wav - 菜单关闭 Menu Close
- **Duration**: 0.2 seconds
- **Characteristics**: Falling tone, swoosh
- **Usage**: Menu panel closes

#### SFX_Step_Complete.wav - 步骤完成 Step Complete
- **Duration**: 1.0 seconds
- **Characteristics**: Chime + chord progression
- **Emotional Impact**: High satisfaction
- **Usage**: Assembly step completion

#### SFX_Achievement_Unlock.wav - 成就解锁 Achievement Unlock
- **Duration**: 1.5 seconds
- **Characteristics**: Celebratory, layered effects
- **Layers**: Chime + fanfare + sparkle
- **Usage**: Achievement unlock event

#### SFX_Countdown_Tick.wav - 倒计时滴答 Countdown Tick
- **Duration**: 0.1 seconds
- **Interval**: 1 second
- **Characteristics**: Clear tick/beep
- **Usage**: Launch countdown timer

#### SFX_Score_Count.wav - 分数计算 Score Counting
- **Duration**: 0.05 seconds
- **Characteristics**: Digital increment sound
- **Usage**: Score counter animation (looped rapidly)

---

### 2.3 环境音效 Ambient SFX

**Directory**: `Content/RocketAssembly/Audio/SFX/Ambient/`

#### AMB_Workshop_Loop.wav - 车间环境音 Workshop Ambient Loop
- **Duration**: 30-60 seconds (seamless loop)
- **Format**: WAV, 48kHz, 16-bit, Stereo
- **Elements**:
  - Machinery hum (low frequency, constant)
  - Ventilation system (mid-frequency white noise)
  - Distant tools (occasional, sparse)
- **Volume**: -18 dB (background)
- **Loop**: Seamless crossfade
- **Usage**: L_AssemblyWorkshop background

#### AMB_Launch_Site_Loop.wav - 发射场环境音 Launch Site Ambient
- **Duration**: 30-60 seconds
- **Format**: WAV, 48kHz, 16-bit, Stereo
- **Elements**:
  - Wind (dynamic variation)
  - Equipment hum
  - Distant ocean/desert atmosphere
- **Volume**: -20 dB
- **Usage**: L_LaunchSite background

---

### 2.4 发射模拟音效 Launch Simulation SFX

**Directory**: `Content/RocketAssembly/Audio/SFX/Launch/`

#### SFX_Ignition.wav - 点火音效 Ignition
- **Duration**: 2.0 seconds
- **Format**: WAV, 48kHz, 16-bit, Mono
- **Stages**:
  - Initial explosion (0-0.3s): Sharp impact
  - Flame spray start (0.3-2.0s): Building roar
- **Low Frequency**: Heavy (20-200 Hz)
- **Volume**: 0 dB (maximum)
- **Usage**: Launch sequence start

#### SFX_Engine_Rumble_Loop.wav - 发动机轰鸣循环 Engine Rumble
- **Duration**: 5 seconds (loop)
- **Characteristics**: Deep continuous rumble
- **Low Frequency**: 30-100 Hz (sub-bass)
- **Haptic Feedback**: Strong vibration sync
- **Usage**: During powered flight

#### SFX_Liftoff.wav - 升空音效 Liftoff
- **Duration**: 3.0 seconds
- **Characteristics**: Acceleration sound, rising pitch
- **Pitch Shift**: +10% over duration
- **Usage**: Rocket begins ascending

#### SFX_Stage_Separation.wav - 级间分离 Stage Separation
- **Duration**: 1.5 seconds
- **Characteristics**: Explosive bolts + separation shockwave
- **Layers**: Mechanical release + air blast
- **Usage**: Multi-stage separation event

#### SFX_Sonic_Boom.wav - 音爆 Sonic Boom
- **Duration**: 0.8 seconds
- **Characteristics**: Sharp supersonic shockwave
- **Dynamics**: Very loud, sudden impact
- **Usage**: Breaking sound barrier

---

## 3. 语音系统实现 Voice System Implementation

### 3.1 中文语音资产 Chinese Voice Assets

**Directory**: `Content/RocketAssembly/Audio/Voice/CN/`

**Voice Actor Requirements 配音要求**:
- **Gender**: Female or male (female recommended for friendliness)
- **Language**: Standard Mandarin Chinese
- **Tone**: Clear, enthusiastic, educational
- **Quality**: 44.1kHz, 16-bit, Mono
- **Processing**: Noise reduction, EQ for clarity

#### 语音脚本列表 Voice Script List

**VO_CN_Welcome.wav**
```
"欢迎来到火箭装配VR科普教育系统！让我们一起探索火箭的奥秘。"
```
- **Duration**: ~5 seconds
- **Tone**: Warm, welcoming
- **Usage**: Main menu / tutorial start

**VO_CN_Step_01.wav**
```
"第一步：放置一级火箭基座。这是整个火箭的基础结构，为后续装配提供支撑。"
```
- **Duration**: ~7 seconds
- **Technical Note**: Clear pronunciation of "基座"

**VO_CN_Step_02.wav**
```
"第二步：安装主发动机。主发动机提供火箭升空所需的强大推力，是火箭的心脏。"
```

**VO_CN_Step_03.wav**
```
"第三步：连接涡轮泵。涡轮泵负责将燃料高速注入燃烧室，确保稳定燃烧。"
```

**VO_CN_Step_04.wav**
```
"第四步：安装液氧燃料舱。液氧作为氧化剂，是火箭燃烧的必要条件。"
```

**VO_CN_Step_05.wav**
```
"第五步：安装燃料舱。液氢燃料与液氧混合燃烧，产生巨大能量推动火箭。"
```

**VO_CN_Step_06.wav**
```
"第六步：连接燃料管路系统。管路系统确保燃料安全高效地输送到发动机。"
```

**VO_CN_Step_07.wav**
```
"第七步：安装级间段。级间段连接一级和二级火箭，在达到预定高度后分离。"
```

**VO_CN_Step_08.wav**
```
"第八步：安装二级火箭筒体。二级火箭在一级分离后继续提供推力，将载荷送入轨道。"
```

**VO_CN_Step_09.wav**
```
"第九步：安装姿态控制系统。它能精确调整火箭飞行姿态，确保按照预定轨迹飞行。"
```

**VO_CN_Step_10.wav**
```
"第十步：安装整流罩。整流罩保护载荷免受大气摩擦影响，在大气层外会自动抛离。"
```

**VO_CN_Complete.wav**
```
"恭喜！你已成功完成火箭装配。所有系统检查正常，准备发射！"
```

**VO_CN_Error_WrongPart.wav**
```
"注意！这个零件不正确，请选择正确的零件后重试。"
```

**VO_CN_Error_WrongSequence.wav**
```
"装配顺序错误，请按照正确的步骤进行装配。"
```

**VO_CN_Hint_LookHere.wav**
```
"请看这里，下一个需要装配的零件在你的右侧。"
```

**VO_CN_Launch_T10.wav**
```
"发射倒计时：10、9、8、7、6、5、4、3、2、1、点火！"
```
- **Duration**: ~12 seconds
- **Pacing**: 1 second per number
- **Emphasis**: Final "点火！" with energy

---

### 3.2 英文语音资产 English Voice Assets

**Directory**: `Content/RocketAssembly/Audio/Voice/EN/`

**Voice Actor Requirements**:
- **Gender**: Female or male (female recommended)
- **Language**: Clear English (American or British)
- **Tone**: Professional, educational, encouraging
- **Quality**: 44.1kHz, 16-bit, Mono

**VO_EN_Welcome.wav**
```
"Welcome to the VR Rocket Assembly Educational System! Let's explore the wonders of rocket science together."
```

**VO_EN_Step_01.wav**
```
"Step 1: Place the first-stage rocket base. This is the foundational structure of the entire rocket."
```

**VO_EN_Step_02.wav**
```
"Step 2: Install the main engine. The main engine provides the powerful thrust needed for liftoff."
```

**VO_EN_Step_03.wav**
```
"Step 3: Connect the turbopump. The turbopump injects fuel into the combustion chamber at high speed."
```

**VO_EN_Step_04.wav**
```
"Step 4: Install the liquid oxygen tank. Liquid oxygen acts as the oxidizer for rocket combustion."
```

**VO_EN_Step_05.wav**
```
"Step 5: Install the fuel tank. Liquid hydrogen fuel combines with liquid oxygen to generate thrust."
```

**VO_EN_Step_06.wav**
```
"Step 6: Connect the fuel line system. The fuel lines ensure safe and efficient propellant delivery."
```

**VO_EN_Step_07.wav**
```
"Step 7: Install the interstage section. This connects the first and second stages, separating at altitude."
```

**VO_EN_Step_08.wav**
```
"Step 8: Install the second-stage body. The second stage continues propulsion after first-stage separation."
```

**VO_EN_Step_09.wav**
```
"Step 9: Install the attitude control system. This precisely adjusts the rocket's flight trajectory."
```

**VO_EN_Step_10.wav**
```
"Step 10: Install the payload fairing. The fairing protects the payload from atmospheric friction."
```

**VO_EN_Complete.wav**
```
"Congratulations! You have successfully assembled the rocket. All systems are go. Ready for launch!"
```

**VO_EN_Error_WrongPart.wav**
```
"Attention! This is the wrong part. Please select the correct part and try again."
```

**VO_EN_Error_WrongSequence.wav**
```
"Assembly sequence error. Please follow the correct steps."
```

**VO_EN_Hint_LookHere.wav**
```
"Look here. The next part to assemble is on your right side."
```

**VO_EN_Launch_T10.wav**
```
"Launch countdown: 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, Ignition!"
```

---

## 4. 背景音乐系统 Background Music System

**Directory**: `Content/RocketAssembly/Audio/Music/`

### 4.1 音乐轨道列表 Music Track List

#### MUS_MainMenu_Loop.ogg - 主菜单音乐 Main Menu Music
- **Style**: Atmospheric, technological, spacey
- **BPM**: 80-100
- **Duration**: 2-3 minutes (seamless loop)
- **Format**: OGG Vorbis, Stereo, Quality 7
- **Instruments**: Synthesizers, electronic pads, ambient sounds
- **Mood**: Calm, mysterious, anticipatory
- **Reference**: Mass Effect main theme, Interstellar soundtrack
- **Usage**: Main menu scene

#### MUS_Assembly_Calm_Loop.ogg - 装配音乐(平静) Assembly Music (Calm)
- **Style**: Relaxing, focused, ambient
- **BPM**: 70-90
- **Duration**: 3-4 minutes
- **Instruments**: Piano, strings, light percussion
- **Mood**: Concentration, learning
- **Usage**: Tutorial mode background

#### MUS_Assembly_Focus_Loop.ogg - 装配音乐(专注) Assembly Music (Focus)
- **Style**: Rhythmic, motivating, energetic
- **BPM**: 90-110
- **Duration**: 3-4 minutes
- **Instruments**: Electronic beats, synth layers
- **Mood**: Active, engaged
- **Usage**: Assessment mode background

#### MUS_Countdown.ogg - 发射倒计时音乐 Countdown Music
- **Style**: Tense, building, epic
- **BPM**: 120-140
- **Duration**: 30-60 seconds (non-looping)
- **Instruments**: Percussion, brass, strings
- **Structure**: Building intensity, climax at T-0
- **Mood**: Excitement, anticipation
- **Usage**: Launch countdown sequence

#### MUS_Launch.ogg - 发射音乐 Launch Music
- **Style**: Epic, grand, triumphant
- **BPM**: 140-160
- **Duration**: 90 seconds
- **Instruments**: Full orchestra
- **Reference**: Two Steps From Hell, Hans Zimmer
- **Mood**: Awe, achievement
- **Usage**: Launch sequence

#### MUS_Success.ogg - 完成庆祝音乐 Success Celebration
- **Style**: Upbeat, victorious, celebratory
- **BPM**: 120
- **Duration**: 20-30 seconds
- **Mood**: Joy, accomplishment
- **Usage**: Mission complete

---

## 5. Blueprint 音频管理器 Blueprint Audio Manager

### 5.1 BP_AdvancedAudioManager

**File**: `Content/RocketAssembly/Blueprints/BP_AdvancedAudioManager.uasset`

**Parent Class**: Actor

#### 变量 Variables

```cpp
// 音量控制 Volume Controls
float MasterVolume = 1.0f;        // 主音量 Master Volume (0-1)
float SFXVolume = 0.8f;           // 音效音量 SFX Volume (0-1)
float VoiceVolume = 1.0f;         // 语音音量 Voice Volume (0-1)
float MusicVolume = 0.6f;         // 音乐音量 Music Volume (0-1)
float AmbientVolume = 0.5f;       // 环境音量 Ambient Volume (0-1)

// 语言设置 Language Settings
ELanguage CurrentLanguage = CN;   // 当前语言 Current Language
// Enum: CN, EN

// 音频组件 Audio Components
UAudioComponent* CurrentMusicComponent;    // 当前音乐组件
UAudioComponent* VoiceComponent;           // 语音组件
UAudioComponent* AmbientComponent;         // 环境音组件

// 音效缓存 Sound Effect Cache
TMap<FName, USoundBase*> SFXSounds;        // 音效缓存映射
TMap<FName, USoundBase*> VoiceSounds_CN;   // 中文语音
TMap<FName, USoundBase*> VoiceSounds_EN;   // 英文语音
TMap<FName, USoundBase*> MusicTracks;      // 音乐轨道

// 音频池 Audio Pool
TArray<UAudioComponent*> SFXPool;          // 音效播放池 (10个组件)
int32 CurrentSFXIndex = 0;                 // 当前池索引

// Sound Classes
USoundClass* MasterSoundClass;
USoundClass* SFXSoundClass;
USoundClass* VoiceSoundClass;
USoundClass* MusicSoundClass;
USoundClass* AmbientSoundClass;

// Sound Mix
USoundMix* GameSoundMix;

// 状态标志 State Flags
bool bVoiceIsPlaying = false;              // 语音播放中
bool bMusicIsPlaying = false;              // 音乐播放中
FName CurrentMusicTrack;                   // 当前音乐轨道名称
```

#### 函数 Functions

##### BeginPlay()
```
功能: 初始化音频管理器
步骤:
1. Load all sound assets into caches
2. Create SFX audio component pool (10 components)
3. Setup sound classes and mix
4. Load settings from SaveGame
5. Apply volume settings
6. Register as global audio manager
```

##### PlaySFX(SFXName, Location, Volume, Pitch)
```
输入:
- SFXName: FName - 音效名称 (e.g., "Grab_Metal")
- Location: FVector - 3D 位置 (optional, use (0,0,0) for 2D)
- Volume: float - 音量倍数 (default 1.0)
- Pitch: float - 音调倍数 (default 1.0)

逻辑:
1. Get next available audio component from pool (round-robin)
2. Find sound in SFXSounds map
3. If not found, log warning and return
4. Set sound, location, volume (SFXVolume * MasterVolume * Volume)
5. Set pitch multiplier
6. Set sound class to SFXSoundClass
7. Enable 3D spatialization if Location is not zero
8. Play sound
9. Increment pool index

用途: 播放任意音效
```

##### PlayVoice(VoiceKey, bInterruptCurrent)
```
输入:
- VoiceKey: FName - 语音key (e.g., "Step_01", "Welcome")
- bInterruptCurrent: bool - 是否打断当前语音

逻辑:
1. If bVoiceIsPlaying and not bInterruptCurrent, queue or ignore
2. Select voice map based on CurrentLanguage
3. Find voice sound in appropriate map
4. If not found, log warning and return
5. Stop current voice if playing
6. Set VoiceComponent sound
7. Apply VoiceVolume * MasterVolume
8. Set sound class to VoiceSoundClass
9. Bind OnAudioFinished to VoiceFinishedCallback
10. Play voice
11. Set bVoiceIsPlaying = true
12. Trigger voice ducking (lower music/ambient)

用途: 播放教学语音
```

##### VoiceFinishedCallback()
```
逻辑:
1. Set bVoiceIsPlaying = false
2. Remove voice ducking (restore music/ambient volumes)
3. Fire OnVoiceFinished event
```

##### PlayMusic(MusicTrackName, bFadeIn, FadeTime)
```
输入:
- MusicTrackName: FName - 音乐轨道名称
- bFadeIn: bool - 是否淡入
- FadeTime: float - 淡入时间(秒)

逻辑:
1. If same track is playing, return
2. If music is playing, fade out current music
3. Find music in MusicTracks map
4. Set CurrentMusicComponent sound
5. Set looping = true
6. Apply MusicVolume * MasterVolume
7. Set sound class to MusicSoundClass
8. If bFadeIn:
   - Start at volume 0
   - Play music
   - Fade to target volume over FadeTime
9. Else:
   - Play at target volume
10. Set CurrentMusicTrack
11. Set bMusicIsPlaying = true

用途: 播放背景音乐
```

##### StopMusic(bFadeOut, FadeTime)
```
逻辑:
1. If not playing, return
2. If bFadeOut:
   - Fade volume to 0 over FadeTime
   - Then stop component
3. Else:
   - Stop immediately
4. Set bMusicIsPlaying = false
5. Clear CurrentMusicTrack
```

##### SetMasterVolume(NewVolume)
```
逻辑:
1. Clamp volume to 0-1
2. Set MasterVolume variable
3. Update MasterSoundClass volume
4. Save to game settings
```

##### SetSFXVolume(NewVolume)
```
逻辑:
1. Clamp to 0-1
2. Set SFXVolume
3. Update SFXSoundClass volume
4. Save to settings
```

##### SetVoiceVolume(NewVolume)
```
Similar to SetSFXVolume for VoiceSoundClass
```

##### SetMusicVolume(NewVolume)
```
Similar, plus update CurrentMusicComponent if playing
```

##### SetLanguage(NewLanguage)
```
输入: ELanguage NewLanguage

逻辑:
1. Set CurrentLanguage
2. If voice is playing, stop it
3. Save to settings
4. Fire OnLanguageChanged event
```

##### LoadAudioAssets()
```
功能: 加载所有音频资产到缓存
逻辑:
1. Load all SFX sounds from /Audio/SFX/ folders
2. Load all CN voice sounds from /Audio/Voice/CN/
3. Load all EN voice sounds from /Audio/Voice/EN/
4. Load all music tracks from /Audio/Music/
5. Populate respective TMaps
6. Log loading results
```

##### CreateAudioPool()
```
功能: 创建音效组件池
逻辑:
1. For i = 0 to 9:
   - Create UAudioComponent
   - Attach to root
   - Add to SFXPool array
2. Set CurrentSFXIndex = 0
```

#### 事件 Events

```
// 事件委托 Event Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVoiceFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLanguageChanged, ELanguage, NewLanguage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMusicChanged, FName, OldTrack, FName, NewTrack);

FOnVoiceFinished OnVoiceFinished;
FOnLanguageChanged OnLanguageChanged;
FOnMusicChanged OnMusicChanged;
```

---

## 6. Sound Cue 配置指南 Sound Cue Configuration

### 6.1 SC_PartCollision - 零件碰撞 Sound Cue

**File**: `Content/RocketAssembly/Audio/SoundCues/SC_PartCollision.uasset`

**Purpose**: Random variation collision sounds with velocity-based volume

**Node Structure**:
```
[Random Node] ─┬─ SFX_Part_Collision_01
               ├─ SFX_Part_Collision_02
               └─ SFX_Part_Collision_03
       │
       ↓
[Attenuation Node] (5m-10m)
       │
       ↓
[Output]
```

**Parameters**:
- Impact Velocity (float): Controls volume multiplier
  - Range: 0.0 - 10.0 (m/s)
  - Volume curve: Linear from 0.1 to 1.0

**Blueprint Usage**:
```
On Component Hit:
  float ImpactSpeed = HitResult.ImpactVelocity.Size();
  PlaySoundAtLocation(SC_PartCollision, HitLocation);
  // Volume auto-scaled by cue parameter
```

---

### 6.2 SC_AssemblySuccess - 装配成功 Sound Cue

**Node Structure**:
```
[Delay 0s] ── SFX_Snap_Success
       │
       ↓
[Delay 0.3s] ── SFX_Achievement_Chime (optional celebratory layer)
       │
       ↓
[Attenuation] (5m-12m)
       │
       ↓
[Output]
```

---

### 6.3 SC_Launch_Sequence - 发射序列 Sound Cue

**Purpose**: Complex launch audio sequence

**Node Structure**:
```
[Concatenator Node] ─┬─ [0s] SFX_Ignition (2s duration)
                     ├─ [2s] SFX_Liftoff (3s duration)
                     └─ [5s] SFX_Engine_Rumble_Loop (looping)
       │
       ↓
[Attenuation] (10m-50m)
       │
       ↓
[Output]
```

**Usage**:
- Triggered at launch countdown T-0
- Automatically sequences ignition, liftoff, and sustained engine sounds

---

## 7. 3D 空间音效设置 3D Spatial Audio Settings

### 7.1 Sound Attenuation Settings

#### SA_Interaction - 交互音效衰减 Interaction Attenuation

**File**: `Content/RocketAssembly/Audio/Attenuation/SA_Interaction.uasset`

```
Attenuation Shape: Sphere
Attenuation Distance:
  - Inner Radius: 300 cm (3m)
  - Falloff Distance: 700 cm (7m)
  - Total Range: 1000 cm (10m)

Distance Algorithm: Natural Sound (Falloff Exponent 2.0)

Spatialization:
  - Enable: true
  - Spatialization Algorithm: Binaural (for VR)
  - Enable Occlusion: true
  - Occlusion Trace Channel: Visibility

Volume Attenuation:
  - Curve: Natural falloff
  - Min Volume: 0.0
  - Max Volume: 1.0

Air Absorption:
  - Enable: true
  - Air Absorption Gain HF: 0.9994
```

---

#### SA_Ambient - 环境音效衰减 Ambient Attenuation

```
Attenuation Shape: Sphere
Attenuation Distance:
  - Inner Radius: 1000 cm (10m)
  - Falloff Distance: 2000 cm (20m)
  - Total Range: 3000 cm (30m)

Distance Algorithm: Linear
Spatialization: Enable binaural
Enable Occlusion: false (ambient sound)
```

---

#### SA_Launch - 发射音效衰减 Launch Attenuation

```
Attenuation Shape: Sphere
Attenuation Distance:
  - Inner Radius: 2000 cm (20m)
  - Falloff Distance: 3000 cm (30m)
  - Total Range: 5000 cm (50m)

Distance Algorithm: Natural (2.0)
Enable Occlusion: true
Volume: Very loud, dramatic falloff
```

---

### 7.2 Reverb Settings 混响设置

#### AR_Workshop - 车间混响 Workshop Reverb

**File**: `Content/RocketAssembly/Audio/Reverb/AR_Workshop.uasset`

**Audio Volume Settings**:
```
Reverb Effect: Workshop Reverb
Preset: Medium Room
Parameters:
  - Reverb Time: 1.2s
  - Reverb Gain: -6 dB
  - Early Reflections: -4 dB
  - Late Reflections: -8 dB
  - Diffusion: 0.7
  - Density: 0.8

Apply To: All spatialized sounds in L_AssemblyWorkshop
```

---

## 8. 手柄震动反馈集成 Haptic Feedback Integration

### 8.1 Haptic Effect Assets

**Directory**: `Content/RocketAssembly/Audio/Haptics/`

#### HE_PartGrab - 抓取震动 Grab Haptic

**File**: `Content/RocketAssembly/Audio/Haptics/HE_PartGrab.uasset`

```
Duration: 0.15 seconds
Intensity Curve:
  - 0.0s: 0.0
  - 0.05s: 0.7
  - 0.15s: 0.0

Frequency: 80 Hz
Controller: Both hands
```

**Blueprint Usage**:
```
On Grip Actor:
  PlayHapticEffect(HE_PartGrab, ControllerHand);
  PlaySFX("Grab_Metal", PartLocation);
```

---

#### HE_PartSnap - 装配震动 Snap Haptic

```
Duration: 0.3 seconds
Intensity Curve:
  - 0.0s: 0.0
  - 0.1s: 0.9
  - 0.2s: 0.4
  - 0.3s: 0.0

Frequency: 120 Hz
Purpose: Satisfying confirmation feedback
```

---

#### HE_EngineRumble - 发动机震动 Engine Rumble

```
Duration: Continuous (loop until stopped)
Intensity: 0.6 (constant)
Frequency: 40 Hz (low rumble)
Purpose: Launch sequence immersion
```

---

### 8.2 BP_HapticManager

**File**: `Content/RocketAssembly/Blueprints/BP_HapticManager.uasset`

**Functions**:

##### PlayHapticWithSound(SFXName, HapticEffect, Hand, Location)
```
功能: 同步播放音效和震动
输入:
  - SFXName: FName
  - HapticEffect: UHapticFeedbackEffect_Base*
  - Hand: EControllerHand (Left/Right/Both)
  - Location: FVector (for 3D sound)

逻辑:
1. Get Audio Manager
2. AudioManager->PlaySFX(SFXName, Location)
3. PlayHapticEffect(HapticEffect, Hand)
```

##### StopContinuousHaptic(Hand)
```
Stops looping haptic effects on specified hand(s)
```

---

## 9. 音效触发时机 Sound Triggering

### 9.1 交互音效触发 Interaction Sound Triggers

#### 在 BP_InteractiveRocketPart 中 In BP_InteractiveRocketPart

##### On Gripped Event
```
Event Gripped:
  ├─ Get Audio Manager
  ├─ Get Part Location
  ├─ Play SFX: "Grab_Metal"
  │   └─ Location: Part Location
  │   └─ Volume: 1.0
  ├─ Play Haptic: HE_PartGrab
  └─ Set bIsBeingHeld = true
```

##### On Dropped Event
```
Event Dropped:
  ├─ Get Audio Manager
  ├─ Get Part Location
  ├─ Play SFX: "Release_Metal"
  │   └─ Location: Part Location
  │   └─ Volume: 0.8
  └─ Set bIsBeingHeld = false
```

##### On Assembly Success Event
```
Event OnPartAssembled:
  ├─ Get Audio Manager
  ├─ Get Socket Location
  ├─ Play SFX: "Snap_Success"
  │   └─ Location: Socket Location
  │   └─ Volume: 1.0
  ├─ Play Haptic: HE_PartSnap (to gripping hand)
  ├─ Play VFX: NS_AssemblySuccess
  └─ Notify Tutorial Manager (step complete)
```

##### On Assembly Failure Event
```
Event OnAssemblyFailed:
  ├─ Get Audio Manager
  ├─ Play SFX: "Snap_Fail"
  │   └─ Volume: 0.7
  ├─ Play VFX: NS_AssemblyFail (red particles)
  └─ Optional: Play Voice hint
```

##### On Component Hit Event
```
Event OnComponentHit:
  ├─ Get Impact Velocity
  ├─ If Velocity > 50 cm/s:
  │   ├─ Get Audio Manager
  │   ├─ Get Hit Location
  │   └─ Play Sound Cue: SC_PartCollision
  │       └─ Location: Hit Location
  │       └─ Velocity Parameter: Impact Velocity
  └─ Else: No sound (too soft)
```

---

### 9.2 UI 音效触发 UI Sound Triggers

#### 在 WBP_MainMenu 中 In WBP_MainMenu

##### Button Interactions
```
Any Button:
  ├─ On Hovered:
  │   └─ Play SFX: "Button_Hover"
  └─ On Clicked:
      ├─ Play SFX: "Button_Click"
      └─ Execute button action

Menu Open/Close:
  ├─ On Panel Appear:
  │   └─ Play SFX: "Menu_Open"
  └─ On Panel Disappear:
      └─ Play SFX: "Menu_Close"
```

---

#### 在 BP_AssemblyTutorialManager 中 In BP_AssemblyTutorialManager

##### Step Progression
```
On Step Complete:
  ├─ Play SFX: "Step_Complete"
  ├─ Delay 0.5s
  ├─ Play Voice: "Step_XX" (next step)
  └─ Update UI

On All Steps Complete:
  ├─ Play SFX: "Achievement_Unlock"
  ├─ Play Voice: "Complete"
  ├─ Play Music: "Success"
  └─ Show score screen
```

---

### 9.3 发射序列音效 Launch Sequence Sounds

#### 在 BP_LaunchSequenceManager 中 In BP_LaunchSequenceManager

##### Launch Countdown
```
Function: StartLaunch()
  ├─ Play Music: "Countdown" (fade in 2s)
  ├─ For T = 10 to 1:
  │   ├─ Play SFX: "Countdown_Tick"
  │   ├─ Play Voice: "Launch_T10" (full countdown voice)
  │   └─ Wait 1 second
  ├─ At T=0:
  │   ├─ Stop Music: "Countdown"
  │   ├─ Play Sound Cue: SC_Launch_Sequence
  │   ├─ Play Music: "Launch" (epic music)
  │   ├─ Play Haptic: HE_EngineRumble (continuous, both hands)
  │   ├─ Start rocket animation
  │   └─ Trigger VFX: NS_RocketExhaust
  ├─ At T+5s (stage separation):
  │   ├─ Play SFX: "Stage_Separation"
  │   └─ Play Haptic: HE_PartSnap (sharp pulse)
  ├─ At T+20s (sonic boom):
  │   ├─ Play SFX: "Sonic_Boom"
  │   └─ Delay 2s (for distance realism)
  └─ At T+30s (sequence end):
      ├─ Stop Haptic: HE_EngineRumble
      ├─ Fade Out Music: "Launch" (5s)
      └─ Return to assembly scene
```

---

## 10. 音频优化与性能 Audio Optimization and Performance

### 10.1 音频资源优化 Audio Asset Optimization

#### 压缩设置 Compression Settings

**SFX Sounds (Short effects < 2s)**:
```
Compression: ADPCM
Quality: 40 (good quality, small size)
Sample Rate: 22050 Hz (sufficient for effects)
Channels: Mono (3D spatialization)
Loading Behavior: Load on Demand
```

**Voice (Instructional)**:
```
Compression: Vorbis Quality 60
Sample Rate: 44100 Hz (clear speech)
Channels: Mono
Loading Behavior: Preload (avoid delays)
```

**Music (Loops)**:
```
Compression: Opus (Vorbis)
Quality: 70 (high quality for music)
Sample Rate: 44100 Hz
Channels: Stereo
Loading Behavior: Stream from Disk
```

**Ambient Loops**:
```
Compression: Opus/Vorbis Quality 50
Sample Rate: 44100 Hz
Channels: Stereo
Loading Behavior: Stream from Disk
```

---

### 10.2 Audio Component Pooling 音频组件池

**Problem**: Creating audio components dynamically is expensive

**Solution**: Audio component pool in BP_AdvancedAudioManager
- Pre-create 10 UAudioComponent instances
- Reuse components in round-robin fashion
- Reduces garbage collection pressure

---

### 10.3 Voice Prioritization 语音优先级

**System**: Voice audio always takes precedence
- When voice plays, apply ducking to music/ambient
- Reduce music volume to 30%
- Reduce ambient volume to 40%
- Restore volumes when voice finishes

---

### 10.4 Sound Concurrency 音频并发控制

**Configure Sound Concurrency Settings**:

**SFX Concurrency**:
```
Max Count: 16
Resolution Rule: Stop Oldest
Volume Scale: 1.0
Duck Amount: 0 dB (no ducking between SFX)
```

**Voice Concurrency**:
```
Max Count: 1 (only one voice at a time)
Resolution Rule: Stop Oldest
Ducking: Apply to Music and Ambient classes
Duck Amount: -10 dB
Fade Time: 0.5s
```

**Music Concurrency**:
```
Max Count: 1
Resolution Rule: Stop Oldest
```

---

### 10.5 Audio LOD 音频LOD

**Distance-based complexity**:
- **Near (< 5m)**: Full quality, all layers
- **Medium (5-15m)**: Standard quality
- **Far (> 15m)**: Simplified/muted

**Implementation**:
```
In SFX_Engine_Rumble Sound Cue:
  Add Distance Attenuation
  Add Volume Curve based on distance
  Reduce sample rate at distance (not typically done in UE5, but conceptual)
```

---

### 10.6 Memory Budget 内存预算

**Target Audio Memory**:
- **Total Audio Memory**: < 50 MB
  - SFX: ~10 MB (compressed)
  - Voice (CN+EN): ~15 MB (preloaded)
  - Music: ~20 MB (streaming, not in memory)
  - Ambient: ~5 MB (streaming)

**Monitoring**:
```
Console Command: "stat soundwaves"
Console Command: "stat soundmixes"
```

---

### 10.7 VR Audio Best Practices VR 音频最佳实践

1. **Always use binaural spatialization** for immersion
2. **Keep UI sounds 2D** (no spatialization for menus)
3. **Use head-locked sounds sparingly** (can cause nausea)
4. **Match haptic timing** with audio for cohesion
5. **Avoid sudden loud sounds** (can startle VR users)
6. **Test with actual VR headsets** (spatial audio varies)
7. **Provide volume controls** for accessibility
8. **Use audio to guide attention** (directional cues)

---

## 11. 测试与验证 Testing and Validation

### 11.1 音频测试清单 Audio Testing Checklist

#### 功能测试 Functional Testing
- [ ] All SFX trigger correctly on interactions
- [ ] Voice instructions play in correct language
- [ ] Music transitions smoothly between tracks
- [ ] Volume controls affect all audio categories
- [ ] Haptic feedback synchronizes with sounds
- [ ] 3D spatialization works correctly (test by moving around)
- [ ] Occlusion works (sound muffled behind walls)
- [ ] Audio ducking works when voice plays
- [ ] Sound pool doesn't run out (no missing sounds)
- [ ] SaveGame loads/saves audio settings

#### 性能测试 Performance Testing
- [ ] Audio memory < 50 MB
- [ ] No audio hitches or stutters
- [ ] Streaming works smoothly for music
- [ ] No excessive audio components created
- [ ] FPS remains stable (90+ for PC VR)

#### VR 舒适度测试 VR Comfort Testing
- [ ] No sudden loud sounds causing discomfort
- [ ] Spatial audio feels natural
- [ ] Head-tracking audio updates smoothly
- [ ] Haptic feedback not too intense
- [ ] Voice instructions clear and understandable

#### 多语言测试 Localization Testing
- [ ] All CN voice assets present and correct
- [ ] All EN voice assets present and correct
- [ ] Language switching works correctly
- [ ] Fallback to default language if asset missing

---

### 11.2 音频调试工具 Audio Debugging Tools

**UE5 Console Commands**:
```
stat sounds               // Show active sounds count
stat soundwaves           // Show loaded sound memory
stat soundmixes           // Show sound mix states
au.Debug.Sounds 1         // Visual debug for 3D sounds
au.3dVisualize.Enabled 1  // Show attenuation spheres
au.IsAudioMixerEnabled    // Check if new audio engine is active
```

**Blueprint Debugging**:
- Add Print String nodes after PlaySFX calls
- Log audio component status
- Display current music track name on HUD
- Show voice subtitle for debugging

---

## 12. 实施时间线 Implementation Timeline

### Week 1: 音效资产创建 SFX Asset Creation
- Day 1-2: Record/source interaction SFX (grab, release, snap, collisions)
- Day 3-4: Record/source UI SFX (buttons, menus, achievements)
- Day 5: Record/source ambient and launch SFX
- Deliverable: All SFX files ready and imported

### Week 2: 语音资产创建 Voice Asset Creation
- Day 1-2: Record Chinese voice lines (hire voice actor)
- Day 3-4: Record English voice lines
- Day 5: Edit and process voice files
- Deliverable: All voice files ready (CN + EN)

### Week 3: 音乐创作 Music Creation
- Day 1-2: Compose/source main menu and assembly music
- Day 3-4: Compose/source countdown and launch music
- Day 5: Mix and master music tracks
- Deliverable: All music tracks ready

### Week 4: Blueprint 实现 Blueprint Implementation
- Day 1-2: Implement BP_AdvancedAudioManager
- Day 3: Create Sound Cues (collision, success, launch sequence)
- Day 4: Setup Sound Attenuation and Reverb
- Day 5: Implement haptic feedback integration
- Deliverable: Audio system fully functional

### Week 5: 集成与测试 Integration and Testing
- Day 1-2: Integrate audio triggers into all Blueprints
- Day 3: Test all audio in VR
- Day 4: Optimize and fix issues
- Day 5: Final polish and validation
- Deliverable: Audio system complete and tested

---

## 13. 替代方案与临时资产 Alternatives and Placeholder Assets

### 13.1 临时音效来源 Placeholder Sound Sources

If professional audio assets are not ready:

**Free Sound Libraries**:
- **Freesound.org**: Large library of CC-licensed sounds
- **Zapsplat.com**: Free sound effects (with attribution)
- **BBC Sound Effects**: Free for personal/educational use
- **YouTube Audio Library**: Royalty-free music and SFX

**Search Terms**:
- Metal grab, metal impact, snap, click
- UI button, menu, notification, achievement
- Workshop ambient, machinery, ventilation
- Rocket launch, engine, ignition, explosion

### 13.2 文本转语音 Text-to-Speech Alternatives

If professional voice acting is not available:

**TTS Services**:
- **Google Cloud TTS**: High-quality neural voices (CN + EN)
- **Amazon Polly**: Natural-sounding TTS
- **Microsoft Azure TTS**: SSML support for emphasis
- **Locally**: Unreal Engine's built-in TTS (lower quality)

**Recommendation**: Use Google Cloud TTS with WaveNet voices
- CN voice: "cmn-CN-Wavenet-C" (female, clear)
- EN voice: "en-US-Wavenet-F" (female, friendly)

### 13.3 免版税音乐 Royalty-Free Music

**Music Libraries**:
- **Epidemic Sound**: Subscription-based, high quality
- **AudioJungle**: Purchase individual tracks
- **Incompetech**: Free with attribution (Kevin MacLeod)
- **Purple Planet**: Free royalty-free music

---

## 14. 总结与交付 Summary and Deliverables

### 14.1 完成标准 Completion Criteria

Audio System is considered complete when:
- ✅ All 50+ sound assets imported and organized
- ✅ Chinese and English voice lines recorded
- ✅ Background music tracks implemented
- ✅ BP_AdvancedAudioManager fully functional
- ✅ Sound Cues configured for complex audio
- ✅ 3D spatial audio working correctly in VR
- ✅ Haptic feedback synchronized with audio
- ✅ Audio triggers integrated into all interactions
- ✅ Volume controls and settings working
- ✅ Language switching functional
- ✅ Performance targets met (< 50 MB, smooth playback)
- ✅ Tested thoroughly in VR headset

### 14.2 交付清单 Deliverables Checklist

**Assets 资产**:
- [ ] Interaction SFX (10+ files)
- [ ] UI SFX (8+ files)
- [ ] Ambient SFX (2+ files)
- [ ] Launch SFX (5+ files)
- [ ] Chinese Voice (15+ files)
- [ ] English Voice (15+ files)
- [ ] Background Music (6 files)

**Blueprints 蓝图**:
- [ ] BP_AdvancedAudioManager
- [ ] BP_HapticManager (optional, can integrate into audio manager)
- [ ] Audio triggers in existing Blueprints (parts, UI, tutorial)

**Configuration 配置**:
- [ ] Sound Cues (3+)
- [ ] Sound Attenuation Settings (3+)
- [ ] Sound Classes (5)
- [ ] Sound Mix (1)
- [ ] Haptic Effects (3+)

**Documentation 文档**:
- [x] This implementation guide
- [ ] Audio asset list spreadsheet
- [ ] Voice script document (for voice actors)

---

### 14.3 下一步 Next Steps

After completing the audio system:
1. Proceed to **Phase 3 Document 2**: Launch Simulation Implementation
2. Integrate audio with launch sequence animations
3. Test complete experience with audio immersion
4. Gather user feedback on audio effectiveness
5. Iterate and polish based on testing

---

## 附录 Appendix

### A. 音频文件命名规范 Audio File Naming Convention

**SFX**:
```
SFX_<Category>_<Description>.wav
Examples:
  SFX_Interaction_GrabMetal.wav
  SFX_UI_ButtonClick.wav
  SFX_Launch_Ignition.wav
```

**Voice**:
```
VO_<Language>_<Context>.wav
Examples:
  VO_CN_Step_01.wav
  VO_EN_Welcome.wav
  VO_CN_Error_WrongPart.wav
```

**Music**:
```
MUS_<Purpose>_<Mood>.ogg
Examples:
  MUS_MainMenu_Loop.ogg
  MUS_Assembly_Calm_Loop.ogg
  MUS_Launch.ogg
```

---

### B. 音频参数快速参考 Audio Parameters Quick Reference

| Category | Format | Sample Rate | Channels | Compression | Loading |
|----------|--------|-------------|----------|-------------|---------|
| SFX      | WAV    | 22-48 kHz   | Mono     | ADPCM 40    | On Demand |
| Voice    | WAV    | 44.1 kHz    | Mono     | Vorbis 60   | Preload |
| Music    | OGG    | 44.1 kHz    | Stereo   | Vorbis 70   | Stream |
| Ambient  | WAV/OGG | 44.1 kHz   | Stereo   | Vorbis 50   | Stream |

---

### C. 联系与支持 Contact and Support

For questions or issues with audio implementation:
- **GitHub Issues**: https://github.com/EVRTOP/VRExpPluginExample_AI/issues
- **Documentation**: Refer to UE5 Audio documentation
- **Community**: Unreal Engine forums, VR development communities

---

**文档版本 Document Version**: 1.0.0
**最后更新 Last Updated**: 2026-01-03
**作者 Author**: VRExpPluginExample_AI Team
**状态 Status**: Complete and Ready for Implementation

---

**Phase 3 Audio System Implementation Guide Complete!**
**Phase 3 音频系统集成指南完成！🎵🚀**
