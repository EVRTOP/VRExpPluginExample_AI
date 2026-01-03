# Phase 2 - Audio System Implementation Guide
# 火箭装配系统 Phase 2 音频系统实现指南

## 📋 Overview 概述

This guide provides specifications for the audio system including sound effects, voice instructions, music, and spatial audio setup.

本指南提供音频系统的规范，包括音效、语音指导、音乐和空间音频设置。

---

## Audio Asset Organization 音频资产组织

### Directory Structure 目录结构

```
Content/RocketAssembly/Audio/
├── SFX/                          # Sound Effects
│   ├── Interaction/             # Interaction sounds
│   │   ├── SFX_Grab_Metal.wav
│   │   ├── SFX_Release_Metal.wav
│   │   ├── SFX_Snap_Success.wav
│   │   ├── SFX_Snap_Fail.wav
│   │   ├── SFX_Part_Collision.wav
│   │   └── SFX_Highlight_On.wav
│   │
│   ├── UI/                      # UI sounds
│   │   ├── SFX_Button_Click.wav
│   │   ├── SFX_Button_Hover.wav
│   │   ├── SFX_Menu_Open.wav
│   │   ├── SFX_Menu_Close.wav
│   │   ├── SFX_Step_Complete.wav
│   │   └── SFX_Achievement_Unlock.wav
│   │
│   └── Ambient/                 # Ambient sounds
│       ├── AMB_Workshop_Loop.wav
│       ├── AMB_Machinery.wav
│       └── AMB_Ventilation.wav
│
├── Voice/                        # Voice Instructions
│   ├── CN/                      # Chinese
│   │   ├── Welcome.wav
│   │   ├── Step_01.wav
│   │   ├── Step_02.wav
│   │   ├── ...
│   │   └── Step_10.wav
│   │
│   └── EN/                      # English
│       ├── Welcome.wav
│       ├── Step_01.wav
│       ├── Step_02.wav
│       ├── ...
│       └── Step_10.wav
│
└── Music/                        # Background Music
    ├── MUS_MainMenu_Loop.wav
    ├── MUS_Assembly_Loop.wav
    ├── MUS_Countdown.wav
    └── MUS_Success.wav
```

---

## Sound Effect Specifications 音效规格

### General Audio Guidelines 通用音频指南

**Technical Specs 技术规格**
- Sample Rate: 44.1 kHz or 48 kHz
- Bit Depth: 16-bit (sufficient for most SFX)
- Format: WAV (uncompressed) for import, UE converts to platform format
- Mono vs Stereo: 
  - 3D spatial sounds: Mono
  - UI sounds: Stereo
  - Music: Stereo

**Duration Guidelines 时长指南**
- UI clicks: 50-200 ms
- Interaction sounds: 200-800 ms
- Ambient loops: 30-120 seconds (seamless loop)
- Music: 2-5 minutes (looping)

**Volume Levels 音量级别**
- Peak amplitude: -3 dB (prevent clipping)
- Average: -12 to -18 dB
- Normalized consistently across similar sound types

---

## 1. Interaction Sound Effects 交互音效

### SFX_Grab_Metal - 抓取金属零件
```
Description: Sound when grabbing a metal rocket part
Duration: ~300ms
Characteristics:
- Initial contact: Subtle metallic "clink"
- Slight scraping as fingers close
- Low-mid frequency emphasis (100-500 Hz)
- No harsh highs

3D Sound: Yes
Attenuation:
- Inner Radius: 100 cm
- Outer Radius: 1000 cm
- Falloff: Natural (inverse square)

Volume: 0.7 (relative to master)
```

### SFX_Release_Metal - 放下零件
```
Description: Sound when releasing a part
Duration: ~400ms
Characteristics:
- Light metallic impact
- Slight ringing/resonance
- Pitch varies by part size (larger = lower)

3D Sound: Yes
Attenuation: Same as Grab

Volume: 0.6

Note: Consider having variants for different surfaces:
- SFX_Release_Metal_Hard (concrete floor)
- SFX_Release_Metal_Soft (on another part)
```

