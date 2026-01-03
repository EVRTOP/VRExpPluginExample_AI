# Phase 2 - Scene Design Guide
# 火箭装配系统 Phase 2 场景设计指南

## 📋 Overview 概述

This guide provides detailed specifications for creating all game levels/maps for the VR Rocket Assembly Educational System.

本指南提供了为 VR 火箭装配教育系统创建所有游戏关卡/地图的详细规范。

---

## Level Creation Overview 关卡创建概述

### Tools Required 所需工具
- Unreal Engine 5.3 Editor
- Modeling Package (Blender/Maya) for custom meshes
- Texture creation software (Substance/Photoshop)

### Performance Targets 性能目标
- PC VR: 90 FPS minimum
- Meta Quest: 72 FPS minimum
- Draw Calls: < 1000
- Triangles: < 3 million visible

---

## 1. L_MainMenu - 主菜单场景

### Purpose 目的
Entry point for the application, displays main menu UI in 3D space with background decoration.
应用程序入口点，在 3D 空间中显示主菜单 UI 和背景装饰。

### Location 位置
`Content/RocketAssembly/Maps/L_MainMenu.umap`

### Environment Design 环境设计

#### Lighting Setup 照明设置
```
Lighting:
- Sky Light
  - Intensity: 1.0
  - Color: Slight blue tint
  - Cubemap: Sky texture

- Directional Light (Sun)
  - Intensity: 3.0
  - Color: Warm white
  - Angle: 45 degrees from above
  - Shadows: Enabled (Baked for performance)

- Spot Lights (3x)
  - Focused on rocket display
  - Intensity: 2.0
  - Color: White
  - Cone Angle: 30 degrees
```

#### Scene Elements 场景元素

**1. Rocket Display Platform 火箭展示平台**
```
Component: Static Mesh (Platform)
- Size: 5m diameter cylinder
- Height: 0.5m
- Material: Metal/concrete
- Location: Center of scene (0, 0, 0)

Rocket Model:
- Static Mesh: SM_Rocket_Complete (full assembled rocket)
- Scale: 1.5m tall
- Animation: Slow rotation (360° over 60 seconds)
- Location: On platform (0, 0, 50)
- Material: PBR metal, reflective
```

**2. Menu UI Position 菜单 UI 位置**
```
Widget Component (WBP_MainMenu)
- Location: (300, 0, 150) - 3 meters in front of player
- Rotation: Facing player
- Size: 1200x800 pixels
- Distance: 3 meters from spawn point
- Draw at Desired Size: 120x80 cm
```

**3. Background Environment 背景环境**
```
Option A: Space Station Interior
- Walls: Industrial panels
- Windows: Showing stars/Earth
- Ambient: Subtle machinery hum

Option B: Launch Site Observation Deck
- Floor: Concrete platform
- Railings: Safety barriers
- View: Launch pad in distance
- Sky: Day or dusk
```

**4. Video Screen (Optional) 视频屏幕（可选）**
```
Component: Static Mesh (Flat screen)
- Size: 4m x 2.25m (16:9 aspect)
- Location: Behind rocket display
- Material: Video player material
- Video: Looping rocket launch montage
- Volume: 30% (ambient background)
```

**5. Decorative Elements 装饰元素**
```
- Mission patches on walls
- Scale models on shelves
- Technical diagrams
- Ambient lighting strips
- Subtle particle effects (dust in light beams)
```

#### Audio Setup 音频设置
```
Background Music:
- Sound: MUS_MainMenu_Loop
- Volume: 50%
- Looping: Yes
- 3D Sound: No (2D)

Ambient Sound:
- Sound: AMB_SpaceStation or AMB_OutdoorPad
- Volume: 30%
- Looping: Yes
- 3D Sound: Yes (origin at center)
```

#### Player Start 玩家起始位置
```
Player Start Actor:
- Location: (0, 0, 170) - Player eye height ~170cm
- Rotation: Facing rocket and menu (Yaw: 0)
```

#### VR Comfort 舒适度设置
```
- Ground plane visible for orientation
- No moving elements in peripheral vision
- Static environment (no camera movement)
- Clear horizon line
```

---

## 2. L_AssemblyWorkshop - 装配车间主场景

### Purpose 目的
Main gameplay level where rocket assembly takes place.
进行火箭装配的主要游戏关卡。

