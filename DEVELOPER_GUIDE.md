# 火箭装配 VR 系统 - 开发者指南
# Rocket Assembly VR System - Developer Guide

## 快速开始 Quick Start

### 环境要求 Requirements
- Unreal Engine 5.3
- Visual Studio 2022
- Windows 10/11 (开发环境)
- VR 设备 (可选，支持 Oculus, Vive, Index)

### 编译项目 Building the Project

1. **克隆仓库 Clone Repository**
```bash
git clone https://github.com/EVRTOP/VRExpPluginExample_AI.git
cd VRExpPluginExample_AI
```

2. **生成项目文件 Generate Project Files**
- 右键 `VRExpPluginExample.uproject`
- 选择 "Generate Visual Studio project files"

3. **编译 Compile**
- 打开 `VRExpPluginExample.sln`
- 选择 "Development Editor" 配置
- 按 F7 或 Build -> Build Solution

4. **运行编辑器 Run Editor**
- 按 F5 或直接双击 .uproject 文件

### 目录结构 Directory Structure

```
VRExpPluginExample_AI/
├── Config/                      # 项目配置
├── Content/                     # 内容资产
│   ├── RocketAssembly/         # 火箭装配系统资产
│   │   ├── Blueprints/         # 蓝图
│   │   ├── UI/                 # UI Widget
│   │   ├── Data/               # 数据表
│   │   ├── Materials/          # 材质
│   │   ├── Meshes/             # 3D模型
│   │   ├── Audio/              # 音频
│   │   ├── VFX/                # 特效
│   │   └── Maps/               # 关卡
│   └── VRE/                    # VR基础资产
├── Plugins/                     # 插件
│   ├── VRExpansionPlugin/      # VR扩展插件
│   └── AdvancedSessions/       # 会话插件
└── Source/                      # C++源代码
    └── VRExpPluginExample/
        └── RocketAssembly/     # 火箭装配C++类
```

## 核心系统架构 Core System Architecture

### 1. 数据层 Data Layer

#### RocketPartData.h
定义所有数据结构：
- `ERocketPartType` - 20+种零件类型
- `EAssemblyState` - 装配状态
- `FRocketPartData` - 零件数据结构
- `FAssemblyStepData` - 装配步骤
- `FRocketKnowledgeData` - 科普知识

### 2. 游戏逻辑层 Game Logic Layer

#### InteractiveRocketPart (C++)
可交互零件 Actor
- 继承: `AGrippableStaticMeshActor`
- 功能: 抓取、装配、高亮、状态管理

#### AssemblySocket (C++)
装配点组件
- 继承: `USceneComponent`
- 功能: 对齐检测、磁吸、验证

#### AssemblyValidator (C++)
装配验证器
- 继承: `UObject`
- 功能: 验证、评分、碰撞检测

### 3. 管理层 Management Layer (Blueprint)

#### BP_RocketPartManager
零件管理器
- 生成/销毁零件
- 库存管理
- 进度保存

#### BP_AssemblyTutorialManager
教学管理器
- 步骤控制
- 提示显示
- AR引导

#### BP_AssessmentManager
评估管理器
- 评分计算
- 成绩报告
- 统计数据

### 4. 表现层 Presentation Layer (Blueprint + UMG)

#### UI Widgets
- WBP_MainMenu - 主菜单
- WBP_AssemblyHUD - HUD
- WBP_PartInfoPanel - 信息面板
- WBP_ScoreReport - 成绩报告

## 开发工作流 Development Workflow

### 添加新零件 Adding a New Part

#### 1. 在数据表中定义零件
打开 `Content/RocketAssembly/Data/DT_RocketParts`，添加新行：
```
NewPart_001:
  PartNameCN: "新零件"
  PartNameEN: "New Part"
  PartType: Other
  AssemblyOrder: 11
  Mass: 100.0
  ...
```

#### 2. 创建零件蓝图
- 继承 `BP_RocketPart_Base` 或 `AInteractiveRocketPart`
- 命名: `BP_Part_NewPart`
- 设置静态网格
- 关联数据表行

#### 3. 创建装配点
在目标装配位置添加 `UAssemblySocket` 组件：
```
- Socket ID: NewPartSocket
- Accepted Part IDs: [NewPart_001]
- Position Tolerance: 5.0
- Rotation Tolerance: 10.0
```

#### 4. 添加到装配步骤
在 `DT_AssemblySteps` 中添加步骤：
```
Step_011:
  StepNumber: 11
  PartIDToAssemble: NewPart_001
  TargetSocketID: NewPartSocket
  ...
```

