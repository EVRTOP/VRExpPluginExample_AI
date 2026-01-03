# Phase 3: 性能优化指南
# Phase 3: Performance Optimization Guide

## 目录 Table of Contents
1. Performance Targets 性能目标
2. Profiling Tools 性能分析工具
3. Rendering Optimization 渲染优化
4. Blueprint Optimization 蓝图优化
5. Memory Management 内存管理
6. VR-Specific Optimization VR专项优化
7. Asset Optimization 资产优化
8. Platform-Specific Tips 平台特定技巧

---

## 1. Performance Targets 性能目标

### PC VR (High-End) PC VR(高端)
- **Target FPS**: 90 FPS (11.1ms per frame)
- **Resolution**: 2160x2400 per eye
- **Draw Calls**: < 2000
- **Poly Count**: < 1.5M visible triangles
- **Memory**: < 4GB VRAM, < 8GB RAM

### Meta Quest (Standalone) Meta Quest(独立设备)
- **Target FPS**: 72 FPS (13.9ms per frame)
- **Resolution**: 1832x1920 per eye
- **Draw Calls**: < 500
- **Poly Count**: < 500K visible triangles
- **Memory**: < 2GB RAM (strict)

**Critical**: VR requires consistent frame rate. Frame drops cause nausea.

---

## 2. Profiling Tools 性能分析工具

### UE5 Built-in Profilers UE5内置分析器

#### Console Commands 控制台命令

```
Performance Stats:
- stat fps: Show frame rate
- stat unit: Frame time breakdown (Game, Draw, GPU)
- stat engine: Engine stats
- stat scenerendering: Rendering stats
- stat rhi: Hardware interface stats
- stat memory: Memory usage
- stat streaming: Streaming stats

GPU Profiling:
- profilegpu: Detailed GPU profiling
- r.ScreenPercentage: Adjust resolution for testing

Specific Systems:
- stat particles: Niagara/Cascade particle counts
- stat audio: Audio stats
- stat physics: Physics simulation time
- stat blueprint: Blueprint execution time
```

#### Unreal Insights 虚幻洞察

```
How to use:
1. Launch with -trace=cpu,gpu,frame
2. Open Unreal Insights tool
3. Analyze frame capture
4. Identify bottlenecks
```

#### GPU Visualizer GPU可视化器

```
~ key (console) → GPU Visualizer
Shows:
- Each render pass time
- Expensive materials
- Lighting cost
- Post-processing cost
```

---

## 3. Rendering Optimization 渲染优化

### LOD (Level of Detail) 细节层次

**For rocket parts**:

```
Mesh LOD Settings:
- LOD 0 (0-3m): Full detail (2000-5000 tris)
- LOD 1 (3-8m): Reduced (1000-2000 tris)
- LOD 2 (8-20m): Low (500-1000 tris)
- LOD 3 (>20m): Very low (200-500 tris)

Auto-generate LODs:
1. Select mesh in Content Browser
2. LOD Settings → Generate LODs
3. Reduction %: 50%, 25%, 10%
4. Test in editor viewport
```

### Material Optimization 材质优化

#### Shader Complexity 着色器复杂度

```
View → Optimization Viewmodes → Shader Complexity

Color Guide:
- Green: Good (< 100 instructions)
- Yellow: Moderate (100-200)
- Red: Expensive (> 200)
- White: Very expensive (> 400)

Target: Keep all materials green/yellow for VR
```

#### Material Tips 材质技巧

```
1. Reduce texture samples:
   - Combine textures (RGB channels)
   - Use atlases instead of multiple textures

2. Simplify shader nodes:
   - Remove unnecessary math
   - Use simpler blend modes
   - Avoid expensive nodes: Noise, Scene Color

3. Use material instances:
   - Master material with parameters
   - Instances are cheaper to switch

4. Optimize for mobile (Quest):
   - Use Mobile material domain
   - Fully rough (no reflections)
   - Limit to 3 texture samples
```

### Lighting Optimization 光照优化

#### Static vs Dynamic 静态vs动态

```
L_AssemblyWorkshop lighting:
- 90% Static lights: Baked lightmaps
- 10% Stationary: Key lights (dynamic shadows)
- Avoid Movable lights (expensive)

Lightmap Resolution:
- Large surfaces: 64-128
- Medium objects: 32-64
- Small details: 16-32
```

#### Light Count 光源数量

```
PC VR: < 10 dynamic lights in view
Quest: < 4 dynamic lights (strict)

Use light functions sparingly (expensive)
```

### Shadows 阴影