### SFX_Snap_Success - 装配成功（卡扣声）
```
Description: Satisfying "snap" when part locks into place
Duration: ~500ms
Characteristics:
- Clear "click" or "chunk" sound
- Positive, affirming tone
- Slight mechanical echo
- Medium-high frequency peak (1-3 kHz)

3D Sound: Yes
Attenuation:
- Inner Radius: 200 cm (audible from farther)
- Outer Radius: 1500 cm

Volume: 0.8 (important feedback)

Audio Processing:
- Add subtle reverb (small room)
- Slight pitch randomization (±5%) to avoid repetition
```

### SFX_Snap_Fail - 装配失败（错误提示音）
```
Description: Negative feedback for assembly failure
Duration: ~300ms
Characteristics:
- Low "thud" or "clunk"
- Slightly dissonant
- No resonance (dead sound)
- Emphasis on low-mid freq (200-800 Hz)

3D Sound: Yes
Attenuation: Same as Snap_Success

Volume: 0.7

Note: Should be clearly different from success sound
Avoid being too harsh or annoying
```

### SFX_Part_Collision - 零件碰撞
```
Description: Generic metal-on-metal collision
Duration: ~200-400ms (varies by impact force)
Characteristics:
- Metallic clang/clatter
- Varies by velocity (louder = harder impact)
- Dynamic pitch based on part size

3D Sound: Yes
Attenuation:
- Inner Radius: 100 cm
- Outer Radius: 800 cm

Volume: Dynamic (0.3 - 0.9 based on impact force)

Implementation:
- Multiple variants (5-8 files)
- Randomly select on collision
- Pitch shift based on part mass
- Volume based on impact velocity
```

### SFX_Highlight_On - 高亮开启（轻微提示音）
```
Description: Subtle sound when part is highlighted
Duration: ~150ms
Characteristics:
- Soft "ping" or "chime"
- Light, non-intrusive
- High frequency (2-4 kHz)
- Quick attack, medium decay

3D Sound: Yes (emanates from part)
Attenuation:
- Inner Radius: 50 cm
- Outer Radius: 500 cm

Volume: 0.4 (subtle)
```

---

## 2. UI Sound Effects UI 音效

### SFX_Button_Click - 按钮点击
```
Description: Generic button press sound
Duration: ~100ms
Characteristics:
- Clean, crisp click
- Modern UI feel (not mechanical)
- Slight "beep" component
- Mid-high frequency

3D Sound: No (2D UI sound)
Volume: 0.6
```

### SFX_Button_Hover - 按钮悬停
```
Description: Sound when cursor/pointer hovers over button
Duration: ~80ms
Characteristics:
- Very subtle
- Soft "tick" or light tone
- Higher pitch than Click

3D Sound: No
Volume: 0.3 (very quiet)
```

### SFX_Menu_Open - 菜单打开
```
Description: Menu panel opening sound
Duration: ~400ms
Characteristics:
- Smooth "whoosh" or slide
- Slight mechanical element
- Rising pitch contour

3D Sound: No
Volume: 0.5
```

### SFX_Menu_Close - 菜单关闭
```
Description: Menu panel closing sound
Duration: ~300ms
Characteristics:
- Similar to Open but reversed
- Falling pitch contour
- Slightly quieter than Open

3D Sound: No
Volume: 0.4
```

### SFX_Step_Complete - 步骤完成（铃声）
```
Description: Positive feedback for completing a step
Duration: ~800ms
Characteristics:
- Pleasant chime or bell tone
- Uplifting, musical quality
- Clear pitch (e.g., C major chord)
- Sustain and gentle fade

3D Sound: No (general feedback)
Volume: 0.7
```

### SFX_Achievement_Unlock - 成就解锁
```
Description: Special sound for unlocking achievements
Duration: ~1.5s
Characteristics:
- Triumphant fanfare
- Multiple tones (musical chord progression)
- Celebratory feel
- Might include sparkle/shimmer elements

3D Sound: No
Volume: 0.8
```

---

## 3. Ambient Sound Effects 环境音效

