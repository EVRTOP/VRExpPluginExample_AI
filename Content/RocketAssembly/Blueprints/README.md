# 蓝图资产 Blueprint Assets

## 目录说明 Directory Description

此目录包含火箭装配系统的蓝图类和逻辑。
This directory contains Blueprint classes and logic for the rocket assembly system.

## 主要蓝图 Main Blueprints

### 管理器 Managers

#### BP_RocketPartManager
零件管理器 - Part Manager
- 管理所有火箭零件的生成和销毁
- 零件库存系统
- 零件重置功能
- 保存/加载装配进度

**功能 Functions:**
- `SpawnPart(PartID)` - 生成零件
- `DestroyPart(Part)` - 销毁零件
- `ResetAllParts()` - 重置所有零件
- `GetAvailableParts()` - 获取可用零件列表
- `SaveProgress()` - 保存进度
- `LoadProgress()` - 加载进度

#### BP_AssemblyTutorialManager
教学管理器 - Tutorial Manager
- 分步教学流程控制
- 步骤提示显示
- 高亮下一个零件
- 显示装配点位置
- 错误检测与纠正

**功能 Functions:**
- `StartTutorial()` - 开始教学
- `NextStep()` - 下一步
- `PreviousStep()` - 上一步
- `ShowHint()` - 显示提示
- `CheckStepCompletion()` - 检查步骤完成

#### BP_AssessmentManager
评估管理器 - Assessment Manager
- 装配正确率计算
- 时间与失误统计
- 评分系统
- 成绩报告生成

**评分维度 Scoring Dimensions:**
- 装配正确率 (40%)
- 装配顺序 (30%)
- 完成时间 (15%)
- 失误次数 (15%)

### 零件基类 Part Base Classes

#### BP_RocketPart_Base
零件蓝图基类 - Part Blueprint Base
- 继承自 C++ `AInteractiveRocketPart`
- 添加蓝图特定功能
- 信息标签显示
- 音效和特效

**蓝图可编辑变量 Editable Variables:**
- Part Data (from Data Table)
- Highlight Materials
- Assembly Sounds
- VFX Effects

### 具体零件 Specific Parts

在 `Parts/` 子目录中创建具体零件蓝图：

#### BP_Part_MainEngine
主发动机零件
- 类型: MainEngineNozzle
- 装配顺序: 2
- 依赖: FirstStageBody

#### BP_Part_FuelTank_LOX
液氧舱零件
- 类型: LOXTank
- 装配顺序: 4
- 依赖: MainEngine

#### BP_Part_FuelTank_LH2
液氢舱零件
- 类型: FuelTank
- 装配顺序: 5
- 依赖: LOXTank

#### BP_Part_Fairing_Left
整流罩(左)
- 类型: FairingLeft
- 装配顺序: 9
- 依赖: SecondStageBody

#### BP_Part_Fairing_Right
整流罩(右)
- 类型: FairingRight
- 装配顺序: 10
- 依赖: FairingLeft

### 装配点 Assembly Sockets

#### BP_AssemblySocket
装配点蓝图 - Assembly Socket Blueprint
- 继承自 C++ `UAssemblySocket`
- 可视化配置
- 指示器网格

**配置参数 Configuration:**
- Socket ID
- Accepted Part Types/IDs
- Position/Rotation Tolerance
- Snap Distance/Strength
- Indicator Settings

### 游戏模式 Game Modes

#### BP_RocketAssemblyGameMode
火箭装配游戏模式
- 管理游戏流程
- 模式切换 (自由/教学/考核)
- 玩家状态管理

#### BP_RocketAssemblyPlayerController
玩家控制器
- VR 输入处理
- UI 交互
- 相机控制

#### BP_VRPawn
VR 玩家 Pawn
- VR 手柄控制
- 传送系统
- 抓取交互

## 蓝图命名规范 Blueprint Naming Convention

```
BP_<Category>_<Name>
```

类别前缀：
- `BP_` - 标准蓝图 Standard Blueprint
- `BPW_` - Widget 蓝图 Widget Blueprint (但UI通常用 WBP_)
- `BPC_` - 组件蓝图 Component Blueprint
- `BPI_` - 接口蓝图 Interface Blueprint

## 创建蓝图指南 Blueprint Creation Guide

### 创建零件蓝图
1. 右键 Content Browser -> Blueprint Class
2. 选择父类: `AInteractiveRocketPart`
3. 命名: `BP_Part_<PartName>`
4. 设置静态网格
5. 配置零件数据 (引用 Data Table)
6. 添加音效和特效
7. 测试抓取和装配功能

### 创建装配点
1. 在目标 Actor 蓝图中添加 `AssemblySocket` 组件
2. 设置位置和旋转
3. 配置 Socket ID
4. 指定接受的零件类型
5. 调整对齐参数
6. 添加可视化指示器

## 事件系统 Event System

### 零件事件 Part Events
- `OnGripped` - 被抓取
- `OnDropped` - 被放下
- `OnPartHovered` - 被悬停
- `OnPartAssembled` - 装配完成
- `OnAssemblyFailed` - 装配失败

### 装配点事件 Socket Events
- `OnPartAssembled` - 零件装配
- `OnPartDisassembled` - 零件卸下
- `OnPartEnterRange` - 零件进入范围
- `OnPartLeaveRange` - 零件离开范围

## 待办事项 TODO

### 高优先级 High Priority
- [x] BP_RocketPart_Base 基类
- [ ] BP_RocketPartManager 管理器
- [ ] BP_AssemblyTutorialManager 教学管理器
- [ ] 创建至少10个零件蓝图
- [ ] 创建装配点蓝图

### 中优先级 Medium Priority
- [ ] BP_AssessmentManager 评估管理器
- [ ] BP_RocketAssemblyGameMode 游戏模式
- [ ] BP_VRPawn VR玩家
- [ ] 音效和特效集成

### 低优先级 Low Priority
- [ ] 高级抓取功能
- [ ] 多人协作支持
- [ ] 成就系统
- [ ] 回放系统

## 最佳实践 Best Practices

1. **注释** - 为重要节点添加注释
2. **组织** - 使用 Reroute 节点保持整洁
3. **函数** - 将重复逻辑封装为函数
4. **变量** - 使用有意义的变量名
5. **分类** - 使用 Category 组织变量
6. **文档** - 在蓝图描述中添加说明
7. **测试** - 创建测试关卡验证功能