#### 5. 测试
- 在测试关卡中生成零件
- 测试抓取功能
- 测试装配到装配点
- 验证对齐和验证逻辑

### 创建自定义验证规则

#### C++ 方式
在 `AssemblyValidator.cpp` 中添加新验证函数：
```cpp
bool UAssemblyValidator::ValidateCustomRule(
    AInteractiveRocketPart* Part,
    UAssemblySocket* Socket)
{
    // 自定义验证逻辑
    return true;
}
```

#### Blueprint 方式
在 `BP_AssemblyTutorialManager` 中：
1. 添加自定义事件
2. 实现验证逻辑
3. 在装配时调用

### 添加科普知识

#### 1. 准备内容
- 文本内容(中英文)
- 图片/视频
- 3D模型(可选)
- 语音讲解(可选)

#### 2. 在数据表中添加
打开 `DT_RocketKnowledge`：
```
Knowledge_New_001:
  TitleCN: "新知识点"
  TitleEN: "New Knowledge"
  Category: "分类"
  ContentCN: "详细内容..."
  ContentEN: "Detailed content..."
```

#### 3. 关联到零件
在 `DT_RocketParts` 中设置：
```
PartData.EducationalContentCN = "参考 Knowledge_New_001"
```

#### 4. 在UI中显示
在 `WBP_PartInfoPanel` 中读取并显示知识内容

## API 参考 API Reference

### InteractiveRocketPart

#### 主要函数 Main Functions

```cpp
// 设置装配状态
void SetAssemblyState(EAssemblyState NewState);

// 检查是否可装配
bool CanAssembleToSocket(UAssemblySocket* Socket);

// 装配到装配点
bool AssembleToSocket(UAssemblySocket* Socket);

// 从装配点卸下
void DisassembleFromSocket();

// 锁定/解锁零件
void LockPart(bool bLock);

// 启用高亮
void EnableHighlight(bool bEnable);

// 获取本地化名称
FText GetLocalizedPartName() const;
```

#### 主要事件 Main Events

```cpp
// 零件被抓取
FVROnGripSignature OnGripped;

// 零件被放下
FVROnDropSignature OnDropped;

// 零件装配完成
FVROnGripSignature OnPartAssembled;

// 装配失败
FVROnGripSignature OnAssemblyFailed;
```

### AssemblySocket

#### 主要函数 Main Functions

```cpp
// 检查零件类型是否接受
bool IsPartTypeAccepted(ERocketPartType PartType) const;

// 检查是否可装配
bool CanAssemblePart(AInteractiveRocketPart* Part) const;

// 装配零件
bool AssemblePart(AInteractiveRocketPart* Part);

// 卸下零件
void DisassemblePart();

// 应用磁吸效果
FTransform ApplySnapEffect(const FTransform& PartTransform, float DeltaTime);

// 更新指示器
void UpdateIndicator();
```

#### 主要事件 Main Events

```cpp
// 零件装配完成
FOnPartAssembled OnPartAssembled;

// 零件卸下
FOnPartDisassembled OnPartDisassembled;

// 零件进入范围
FOnPartEnterRange OnPartEnterRange;

// 零件离开范围
FOnPartLeaveRange OnPartLeaveRange;
```

### AssemblyValidator

#### 主要函数 Main Functions

```cpp
// 验证装配
FAssemblyValidationResult ValidatePartAssembly(
    AInteractiveRocketPart* Part,
    UAssemblySocket* Socket,
    const TArray<AInteractiveRocketPart*>& AssembledParts);

// 验证位置对齐
bool ValidatePositionAlignment(
    const FVector& PartLocation,
    const FVector& SocketLocation,
    float& OutDeviation);

// 验证旋转对齐
bool ValidateRotationAlignment(
    const FRotator& PartRotation,
    const FRotator& SocketRotation,
    float& OutDeviation);

// 计算精度分数
float CalculatePrecisionScore(
    float PositionDeviation,
    float RotationDeviation);
```

## 调试技巧 Debugging Tips

### 1. 可视化调试
在 `AssemblySocket::TickComponent` 中添加：
```cpp
// 绘制装配点位置
DrawDebugSphere(GetWorld(), GetComponentLocation(), 10.0f, 12, FColor::Green);

// 绘制磁吸范围
DrawDebugSphere(GetWorld(), GetComponentLocation(), SnapDistance, 24, FColor::Yellow);
```