### AMB_Workshop_Loop - 车间环境音循环
```
Description: Continuous background ambience for workshop
Duration: 60-90 seconds (seamless loop)
Characteristics:
- Low-frequency rumble (distant machinery)
- Intermittent metal clanks (other workers, distant)
- Ventilation hum (constant)
- No prominent rhythmic patterns
- Subtle, not attention-grabbing

3D Sound: Yes (omnidirectional or from center)
Attenuation:
- Inner Radius: 500 cm
- Outer Radius: 2000 cm (fills space)

Volume: 0.3 (background)
Looping: Yes (ensure seamless loop points)
```

### AMB_Machinery - 机械运转声
```
Description: Closer machinery sound (optional accent)
Duration: 30 seconds loop
Characteristics:
- Rhythmic hum or rotation
- Mid-frequency motor sound
- Consistent pitch

3D Sound: Yes (from specific location, e.g., crane)
Attenuation:
- Inner Radius: 200 cm
- Outer Radius: 1000 cm

Volume: 0.4
Looping: Yes
```

### AMB_Ventilation - 通风系统声
```
Description: Air flow and ventilation
Duration: 45 seconds loop
Characteristics:
- Consistent white noise-like sound
- Low rumble
- Slight pitch variation (wind-like)

3D Sound: Yes (from ceiling/vents)
Attenuation:
- Inner Radius: 300 cm
- Outer Radius: 1500 cm

Volume: 0.25
Looping: Yes
```

---

## 4. Voice Instructions 语音指导

### Voice Recording Guidelines 语音录制指南

**Speaker Requirements 说话者要求**
- Clear, professional voice
- Neutral accent (standard Mandarin for CN, neutral English for EN)
- Friendly but authoritative tone
- Consistent voice across all files

**Recording Specs 录制规格**
- Sample Rate: 48 kHz
- Bit Depth: 24-bit (for processing, export 16-bit)
- Format: Mono WAV
- Distance: 10-15 cm from microphone
- Environment: Sound-treated room (no echo)

**Post-Processing 后期处理**
- Noise reduction (remove background hiss)
- EQ: High-pass filter at 80 Hz
- Compression: Moderate (for consistent volume)
- Normalization: -3 dB peak
- De-essing (remove harsh "s" sounds)

### Voice File Naming 语音文件命名

```
Pattern: {Language}_{Type}_{Number}.wav

Examples:
CN_Welcome.wav
CN_Step_01.wav
CN_Step_02.wav
CN_Error_WrongPart.wav
CN_Success_Complete.wav

EN_Welcome.wav
EN_Step_01.wav
EN_Step_02.wav
EN_Error_WrongPart.wav
EN_Success_Complete.wav
```

### Voice Script Content 语音脚本内容

#### Welcome Messages 欢迎消息

**CN_Welcome.wav (中文)**
```
Script:
"欢迎来到火箭装配VR教育系统。在这里，您将学习火箭的结构和装配流程。请按照指示，一步一步完成火箭的组装。祝您学习愉快！"

Duration: ~10 seconds
Tone: Welcoming, enthusiastic
```

**EN_Welcome.wav (English)**
```
Script:
"Welcome to the Rocket Assembly VR Educational System. Here, you will learn about rocket structure and assembly procedures. Please follow the instructions step by step to complete the rocket assembly. Enjoy your learning experience!"

Duration: ~10 seconds
Tone: Welcoming, enthusiastic
```

#### Step Instructions 步骤说明

**CN_Step_01.wav**
```
Script:
"第一步：放置一级火箭基座。请找到一级火箭筒体，将其放置在中央装配架的底部。确保对齐装配点。"

Duration: ~8 seconds
Tone: Clear, instructional
```

**EN_Step_01.wav**
```
Script:
"Step One: Place the first-stage rocket base. Please find the first-stage rocket body and place it at the bottom of the central assembly frame. Make sure to align it with the assembly point."

Duration: ~8 seconds
Tone: Clear, instructional
```

**CN_Step_02.wav**
```
Script:
"第二步：安装主发动机。主发动机位于您的左侧货架上。拿起发动机，将其安装到一级火箭底部的装配点。您会听到卡扣声，表示安装成功。"

Duration: ~12 seconds
```