```
PC VR:
- Cascaded Shadow Maps: 4 cascades
- Shadow resolution: 2048
- Shadow distance: 20m

Quest:
- Modulated shadows (cheaper)
- Shadow resolution: 512-1024
- Shadow distance: 10m
- Consider baked shadows only
```

---

## 4. Blueprint Optimization 蓝图优化

### Event Tick Optimization Tick事件优化

**Problem**: Tick runs every frame (expensive)

**Solutions**:

#### 1. Use Timers Instead of Tick
```
Bad:
  Event Tick
    ├─ Do Something
    └─ (Runs 90 times/second)

Good:
  Event BeginPlay
    └─ Set Timer by Function Name
        ├─ Function: DoSomething
        ├─ Time: 0.1 (10 times/second)
        └─ Looping: true
```

#### 2. Disable Tick When Not Needed
```
When idle:
  Set Actor Tick Enabled: false

When active:
  Set Actor Tick Enabled: true
```

#### 3. Skip Frames
```
Event Tick
  ├─ Increment Counter
  ├─ Branch: Counter % 5 == 0?
  │   └─ True: Do Expensive Calculation
  └─ (Only runs every 5 frames)
```

### Reduce Blueprint Complexity 减少蓝图复杂度

```
1. Move hot paths to C++:
   - Validation logic
   - Physics calculations
   - Frequently called functions

2. Cache references:
   Bad: Get Player Controller every tick
   Good: Get once in BeginPlay, store in variable

3. Use const for read-only:
   - Mark parameters as const
   - Blueprint compiler can optimize

4. Avoid nested loops:
   - O(n²) algorithms are slow
   - Use TMap/TSet for lookups instead
```

### Blueprint Nativization (Deprecated in UE5)

Note: Blueprint nativization removed in UE5.  
Alternative: Convert critical Blueprints to C++

---

## 5. Memory Management 内存管理

### Texture Streaming 纹理流送

```
Texture Settings:
- Never Stream: UI textures (small)
- Stream: World textures (large)
- Mip Gen Settings: FromTextureGroup

Texture Groups:
- World: 2048x2048 max, mipmaps
- Character: 1024x1024
- UI: 512x512, no mipmaps
- Effects: 512x512, compressed

Compression:
- PC: BC7 (high quality)
- Mobile: ASTC (hardware compressed)
```

### Memory Pools 内存池

```
Project Settings → Engine → Memory:
- Streaming Pool Size: 1000 MB (PC), 256 MB (Quest)
- Max Shader Cache Size: 10 MB
- Texture Streaming Pool Size: Auto
```

### Garbage Collection 垃圾回收

```
Avoid creating temporary objects in Tick:
Bad:
  Event Tick
    └─ Spawn Actor (leaks memory)

Good:
  Pool objects, reuse them

Force GC (for testing only):
  Console: obj gc
```

---

## 6. VR-Specific Optimization VR专项优化

### Forward Rendering 前向渲染

```
Project Settings → Engine → Rendering:
- Forward Shading: Enable (better for VR)

Benefits:
- Better performance with many lights
- Lower VRAM usage
- MSAA support (better anti-aliasing for VR)
```

### Instanced Stereo Rendering 实例化立体渲染

```
Automatically enabled in VR:
- Renders both eyes in single pass
- Significant performance gain

Ensure materials support instancing:
- Use 'Instanced Stereo' in material settings
```

### Dynamic Resolution 动态分辨率

```
Console: r.DynamicRes.OperationMode 1

Settings:
- Min Screen Percentage: 50%
- Max Screen Percentage: 100%
- Target FPS: 90 (PC), 72 (Quest)

Behavior:
- If FPS drops: Lower resolution
- If FPS high: Increase resolution
- Mostly imperceptible to user
```

### Fixed Foveated Rendering (Quest) 固定注视点渲染

```
Quest-specific:
- Center of view: Full resolution
- Periphery: Lower resolution
- Matches human eye perception

Enable: Oculus settings in project
```

### Occlusion Culling 遮挡剔除

```
Place Cull Distance Volumes:
- Cull small objects at distance
- Example: Nuts/bolts invisible > 10m

Precomputed Visibility:
- For static environments
- Reduces draw calls significantly
```

---

## 7. Asset Optimization 资产优化

### 3D Model Guidelines 3D模型指南

```
Polygon Budget per Part:
- Simple parts (bolts): < 500 tris
- Medium parts (panels): 1000-2000 tris
- Complex parts (engines): 3000-5000 tris
- Total rocket assembled: < 100,000 tris

Optimization:
- Remove unseen faces (inside surfaces)
- Use normal maps instead of geometry
- Merge static meshes when possible
- Use instancing for repeated elements
```

### Audio Optimization 音频优化