### Location 位置
`Content/RocketAssembly/Maps/L_AssemblyWorkshop.umap`

### Dimensions 尺寸
- Total Area: 20m x 20m x 12m (height)
- Playable Area: 15m x 15m
- Assembly Area: 10m x 10m (center)

### Environment Design 环境设计

#### 1. Assembly Platform 装配平台

**Central Platform 中央平台**
```
Static Mesh: SM_AssemblyPlatform
- Size: 10m x 10m x 0.2m
- Material: Industrial metal with grid pattern
- Location: Center (0, 0, 0)
- Collision: Block all

Grid Lines:
- Decal: Grid pattern every 50cm
- Color: White, subtle
- Helps with alignment
```

**Assembly Frame 装配架**
```
Static Mesh: SM_AssemblyFrame
- Height: 8m
- Width: 4m
- Material: Steel beams
- Purpose: Visual reference for vertical assembly
- Location: Center of platform
- Contains Socket Indicator actors
```

**Tool Cart 工具车**
```
Static Mesh: SM_ToolCart
- Size: 1m x 0.6m x 1m
- Location: Edge of platform
- Contains: Reference tools (non-interactive)
- Material: Metal with worn texture
```

#### 2. Parts Storage Area 零件存放区

**Shelving Units (4x) 货架组**
```
Static Mesh: SM_IndustrialShelf
- Size: 3m wide x 0.6m deep x 2m tall
- Locations: North, South, East, West of assembly area
- Distance from center: 7m
- Shelves: 3 levels

Each shelf holds:
- Level 1 (bottom): Large parts (engines, tanks)
- Level 2 (middle): Medium parts (pipes, connectors)
- Level 3 (top): Small parts (bolts, sensors)

Part Spawn Points:
- Marked with floor decals
- Labeled with part name signs
- Parts spawn here at game start
```

**Storage Boxes 收纳箱**
```
Static Mesh: SM_StorageBox
- Count: 8-10 boxes
- Size: Varies (0.5m - 1m cubed)
- Location: Around shelves
- Purpose: Visual detail, some interactive
```

**Labeling System 标识系统**
```
3D Text Actors:
- Font Size: Large, readable from 3m
- Content: Part categories in CN + EN
- Examples: 
  - "发动机系统 Engine System"
  - "燃料系统 Fuel System"
- Material: Emissive, easy to read
```

#### 3. Workshop Environment 车间环境

**Walls 墙面**
```
Static Mesh: SM_IndustrialWall
- Height: 12m
- Material: Concrete with metal panels
- Color: Light gray
- Details: 
  - Ventilation vents
  - Cable conduits
  - Warning signs
  - Safety posters
  - Technical diagrams
```

**Floor 地面**
```
Static Mesh: SM_WorkshopFloor
- Material: Concrete with painted lines
- Color: Dark gray
- Markings:
  - Safety zones (yellow striped tape)
  - Pedestrian walkways
  - Equipment zones
  - Assembly area boundary (green line)
- Weathering: Subtle wear and oil stains
```

**Ceiling 天花板**
```
Static Mesh: SM_IndustrialCeiling
- Height: 12m
- Details:
  - Exposed steel beams
  - Ventilation ducts
  - Overhead crane rail (decorative)
  - Suspended lights
  - Hanging cables
```

**Windows (Optional) 窗户（可选）**
```
Static Mesh: SM_IndustrialWindow
- Count: 4-6 windows
- Size: 3m x 2m each
- Height: 6m from floor
- Glass: Semi-transparent
- View: Sky/clouds or adjacent buildings
- Purpose: Natural light simulation
```

#### 4. Decorative Elements 装饰元素

**Overhead Crane 起重机**
```
Static Mesh: SM_OverheadCrane
- Location: Ceiling
- Material: Industrial yellow
- Purpose: Visual detail (non-functional)
- Adds scale and realism
```

**Control Station 控制站**
```
Static Mesh: SM_ControlStation
- Location: Corner of workshop
- Size: 2m x 1m desk
- Details:
  - Monitors (showing rocket schematics)
  - Keyboard and mouse
  - Coffee mug (detail)
  - Papers and clipboards
```

