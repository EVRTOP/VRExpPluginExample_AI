# 火箭装配 VR 科普教育系统 - Rocket Assembly VR Educational System

## 项目概述 Project Overview

基于 Unreal Engine 5.3 和 VRExpansionPlugin 的沉浸式 VR 火箭装配教育系统。
An immersive VR rocket assembly educational system built on Unreal Engine 5.3 and VRExpansionPlugin.

## 核心功能 Core Features

### 1. 火箭零件系统 Rocket Parts System
- 20+ 种火箭零件类型
- 详细的零件数据结构
- 科普知识内容集成

### 2. VR 交互系统 VR Interaction System
- 基于 VRExpansionPlugin 的抓取系统
- 精确装配对齐
- 磁吸效果
- 实时反馈

### 3. 装配验证系统 Assembly Validation System
- 位置和角度对齐检测
- 装配顺序验证
- 依赖关系检查
- 碰撞检测
- 精度评分

### 4. 教学引导系统 Tutorial System
- 分步教学流程
- AR 可视化指引
- 多语言支持(中英文)

## 技术架构 Technical Architecture

### C++ 核心类 Core C++ Classes

#### RocketPartData.h
数据结构定义：
- `ERocketPartType` - 零件类型枚举
- `EAssemblyState` - 装配状态枚举
- `FRocketPartData` - 零件数据结构
- `FAssemblyStepData` - 装配步骤数据
- `FRocketKnowledgeData` - 科普知识数据

#### InteractiveRocketPart
可交互火箭零件 Actor，继承自 `AGrippableStaticMeshActor`
- 高亮显示系统
- 装配状态管理
- 信息提示功能
- 锁定/解锁机制
- 事件系统

#### AssemblySocket
装配点组件，继承自 `USceneComponent`
- 装配点定义
- 对齐检测(位置、角度)
- 磁吸效果
- 可视化指示器
- 零件检测

#### AssemblyValidator
装配验证器 UObject
- 装配验证逻辑
- 碰撞检测
- 顺序验证
- 依赖检查
- 评分系统

## 目录结构 Directory Structure

```
Content/RocketAssembly/
├── Blueprints/          # 蓝图资产
│   ├── Parts/           # 零件蓝图
│   └── Sockets/         # 装配点蓝图
├── UI/                  # UI 界面
├── Data/                # 数据表
├── Materials/           # 材质
├── Meshes/              # 3D 模型
│   ├── Engines/         # 发动机
│   ├── FuelTanks/       # 燃料舱
│   ├── Fairings/        # 整流罩
│   ├── ControlSystems/  # 控制系统
│   └── Structures/      # 结构件
├── Textures/            # 纹理
├── Audio/               # 音频
│   ├── SFX/            # 音效
│   ├── Voice/          # 语音
│   │   ├── CN/         # 中文
│   │   └── EN/         # 英文
│   └── Music/          # 音乐
├── VFX/                # 视觉特效
└── Maps/               # 关卡地图

Source/VRExpPluginExample/RocketAssembly/
├── RocketPartData.h              # 数据结构
├── InteractiveRocketPart.h/cpp   # 可交互零件
├── AssemblySocket.h/cpp          # 装配点
└── AssemblyValidator.h/cpp       # 验证器
```

## 零件类型 Part Types

### 主发动机系统 Main Engine System
- MainEngineNozzle - 主发动机喷嘴
- TurboPump - 涡轮泵
- CombustionChamber - 燃烧室

### 燃料系统 Fuel System
- LOXTank - 液氧舱
- FuelTank - 燃料舱(液氢/煤油)
- FuelPipeline - 燃料管路

### 箭体结构 Rocket Body Structure
- FirstStageBody - 一级火箭筒体
- SecondStageBody - 二级火箭筒体
- InterstageSection - 级间段

### 控制系统 Control System
- AttitudeControlEngine - 姿态控制发动机
- GyroscopeUnit - 陀螺仪组件
- ControlComputer - 控制计算机

### 整流罩与载荷 Fairing & Payload
- FairingLeft - 整流罩(左)
- FairingRight - 整流罩(右)
- SatelliteAdapter - 卫星适配器
- PayloadSimulator - 模拟载荷

### 连接与分离系统 Connection & Separation
- StageSeparator - 级间分离装置
- ExplosiveBolt - 爆炸螺栓
- SupportStructure - 支撑结构

## 装配流程 Assembly Process

1. **基础安装** - 放置一级火箭基座
2. **发动机装配** - 安装主发动机和涡轮泵
3. **燃料系统** - 安装燃料舱和管路
4. **箭体组装** - 安装级间段和二级火箭
5. **控制系统** - 安装控制组件
6. **整流罩安装** - 安装整流罩两半
7. **最终检查** - 验证所有零件装配正确

## 使用方法 Usage

### 开发者 For Developers

1. 打开项目：使用 UE 5.3 打开 `VRExpPluginExample.uproject`
2. 编译代码：右键项目文件选择 "Generate Visual Studio project files"
3. 在 Visual Studio 中编译解决方案
4. 运行编辑器

### 创建新零件 Creating New Parts

1. 继承 `AInteractiveRocketPart` 创建蓝图类
2. 在 Data Tables 中添加零件数据
3. 设置零件属性：
   - 零件ID、名称、类型
   - 3D模型和材质
   - 装配顺序和依赖
   - 科普知识内容

### 创建装配点 Creating Assembly Sockets

1. 在目标 Actor 上添加 `UAssemblySocket` 组件
2. 配置装配点：
   - 设置 SocketID
   - 指定接受的零件类型/ID
   - 调整对齐容差
   - 配置磁吸参数

## 配置参数 Configuration Parameters

### 装配对齐 Assembly Alignment
- 位置容差：默认 5 cm
- 角度容差：默认 10 度
- 磁吸距离：默认 30 cm
- 磁吸强度：0.0 - 1.0

### 评分系统 Scoring System
- 装配正确率：40%
- 装配顺序：30%
- 完成时间：15%
- 失误次数：15%

## 本地化 Localization

系统支持中英文双语：
- 零件名称和描述
- 教学提示文本
- 错误消息
- 科普知识内容
- UI 界面

## 性能优化 Performance Optimization

- LOD 系统用于3D模型
- 材质实例化
- 对象池管理
- 异步资源加载
- VR 目标：90 FPS (PC), 72 FPS (Quest)

## 支持平台 Supported Platforms

- Windows PC VR
- Meta Quest (Android)
- HoloLens
- Linux VR

## 依赖插件 Required Plugins

- VRExpansionPlugin
- OpenXR
- OpenXRExpansionPlugin
- AdvancedSessions

## 许可证 License

Copyright VRExpPluginExample. All Rights Reserved.

## 联系方式 Contact

GitHub: https://github.com/EVRTOP/VRExpPluginExample_AI

---

## 开发进度 Development Progress

- ✅ Phase 1: 核心基础架构 (完成)
- 🔄 Phase 2: 装配系统
- ⏳ Phase 3: 教学引导系统
- ⏳ Phase 4: UI 系统
- ⏳ Phase 5: 数据与内容
- ⏳ Phase 6: 评估与完善

最后更新: 2026-01-03