### 2. 日志输出
```cpp
UE_LOG(LogTemp, Warning, TEXT("Part %s assembled"), *PartData.PartNameEN.ToString());
```

### 3. Blueprint 调试
- 使用 Print String 节点
- 设置断点 (右键节点 -> Add Breakpoint)
- 查看变量值 (Watch 窗口)

### 4. VR 调试
- 使用 VR Preview 模式
- 启用 VR Editor 插件
- 使用 Editor 中的 VR 调试工具

## 性能优化 Performance Optimization

### 1. LOD 系统
为所有3D模型创建LOD级别：
- LOD0: 高精度 (近距离)
- LOD1: 中等精度
- LOD2: 低精度
- LOD3: 最低精度 (远距离)

### 2. 材质优化
- 使用材质实例
- 合并材质通道
- 减少材质指令数

### 3. 蓝图优化
- 避免在 Tick 中执行复杂逻辑
- 使用 Timer 替代 Tick
- 缓存引用，避免重复查找

### 4. VR 性能
- 目标帧率: 90 FPS (PC), 72 FPS (Quest)
- 使用 Forward Rendering
- 启用 Instanced Stereo Rendering
- 优化阴影和光照

## 测试清单 Testing Checklist

### 功能测试 Functional Testing
- [ ] 所有零件可正常抓取
- [ ] 装配点正确检测对齐
- [ ] 装配验证正确工作
- [ ] 教学步骤流程完整
- [ ] UI 所有功能正常
- [ ] 评分系统准确
- [ ] 数据表正确加载
- [ ] 本地化正常切换

### VR 体验测试 VR Experience Testing
- [ ] 手柄映射正确
- [ ] 抓取手感良好
- [ ] 无明显延迟
- [ ] 无眩晕感
- [ ] 音效空间定位准确
- [ ] UI 可读性好

### 性能测试 Performance Testing
- [ ] 帧率稳定在目标值
- [ ] 内存占用合理
- [ ] 加载时间可接受
- [ ] 无明显卡顿

### 兼容性测试 Compatibility Testing
- [ ] Windows VR 设备
- [ ] Meta Quest
- [ ] 不同 VR 设备

## 常见问题 Common Issues

### 问题：零件无法抓取
**解决方案:**
1. 检查零件是否继承 `AGrippableStaticMeshActor`
2. 确认 `SetDenyGripping(false)`
3. 检查碰撞设置
4. 验证 VR 手柄输入

### 问题：装配点不工作
**解决方案:**
1. 确认 Socket ID 唯一
2. 检查 Accepted Part Types/IDs
3. 验证对齐容差设置
4. 检查 Tick 是否启用

### 问题：性能低下
**解决方案:**
1. 检查多边形数量
2. 优化材质
3. 使用 LOD
4. 减少 Draw Calls
5. 使用 Profiler 分析

### 问题：编译错误
**解决方案:**
1. 清理项目 (Delete Binaries, Intermediate, Saved)
2. 重新生成项目文件
3. 检查头文件包含
4. 验证模块依赖

## 扩展开发 Extension Development

### 添加新的零件类型
1. 在 `ERocketPartType` 枚举中添加新类型
2. 重新编译C++代码
3. 更新数据表

### 添加新的游戏模式
1. 创建新的 GameMode Blueprint
2. 继承 `BP_RocketAssemblyGameMode`
3. 实现自定义规则
4. 在关卡中设置

### 支持新的VR设备
1. 在 .uproject 中启用对应插件
2. 配置输入映射
3. 测试手柄功能
4. 调整 UI 显示

## 贡献指南 Contributing Guide

### 代码规范 Code Standards
- C++: 遵循 UE5 编码标准
- Blueprint: 保持整洁，添加注释
- 命名: 使用有意义的名称

### 提交流程 Submission Process
1. Fork 项目
2. 创建功能分支
3. 提交更改
4. 创建 Pull Request
5. 等待代码审查

### 文档更新 Documentation Updates
- 更新相关 README
- 添加代码注释
- 更新 API 文档

## 资源链接 Resource Links

- [Unreal Engine Documentation](https://docs.unrealengine.com/)
- [VRExpansionPlugin Documentation](https://vreue4.com/)
- [OpenXR Documentation](https://www.khronos.org/openxr/)

## 联系支持 Contact & Support

- GitHub Issues: https://github.com/EVRTOP/VRExpPluginExample_AI/issues
- 邮件: support@example.com (替换为实际联系方式)

---

**最后更新**: 2026-01-03
**版本**: 0.1.0 (Phase 1 完成)