**Safety Equipment 安全设备**
```
Static Meshes:
- SM_FireExtinguisher (x4)
- SM_FirstAidKit (x2)
- SM_SafetySign (various warnings)
- SM_EmergencyLight (flashing effect)

Locations: Near exits and high-traffic areas
```

**Tool Wall 工具墙**
```
Static Mesh: SM_ToolBoard
- Size: 3m x 2m
- Location: Wall near entrance
- Details: Wrenches, hammers, measuring tools
- Material: Organized tool silhouettes
- Purpose: Visual realism
```

**Informational Displays 信息展板**
```
Static Mesh: SM_InfoBoard
- Count: 3-4 boards
- Size: 1.5m x 1m
- Content:
  - Assembly instructions
  - Rocket diagrams
  - Safety procedures
  - Historical rocket photos
- Material: Poster/printed material
```

#### 5. Lighting System 照明系统

**Main Lighting 主要照明**
```
Directional Light (Fake Sun):
- Intensity: 2.0
- Color: Neutral white (slight cool tint)
- Angle: From above and side
- Shadows: Baked (for performance)
- Light Mobility: Static

Sky Light:
- Intensity: 0.8
- Color: Blue tint
- Sky Distance: 150000
- Source: Custom HDRI (indoor workshop)
```

**Overhead Work Lights 顶部工作灯**
```
Point Lights (12-16 units):
- Intensity: 2.0
- Color: Warm white
- Radius: 8m
- Falloff: 2.0
- Source: Suspended industrial lights
- Arrangement: Grid pattern above assembly area
- Light Mobility: Static (baked)
```

**Accent Lighting 重点照明**
```
Spot Lights (4-6 units):
- Intensity: 3.0
- Color: White
- Cone Angle: 45 degrees
- Location: Highlighting assembly frame
- Purpose: Draw attention to work area
- Light Mobility: Stationary (dynamic shadows)
```

**Emergency Lighting 应急照明**
```
Red Warning Lights (2-4 units):
- Intensity: 1.0
- Color: Red
- Blinking: Slow pulse (1 second cycle)
- Location: Near exits
- Blueprint: BP_EmergencyLight with pulsing logic
```

**Emissive Materials 自发光材质**
```
Elements with emissive:
- Monitor screens (blue glow)
- Control panel buttons (green/red)
- Exit signs (green, high intensity)
- Safety signs (yellow borders)
- Floor marking lights (blue LED strips)
```

#### 6. Audio Environment 音频环境

**Ambient Sound 环境音**
```
Sound Cue: AMB_Workshop_Loop
- Volume: 40%
- Looping: Yes
- 3D Sound: Yes
- Attenuation: Inner Radius 10m, Outer 30m
- Location: Center of workshop
- Content: 
  - Distant machinery hum
  - Ventilation system
  - Occasional metal clanks
  - Muffled outdoor sounds
```

**Background Music 背景音乐**
```
Sound Cue: MUS_Assembly_Loop
- Volume: 25%
- Looping: Yes
- 3D Sound: No (2D ambient)
- Style: Calm, focused, scientific
- Starts: When player begins assembly
```

**Interaction Sounds 交互音效**
```
Spatial sounds from:
- Part grabbing (at part location)
- Part assembly (at socket location)
- Footsteps (if using smooth locomotion)
- UI interactions (2D, follow player)
```

#### 7. Interactive Elements 交互元素

**Teleport Markers 瞬移标记**
```
Blueprint: BP_TeleportMarker
- Count: 8-10 markers
- Locations: 
  - Around assembly area (every 2m)
  - Near each shelving unit
  - At control station
- Visual: Blue circular decal
- Activation: VR thumbstick aim
```

**Knowledge Terminal 知识库终端**
```
Blueprint: BP_KnowledgeTerminal
- Location: Near control station
- Static Mesh: Computer terminal
- Interaction: Press button to open WBP_KnowledgeLibrary
- Visual: Glowing screen
```

**Spawn Points for Parts 零件生成点**
```
Actor: Target Point or Scene Component
- Count: Match number of parts (10-20)
- Naming: "SpawnPoint_PartID"
- Location: On shelves and floor
- PartManager uses these for initial spawn
```

**Assembly Sockets 装配点**
```
Component: UAssemblySocket (from C++ class)
- Count: 10-20 (one per part)
- Location: On assembly frame, progressively upward
- Spacing: Appropriate for part sizes
- Visual: BP_SocketIndicator actors at each location
```