**EN_Step_02.wav**
```
Script:
"Step Two: Install the main engine. The main engine is located on the shelf to your left. Pick up the engine and attach it to the assembly point at the bottom of the first stage. You will hear a click sound indicating successful installation."

Duration: ~12 seconds
```

*(Continue for Steps 3-10 with similar structure)*

#### Error Messages 错误消息

**CN_Error_WrongPart.wav**
```
Script:
"抱歉，您选择了错误的零件。请查看当前步骤的提示，选择正确的零件。"

Duration: ~5 seconds
Tone: Gentle correction, not harsh
```

**EN_Error_WrongPart.wav**
```
Script:
"Sorry, you have selected the wrong part. Please check the current step's instructions and select the correct part."

Duration: ~5 seconds
```

#### Success Messages 成功消息

**CN_Success_Complete.wav**
```
Script:
"恭喜！您已成功完成火箭装配。这枚火箭现在已经准备好发射。您对火箭结构和装配流程有了深入的了解。非常棒！"

Duration: ~10 seconds
Tone: Celebratory, proud
```

**EN_Success_Complete.wav**
```
Script:
"Congratulations! You have successfully completed the rocket assembly. This rocket is now ready for launch. You have gained a deep understanding of rocket structure and assembly procedures. Excellent work!"

Duration: ~10 seconds
```

---

## 5. Background Music 背景音乐

### Music Guidelines 音乐指南

**Style 风格**
- Instrumental (no vocals)
- Modern, scientific, inspiring
- Not distracting or overwhelming
- Appropriate for educational context

**Technical 技术**
- Stereo
- Sample Rate: 44.1 kHz or 48 kHz
- Format: WAV (UE can convert to OGG Vorbis for streaming)
- Seamless loops

### MUS_MainMenu_Loop - 主菜单音乐
```
Description: Menu background music
Duration: 2-3 minutes
Characteristics:
- Calm, welcoming
- Ambient electronic or orchestral
- Moderate tempo (90-110 BPM)
- Sense of wonder and possibility
- Slight sci-fi elements

Volume: 0.4-0.5
Looping: Yes
```

### MUS_Assembly_Loop - 装配过程音乐（轻松）
```
Description: Background music during assembly
Duration: 3-5 minutes
Characteristics:
- Calm, focused atmosphere
- Minimal percussion (non-distracting)
- Repetitive but not monotonous
- Helps concentration
- Gentle, flowing

Volume: 0.3 (lower, stays in background)
Looping: Yes
```

### MUS_Countdown - 发射倒计时音乐（紧张）
```
Description: Music for launch countdown (if applicable)
Duration: 60 seconds
Characteristics:
- Building tension
- Increasing tempo
- Rising pitch
- Orchestral or electronic crescendo
- Culminates at liftoff

Volume: 0.6
Looping: No
```

### MUS_Success - 完成庆祝音乐
```
Description: Victory/completion music
Duration: 30-60 seconds
Characteristics:
- Uplifting, triumphant
- Major key
- Strong melody
- Celebratory feel
- Could have fanfare elements

Volume: 0.7
Looping: No
```

---

## Sound Cue Setup 音效 Cue 设置

### What are Sound Cues? 什么是 Sound Cues？

Sound Cues are Unreal's audio node graph system for creating complex sound behaviors.

### Example: SFX_Snap_Success_Cue

```
Sound Cue Graph:

[Random Node]
├── Input 0: SFX_Snap_Success_01.wav (Weight 1.0)
├── Input 1: SFX_Snap_Success_02.wav (Weight 1.0)
└── Input 2: SFX_Snap_Success_03.wav (Weight 1.0)
    ↓
[Modulator Node]
├── Pitch Min: 0.95
├── Pitch Max: 1.05
├── Volume Min: 0.9
└── Volume Max: 1.0
    ↓
[Attenuation Node]
├── Attenuation: Att_Interaction
    ↓
[Output]
```

