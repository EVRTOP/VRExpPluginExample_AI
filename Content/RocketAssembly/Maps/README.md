# 关卡地图 Maps/Levels

此目录包含火箭装配系统的关卡地图。
This directory contains level maps for the rocket assembly system.

## 关卡列表 Level List

### L_AssemblyWorkshop
**火箭装配车间 - Rocket Assembly Workshop**

主要装配场景，玩家在此进行火箭装配。

**环境设置:**
- 大型工业车间
- 高天花板(至少20米)
- 装配平台(带网格地面)
- 零件存放区
- 工具台
- 照明系统

**必需Actor:**
- BP_RocketPartManager - 零件管理器
- BP_AssemblyTutorialManager - 教学管理器
- BP_VRPawn - VR玩家
- 装配点(多个 AssemblySocket)

**照明:**
- Directional Light (主光源)
- Point Lights (补光)
- Spot Lights (聚光灯，突出装配区域)
- Sky Light (环境光)

**后处理:**
- Exposure: Auto
- Bloom: 适度
- 色调映射: ACES

### L_MainMenu
**主菜单场景 - Main Menu Level**

简化的展示场景，用于主菜单UI。

**内容:**
- 背景火箭模型
- 旋转展示台
- 简单照明
- UI界面显示

### L_LaunchSite (可选)
**发射场景 - Launch Site**

完成装配后的发射观看场景。

**内容:**
- 发射台
- 完整的火箭
- 发射动画
- 观众视角
- 特效(烟雾、火焰)

### L_Tutorial
**教学关卡 - Tutorial Level**

简化的教学关卡，用于新手引导。

**内容:**
- 较少的零件(3-5个)
- 明确的装配指引
- 详细的提示
- 无时间限制

### L_Test
**测试关卡 - Test Level**

开发测试用关卡。

**用途:**
- 快速测试新功能
- 性能测试
- 调试工具

## 关卡设置 Level Settings

### World Settings
- Kill Z: -10000
- Enable World Composition: No
- Default Game Mode: BP_RocketAssemblyGameMode

### VR Settings
- Start in VR: Yes
- VR Spectator Screen Mode: SingleEye
- Motion Controller Source: OpenXR

### 物理设置 Physics Settings
- Gravity: -980 cm/s²
- Substeps: 1
- Max Substeps: 6

## 优化建议 Optimization Tips

### 光照
- 使用烘焙光照
- 限制动态光源数量
- 使用光照贴图

### 几何体
- 使用LOD
- 合并静态网格
- 剔除不可见物体

### 材质
- 使用材质实例
- 优化材质复杂度
- 合理使用透明材质

### VR优化
- 目标帧率: 90 FPS
- Forward Rendering
- Instanced Stereo
- 固定Foveated Rendering (Quest)

## 创建新关卡 Creating New Levels

1. File -> New Level
2. 选择模板(Empty Level 或 Default)
3. 设置 World Settings
4. 添加必需Actor
5. 配置照明
6. 添加装配点
7. 测试功能
8. 优化性能

## 关卡流程 Level Workflow

### 开始游戏
1. L_MainMenu 加载
2. 玩家选择模式
3. 流式加载 L_AssemblyWorkshop
4. 初始化管理器
5. 开始装配

### 完成装配
1. 验证装配正确性
2. 显示成绩报告
3. 可选: 流式加载 L_LaunchSite
4. 播放发射动画

## 待办事项 TODO

- [ ] 创建 L_AssemblyWorkshop 基础场景
- [ ] 添加环境装饰
- [ ] 设置照明系统
- [ ] 放置装配点
- [ ] 创建 L_MainMenu
- [ ] 创建 L_Tutorial
- [ ] 优化性能
- [ ] 添加音效和音乐