#### 8. Post-Processing 后期处理

**Post Process Volume 后期处理体积**
```
Settings:
- Auto Exposure: Disabled (VR best practice)
- Bloom: Intensity 0.5
- Ambient Occlusion: Enabled, Quality Medium
- Motion Blur: Disabled (causes nausea in VR)
- Depth of Field: Disabled
- Vignette: Optional, subtle (0.3) for comfort
- Color Grading: Slight desaturation for realism
```

#### 9. Optimization 优化

**Level of Detail (LOD)**
```
All static meshes should have:
- LOD0: Full detail (< 5m from player)
- LOD1: 50% triangles (5-10m)
- LOD2: 25% triangles (10-20m)
- LOD3: 10% triangles (> 20m)
```

**Occlusion Culling 遮挡剔除**
```
- Use Precomputed Visibility Volumes
- Place around workshop perimeter
- Occludes objects behind walls/shelves
```

**Light Baking 灯光烘焙**
```
- Static lights: Baked shadows
- Lightmap Resolution: 
  - Walls/floor: 128
  - Small objects: 64
  - Build Quality: Production for final
```

**Instance Static Meshes 实例化静态网格**
```
Use ISM for repeated elements:
- Shelving units
- Storage boxes
- Light fixtures
- Floor tiles
```

---

## 3. L_LaunchSite - 发射场景（可选）

### Purpose 目的
Bonus level showing assembled rocket at launch pad with launch simulation.
显示装配好的火箭在发射台上以及发射模拟的额外关卡。

### Location 位置
`Content/RocketAssembly/Maps/L_LaunchSite.umap`

### Environment Design 环境设计

#### Launch Pad 发射台
```
Static Mesh: SM_LaunchPad
- Size: 30m x 30m platform
- Height: 5m
- Material: Reinforced concrete
- Details:
  - Flame trench (center)
  - Water suppression pipes
  - Access towers
  - Umbilical towers
```

#### Assembled Rocket 装配好的火箭
```
Blueprint: BP_RocketComplete
- Uses meshes from assembled parts
- Height: 8-10m (scaled to player view)
- Location: Center of launch pad
- State: Fueled and ready
```

#### Control Center 控制中心
```
Building: SM_ControlBuilding
- Distance: 500m from pad (feels safe)
- Interior: Multiple monitors
- Countdown Display: Large LED timer
- Windows: Facing launch pad
```

#### Environment 环境
```
Landscape:
- Terrain: Flat coastal or desert
- Size: 2km x 2km
- Textures: Sand, rock, sparse vegetation
- Ocean: Optional (for coastal site)

Sky:
- Sky Atmosphere component
- Time: Dusk or day
- Clouds: Dynamic or static
- Horizon: Clear for visibility
```

#### Launch Sequence 发射序列

**Countdown Timeline**
```
Blueprint Timeline (60 seconds total):

T-10s: Final checks
- Audio: "T-minus 10"
- Lights: Warning lights flash

T-5s: Engine ignition sequence
- VFX: Spark ignition
- Audio: Engine rumble building

T-0: Liftoff
- VFX: Massive flame and smoke
- Audio: Thunder roar
- Rocket: Begins to rise
- Camera: Shake effect
- Haptics: Controller vibration

T+5s: Clear tower
- Rocket: 100m altitude
- VFX: Continuous flame trail
- Audio: Doppler shift as rocket ascends

T+30s: Out of sight
- Rocket: Fades into sky
- Audio: Distant rumble
- VFX: Contrail visible
- Success message displayed
```

**VFX Effects 视觉特效**
```
Niagara Systems:
- NS_RocketFlame: Engine flames (large scale)
- NS_LaunchSmoke: Massive smoke clouds
- NS_Shockwave: Ground shockwave on liftoff
- NS_Contrail: White trail in sky
- NS_SteamVent: Pre-launch venting

Parameters:
- Particle count: High (not VR performance critical, short duration)
- Color: Orange/yellow flames, white/gray smoke
- Size: Scale appropriate to rocket
```

**Audio for Launch 发射音效**
```
Sound Cues:
- SFX_Countdown: Voice countdown
- SFX_EngineIgnition: Spark and rumble
- SFX_Liftoff: Massive roar (loud, immersive)
- SFX_SonicBoom: Optional shockwave sound
- MUS_Countdown: Tense music building

Mixing:
- Voice: Clear, 100%
- SFX: Very loud (80%), spatial
- Music: 40%, background
```