```
Compression:
- SFX: ADPCM (low latency)
- Voice: Vorbis Quality 60
- Music: Opus/Vorbis, stream from disk

Loading:
- Preload: Short SFX, voice lines
- Stream: Music, ambient loops
- On-demand: Rarely used sounds

Concurrency:
- Limit simultaneous sounds: 16 (PC), 8 (Quest)
- Use sound concurrency settings
```

### Particle System Optimization 粒子系统优化

```
Niagara Budget:
- PC: < 15,000 active particles
- Quest: < 3,000 active particles

Techniques:
1. LOD: Reduce spawn rate at distance
2. Cull: Don't render off-screen particles
3. GPU simulation: Offload to GPU (PC only)
4. Sprite vs Mesh: Sprites cheaper (use for smoke, sparks)
5. Lifespan: Shorter lifespan = fewer particles
```

---

## 8. Platform-Specific Tips 平台特定技巧

### PC VR Optimization PC VR优化

```
Advantages:
- More GPU power
- More memory
- Can use complex effects

Tips:
- Utilize GPU effectively (high-quality materials)
- Use post-processing (careful with performance)
- Dynamic shadows acceptable
- High-res textures OK
```

### Quest Optimization Quest优化

```
Limitations:
- Mobile GPU (Adreno 650)
- Limited RAM (6GB total, ~2GB for app)
- Battery-powered (thermal throttling)

Critical Optimizations:
1. Aggressive LOD: Switch to low-poly early
2. Simple materials: Mobile shaders only
3. Baked lighting: Avoid dynamic lights
4. Low-res textures: 1024x1024 max for most
5. Minimal particles: < 1000 active
6. Static geometry: Pre-combine meshes
7. Occlusion culling: Essential
8. Forward rendering: Always

Test on device early and often!
```

### Profiling on Quest

```
Use Oculus Developer Hub:
- Real-time performance monitor
- GPU/CPU usage
- Thermal state
- Frame timing

Or use ADB:
  adb logcat | grep -i "VrApi"
```

---

## 9. Optimization Checklist 优化检查清单

### Pre-Launch Checklist 发布前检查

#### Rendering 渲染
- [ ] All meshes have LODs
- [ ] Materials shader complexity < 200 instructions
- [ ] Lighting mostly static/baked
- [ ] Shadow distance appropriate (10-20m)
- [ ] Culling volumes placed
- [ ] Draw calls < target (2000 PC, 500 Quest)

#### Blueprints 蓝图
- [ ] Minimal use of Event Tick
- [ ] References cached (not fetched every frame)
- [ ] Heavy logic moved to C++
- [ ] No nested loops in hot paths

#### Assets 资产
- [ ] Textures compressed and streamed
- [ ] Audio compressed and loading optimized
- [ ] Particles within budget
- [ ] Unused assets removed from build

#### VR Specific VR专项
- [ ] Forward rendering enabled
- [ ] Instanced stereo working
- [ ] Dynamic resolution enabled
- [ ] Consistent 90 FPS (PC) / 72 FPS (Quest)
- [ ] No frame spikes > 2ms

#### Memory 内存
- [ ] Texture streaming working
- [ ] No memory leaks (long play test)
- [ ] Build size reasonable (< 2GB PC, < 1GB Quest)

---

## 10. Performance Testing 性能测试

### Test Scenarios 测试场景

```
1. Idle Scene:
   - Just environment, no activity
   - Should be very high FPS (headroom)

2. Full Assembly:
   - All parts visible, moving
   - Target FPS maintained

3. Launch Sequence:
   - Heavy VFX active
   - Target FPS maintained (or close)

4. Long Play Session:
   - 30-60 minutes continuous play
   - Check for memory leaks (FPS degradation)
   - Check for thermal throttling (Quest)
```

### Automated Testing 自动化测试

```
Create BP_PerformanceTestController:
  - Automated camera fly-through
  - Log FPS every second
  - Log memory usage
  - Generate performance report

Run overnight, analyze logs
```

---

## Deliverables 交付清单

**Optimization Work**:
- [ ] All meshes LOD'd
- [ ] All materials optimized
- [ ] Lighting baked
- [ ] Blueprints optimized (Tick minimized)
- [ ] Assets compressed

**Documentation**:
- [ ] Performance test results
- [ ] Profiling screenshots
- [ ] Platform-specific settings documented

**Testing**:
- [ ] 90 FPS on PC VR consistently
- [ ] 72 FPS on Quest consistently
- [ ] No memory leaks in 1-hour session
- [ ] Thermal management on Quest acceptable

---

**Status**: Complete Optimization Guide ✅  
**Version**: 1.0.0  
**Date**: 2026-01-03

**Phase 3 Performance Optimization Complete! ⚡🚀**