**Benefits of Sound Cues 好处**
- Variation (avoid repetition)
- Random pitch/volume
- Conditional logic (play different sound based on parameter)
- Delay, reverb, other effects

### Attenuation Presets 衰减预设

Create shared attenuation settings:

**Att_Interaction (互动音效衰减)**
```
Shape: Sphere
Falloff Distance: 1000 cm
Inner Radius: 100 cm
Attenuation Function: Natural (Inverse Square)
Spatial Mode: 3D
```

**Att_UI (UI 音效衰减)**
```
Spatial Mode: 2D (non-spatial)
Volume: No falloff
```

**Att_Ambient (环境音效衰减)**
```
Shape: Sphere
Falloff Distance: 2000 cm
Inner Radius: 500 cm
Attenuation Function: Linear
```

---

## Audio Implementation in Blueprints 蓝图中的音频实现

### Playing Sounds at Location (3D) 在位置播放音效（3D）

```
Blueprint Node: Play Sound at Location

Inputs:
- Sound: SFX_Snap_Success_Cue
- Location: Get Actor Location (of part or socket)
- Volume Multiplier: 1.0
- Pitch Multiplier: 1.0
- Start Time: 0.0
- Attenuation Settings: Att_Interaction

Use Case: Part assembly success sound
```

### Playing 2D Sounds (UI) 播放 2D 音效（UI）

```
Blueprint Node: Play Sound 2D

Inputs:
- Sound: SFX_Button_Click
- Volume Multiplier: 1.0
- Pitch Multiplier: 1.0
- Start Time: 0.0

Use Case: Button clicks, menu sounds
```

### Playing Music (Background) 播放音乐（背景）

```
Component: Audio Component (added to GameMode or Level Blueprint)

Settings:
- Sound: MUS_MainMenu_Loop
- Auto Activate: True
- Is UISound: False
- Volume Multiplier: 0.5
- bLooping: True

Blueprint:
On BeginPlay:
- Set Sound (choose music based on mode)
- Play
```

### Volume Control from Settings 从设置控制音量

```
In BP_AudioManager or GameMode:

Function: SetMasterVolume(Volume: Float)
1. Clamp Volume (0-1)
2. Set Sound Class Volume:
   - Sound Class: Master
   - Volume: Volume
3. Save to Settings

Function: SetSFXVolume(Volume: Float)
1. Clamp Volume (0-1)
2. Set Sound Class Volume:
   - Sound Class: SFX
   - Volume: Volume
3. Save to Settings
```

---

## Sound Class Hierarchy 音效类层级

Create Sound Classes for volume control:

```
SoundClass 'Master'
├── SoundClass 'Music'
├── SoundClass 'SFX'
│   ├── SoundClass 'UI'
│   ├── SoundClass 'Interaction'
│   └── SoundClass 'Ambient'
└── SoundClass 'Voice'
```

**Usage 使用**:
- Assign each Sound Wave to appropriate Sound Class
- Control volume of entire class from Blueprint
- Players can adjust volume by category in settings

---

## Subtitles System 字幕系统

### Subtitle Setup 字幕设置

For voice instructions, enable subtitles:

**In Sound Wave Properties 在音频波形属性中**:
```
Subtitles:
- Enable: True
- Subtitle Text:
  - Time: 0.0
  - Text: "第一步：放置一级火箭基座。"
  - Language: "zh-CN"
```

**Multiple Languages 多语言**:
```
Can have multiple subtitle tracks:
- Chinese: "第一步：放置一级火箭基座。"
- English: "Step One: Place the first-stage rocket base."
```

**Display in UMG 在 UMG 中显示**:
Create WBP_Subtitles widget:
- Text Block for subtitle text
- Background (semi-transparent)
- Position: Bottom center of screen
- Auto-hide after audio ends

Bind to audio component:
- OnAudioFinished → Hide subtitles
- OnSubtitle → Show and update text

---

## Haptic Feedback 触觉反馈

While not audio, haptics complement sound for VR:

### When to Use Haptics 何时使用触觉反馈

1. **Part Grabbed 抓取零件**
   - Intensity: Light
   - Duration: 50ms
   - Pattern: Single pulse