---

## 4. L_TestLevel - 测试关卡

### Purpose 目的
Minimal level for quick testing of mechanics without full environment.
用于快速测试机制的最小关卡，无需完整环境。

### Location 位置
`Content/RocketAssembly/Maps/L_TestLevel.umap`

### Design 设计

```
Minimal Setup:
- Flat ground plane (10m x 10m)
- Basic lighting (one directional light)
- Player start
- 1-3 test parts spawned
- 1-2 assembly sockets
- Debug text overlays

Purpose:
- Test C++ classes
- Test Blueprint logic
- Test part interactions
- Test assembly validation
- Performance profiling

No decorative elements
Fast iteration time
```

---

## Implementation Checklist 实施清单

### L_MainMenu
- [ ] Create new level
- [ ] Add lighting (Sky + Directional)
- [ ] Place rocket display
- [ ] Position menu widget
- [ ] Add background elements
- [ ] Setup audio (music + ambient)
- [ ] Add player start
- [ ] Test in VR

### L_AssemblyWorkshop
- [ ] Create level and block out dimensions
- [ ] Build assembly platform
- [ ] Place assembly frame with sockets
- [ ] Create shelving units
- [ ] Position part spawn points
- [ ] Add walls, floor, ceiling
- [ ] Place decorative elements
- [ ] Setup lighting (main + accents)
- [ ] Add ambient audio
- [ ] Place teleport markers
- [ ] Optimize (LODs, lightmaps)
- [ ] Test full assembly workflow
- [ ] Performance test (90 FPS in VR)

### L_LaunchSite (Optional)
- [ ] Create level
- [ ] Build launch pad
- [ ] Place assembled rocket
- [ ] Create control center
- [ ] Setup landscape
- [ ] Implement launch sequence
- [ ] Add VFX (flames, smoke)
- [ ] Add audio (countdown, launch)
- [ ] Test camera shake and haptics
- [ ] Test complete experience

### L_TestLevel
- [ ] Create minimal level
- [ ] Add basic lighting
- [ ] Spawn test parts
- [ ] Quick iteration setup
- [ ] Debug visualization

---

## Performance Optimization Tips 性能优化技巧

1. **Lighting**
   - Use static lights where possible
   - Bake lighting for static objects
   - Limit dynamic lights (< 3 visible)

2. **Geometry**
   - Use LODs for all meshes
   - Combine small meshes into single mesh
   - Reduce overlapping geometry

3. **Materials**
   - Use material instances (not base materials)
   - Limit shader complexity
   - Use simpler materials for distant objects

4. **VFX**
   - Limit particle count in VR
   - Use GPU particles (Niagara)
   - Cull particles behind player

5. **Audio**
   - Use sound attenuation
   - Limit concurrent sounds (< 32)
   - Compress audio appropriately

6. **Culling**
   - Use occlusion culling volumes
   - Set appropriate cull distances
   - Disable tick on distant actors

---

## Testing Guidelines 测试指南

### Visual Quality
- [ ] Lighting looks realistic
- [ ] No light leaks
- [ ] Materials look correct
- [ ] Colors are appropriate
- [ ] No z-fighting

### VR Comfort
- [ ] Stable framerate (90 FPS+)
- [ ] No motion sickness triggers
- [ ] Clear sight lines
- [ ] Proper scale (1 unit = 1 cm)
- [ ] Ground plane visible

### Functionality
- [ ] Player can reach all areas
- [ ] Parts spawn correctly
- [ ] Assembly sockets work
- [ ] UI is readable
- [ ] Audio is balanced

### Performance
- [ ] FPS meets target
- [ ] Draw calls reasonable
- [ ] Memory usage acceptable
- [ ] No hitches or stutters

---

**Creation Order Recommendation**:
1. L_TestLevel (for basic testing)
2. L_AssemblyWorkshop (main gameplay)
3. L_MainMenu (entry point)
4. L_LaunchSite (polish/bonus)

**创建顺序建议**：
1. L_TestLevel（基础测试）
2. L_AssemblyWorkshop（主要游戏）
3. L_MainMenu（入口点）
4. L_LaunchSite（润色/额外内容）
