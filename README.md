# VRExpPluginExample with Rocket Assembly Educational System

## 项目介绍 Project Introduction

这是一个基于 Unreal Engine 5.3 和 VRExpansionPlugin 的 VR 项目模板，现已集成完整的**火箭装配 VR 科普教育系统**。

This is a VR project template based on Unreal Engine 5.3 and VRExpansionPlugin, now featuring a complete **Rocket Assembly VR Educational System**.

---

## 🚀 火箭装配系统 Rocket Assembly System

### 特性 Features

- ✅ **完整的 C++ 核心系统** - 4个核心类，约1,800行代码
- ✅ **VR 交互系统** - 基于 VRExpansionPlugin 的精确抓取和装配
- ✅ **装配验证系统** - 位置、角度、顺序、依赖关系验证
- ✅ **教育内容集成** - 科普知识、中英文双语支持
- ✅ **完整文档** - 12个文档文件，约33,000字
- ✅ **数据模板** - CSV模板和30条示例数据

### 核心系统 Core Systems

1. **InteractiveRocketPart** - 可交互火箭零件
   - 20+ 种零件类型
   - 多状态高亮系统
   - 装配状态管理
   
2. **AssemblySocket** - 装配点组件
   - 对齐检测和验证
   - 磁吸效果
   - 可视化指示器

3. **AssemblyValidator** - 装配验证器
   - 精度评分 (0-100)
   - 碰撞检测
   - 依赖检查

4. **RocketPartData** - 数据结构
   - 零件数据
   - 装配步骤
   - 科普知识

### 快速开始 Quick Start

#### 1. 查看文档 Read Documentation
```
📖 PROJECT_SUMMARY.md      - 项目总览
📖 DEVELOPER_GUIDE.md      - 开发者指南
📖 IMPLEMENTATION_GUIDE.md - 实施指南
```

#### 2. 浏览源代码 Explore Source Code
```
Source/VRExpPluginExample/RocketAssembly/
├── RocketPartData.h           - 数据结构
├── InteractiveRocketPart.*    - 可交互零件
├── AssemblySocket.*           - 装配点
└── AssemblyValidator.*        - 验证器
```

#### 3. 使用数据模板 Use Data Templates
```
Content/RocketAssembly/Data/
├── DT_RocketParts_Template.csv       - 零件数据
├── DT_AssemblySteps_Template.csv     - 装配步骤
└── DT_RocketKnowledge_Template.csv   - 科普知识
```

### 开发进度 Development Progress

- ✅ **Phase 1**: 核心基础架构 (完成)
- ⏳ **Phase 2**: Blueprint 实现
- ⏳ **Phase 3**: 教学引导系统
- ⏳ **Phase 4**: UI 系统
- ⏳ **Phase 5**: 数据与内容
- ⏳ **Phase 6**: 评估与完善

### 技术栈 Tech Stack

- **引擎**: Unreal Engine 5.3
- **VR**: VRExpansionPlugin + OpenXR
- **语言**: C++ + Blueprint
- **平台**: Win64, Linux, Android, HoloLens

---

## 🛠️ 项目设置 Project Setup

### 环境要求 Requirements

* Unreal Engine 5.3
* Visual Studio 2022
* VR 设备 (可选) - Oculus, Vive, Index, Quest

### 编译步骤 Build Steps

1. **生成项目文件 Generate Project Files**
   * 右键 `VRExpPluginExample.uproject`
   * 选择 "Generate Visual Studio project files"

2. **编译项目 Build Project**
   * 打开 `VRExpPluginExample.sln`
   * 选择 "Development Editor" 配置
   * Build (F7)

3. **运行编辑器 Run Editor**
   * 按 F5 或直接双击 `.uproject` 文件

### 切换引擎版本 Switch Engine Version

* 右键 `VRExpPluginExample.uproject`
* 选择 "Switch Unreal Engine version..."
* 选择兼容版本 (推荐 5.3)
* 重新生成项目文件

---

## 📚 文档资源 Documentation

### 主要文档 Main Documents
- **PROJECT_SUMMARY.md** - 完整项目总结
- **DEVELOPER_GUIDE.md** - API参考和开发流程
- **IMPLEMENTATION_GUIDE.md** - 10步实施指南

### 目录文档 Directory Docs
- `Content/RocketAssembly/README.md` - 系统概述
- `Content/RocketAssembly/*/README.md` - 各模块详细文档

---

## 🎮 支持平台 Supported Platforms

- ✅ Windows PC VR
- ✅ Meta Quest (Android)
- ✅ HoloLens
- ✅ Linux VR

---

## 🔌 插件 Plugins

### 必需插件 Required
- **VRExpansionPlugin** - VR 交互核心
- **OpenXR** - VR 标准接口
- **OpenXRExpansionPlugin** - OpenXR 扩展

### 可选插件 Optional
- **AdvancedSessions** - 多人联机支持

---

## 📖 相关链接 Links

- [Unreal Engine Documentation](https://docs.unrealengine.com/)
- [VRExpansionPlugin](https://vreue4.com/)
- [OpenXR](https://www.khronos.org/openxr/)

---

## 📄 许可证 License

Copyright VRExpPluginExample. All Rights Reserved.

---

## 🤝 贡献 Contributing

欢迎贡献！请查看 `DEVELOPER_GUIDE.md` 了解开发规范。

Welcome contributions! Check `DEVELOPER_GUIDE.md` for coding standards.

---

## 📞 支持 Support

- GitHub Issues: [VRExpPluginExample_AI Issues](https://github.com/EVRTOP/VRExpPluginExample_AI/issues)
- 文档：查看项目内的各个 README 文件

---

**Version**: 1.0.1 - Foundation Complete
**Last Updated**: 2026-01-03

🚀 **Ready to build amazing VR educational experiences!**