2. **Part Assembled 装配零件**
   - Intensity: Medium
   - Duration: 200ms
   - Pattern: Two pulses (snap feel)

3. **Assembly Failed 装配失败**
   - Intensity: Strong
   - Duration: 100ms
   - Pattern: Single sharp pulse

4. **Button Click 按钮点击**
   - Intensity: Light
   - Duration: 30ms
   - Pattern: Single tiny pulse

### Blueprint Implementation 蓝图实现

```
Blueprint Node: Play Haptic Effect

Inputs:
- Haptic Effect: Choose preset or custom
- Hand: Left, Right, or Both
- Intensity: 0.0 - 1.0
- Play: Execute

Example in InteractiveRocketPart::OnGripped:
1. Play Sound at Location: SFX_Grab_Metal
2. Play Haptic Effect: Intensity 0.3, Hand = Gripping Hand
```

---

## Audio Testing Checklist 音频测试清单

### Functionality 功能
- [ ] All sounds play at correct times
- [ ] 3D sounds have proper attenuation
- [ ] Volume controls work (master, SFX, music, voice)
- [ ] Voice instructions play in correct language
- [ ] Subtitles display correctly
- [ ] No audio pops or clicks
- [ ] Seamless loop points for ambient/music

### Mix and Balance 混音和平衡
- [ ] Voice is always audible over music/SFX
- [ ] Music stays in background (not overwhelming)
- [ ] UI sounds are clear but not annoying
- [ ] Interaction sounds feel satisfying
- [ ] No frequency masking (sounds don't hide each other)

### Performance 性能
- [ ] No audio-related frame drops
- [ ] Streaming music works smoothly
- [ ] Many simultaneous sounds don't cause issues
- [ ] Low memory usage for audio

### VR Experience VR 体验
- [ ] Spatial audio works correctly (sound from right direction)
- [ ] Headphone mix is immersive
- [ ] Volume levels comfortable in VR headset
- [ ] Haptics synchronized with audio

---

## Asset Creation Recommendations 资产创建建议

### Options for Creating Audio 创建音频的选择

1. **Record Custom Audio 录制自定义音频**
   - Pros: Unique, tailored to project
   - Cons: Requires equipment, time, skill

2. **Use Sound Libraries 使用音频库**
   - Freesound.org (free, Creative Commons)
   - Epidemic Sound (subscription)
   - AudioJungle (purchase individual)
   - Pros: High quality, fast
   - Cons: May need license, not unique

3. **Hire Audio Professional 雇用音频专业人员**
   - Voice actor for instructions
   - Sound designer for SFX
   - Composer for music
   - Pros: Best quality
   - Cons: Most expensive

### Placeholder Audio 占位音频

For development, use:
- Engine default sounds
- Simple synthesized tones
- Free library sounds
- Replace with final audio later

**Important 重要**: 
- Always respect licenses and attribution
- Budget for professional audio in final release
- Test early with placeholder audio

---

## Summary 总结

**Audio Asset Counts 音频资产数量**:
- Interaction SFX: ~10-15 files
- UI SFX: ~10 files
- Ambient: ~3-5 loops
- Voice (CN + EN): ~30-40 files (15-20 each language)
- Music: ~4-6 tracks

**Total Estimated Audio Size 预计总音频大小**:
- SFX: ~50 MB
- Voice: ~100 MB (depending on quality/length)
- Music: ~100-150 MB
- Total: ~250-300 MB

**Implementation Priority 实施优先级**:
1. Core interaction sounds (grab, snap, fail)
2. Voice instructions (at least CN or EN to start)
3. Background music (1-2 tracks minimum)
4. UI sounds
5. Ambient sounds
6. Polish and variations

**实施优先级**：
1. 核心交互音效（抓取、装配、失败）
2. 语音说明（至少先中文或英文）
3. 背景音乐（最少 1-2 首）
4. UI 音效
5. 环境音效
6. 润色和变化

---

**Ready to create immersive audio for your VR experience! 准备为您的 VR 体验创建沉浸式音频！🎵**
