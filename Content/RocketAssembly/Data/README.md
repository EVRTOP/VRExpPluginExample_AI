# 数据表 Data Tables

## 目录说明 Directory Description

此目录包含火箭装配系统的数据表资产。
This directory contains Data Table assets for the rocket assembly system.

## 数据表列表 Data Tables

### DT_RocketParts.uasset
**火箭零件数据表 - Rocket Parts Data Table**

基于 `FRocketPartData` 结构

包含所有火箭零件的详细信息：
- 零件ID、名称(中英文)
- 零件类型
- 3D模型和材质引用
- 物理属性(质量、尺寸)
- 装配点信息
- 装配顺序和依赖
- 科普知识内容
- 语音讲解文件

**示例行 Example Rows:**
```
MainEngine_001:
  - PartNameCN: 主发动机
  - PartNameEN: Main Engine
  - PartType: MainEngineNozzle
  - AssemblyOrder: 2
  - Mass: 500.0 kg
  - DependentPartIDs: [FirstStageBody_001]

FuelTank_LOX_001:
  - PartNameCN: 液氧舱
  - PartNameEN: LOX Tank
  - PartType: LOXTank
  - AssemblyOrder: 4
  - Mass: 800.0 kg
  - DependentPartIDs: [MainEngine_001]
```

### DT_AssemblySteps.uasset
**装配步骤数据表 - Assembly Steps Data Table**

基于 `FAssemblyStepData` 结构

定义分步装配教学流程：
- 步骤序号和名称
- 需要装配的零件ID
- 目标装配点ID
- 教学文本(中英文)
- 语音文件引用
- 提示动画/视频
- AR引导设置

**示例步骤 Example Steps:**
```
Step_001:
  - StepNumber: 1
  - StepNameCN: 放置火箭基座
  - StepNameEN: Place Rocket Base
  - PartIDToAssemble: FirstStageBody_001
  - TargetSocketID: GroundSocket
  - bShowARGuidance: true

Step_002:
  - StepNumber: 2
  - StepNameCN: 安装主发动机
  - StepNameEN: Install Main Engine
  - PartIDToAssemble: MainEngine_001
  - TargetSocketID: EngineSocket_001
  - bShowARGuidance: true
```

### DT_RocketKnowledge.uasset
**火箭科普知识数据表 - Rocket Knowledge Data Table**

基于 `FRocketKnowledgeData` 结构

包含科普教育内容：
- 知识点ID和标题
- 详细内容(中英文)
- 知识类别
- 相关图片和3D模型
- 语音讲解

**知识类别 Knowledge Categories:**
- 发动机原理 Engine Principles
- 燃料知识 Fuel Knowledge
- 多级火箭 Multi-Stage Rockets
- 导航控制 Navigation & Control
- 空气动力学 Aerodynamics
- 历史里程碑 Historical Milestones

**示例内容 Example Content:**
```
Knowledge_Engine_001:
  - TitleCN: 火箭发动机工作原理
  - TitleEN: Rocket Engine Working Principle
  - Category: 发动机原理
  - ContentCN: 火箭发动机通过燃烧推进剂产生高速气流...
  - ContentEN: Rocket engines generate high-speed gas flow...

Knowledge_Fuel_001:
  - TitleCN: 液氧液氢推进剂
  - TitleEN: LOX/LH2 Propellant
  - Category: 燃料知识
  - ContentCN: 液氧液氢是一种高性能推进剂组合...
  - ContentEN: LOX/LH2 is a high-performance propellant...
```

### DT_Achievements.uasset
**成就数据表 - Achievements Data Table**

成就系统配置：
- 成就ID和名称
- 解锁条件
- 奖励
- 图标和描述

**示例成就 Example Achievements:**
```
Achievement_FirstAssembly:
  - NameCN: 初次装配
  - NameEN: First Assembly
  - DescriptionCN: 完成第一个零件的装配
  - DescriptionEN: Complete first part assembly
  - Condition: AssembleFirstPart

Achievement_PerfectAssembly:
  - NameCN: 完美装配
  - NameEN: Perfect Assembly
  - DescriptionCN: 以100%精度完成装配
  - DescriptionEN: Complete assembly with 100% precision
  - Condition: Score >= 100
```

## 创建数据表 Creating Data Tables

### 步骤 Steps
1. 右键 Content Browser -> Miscellaneous -> Data Table
2. 选择行结构 (Row Structure):
   - FRocketPartData (零件)
   - FAssemblyStepData (步骤)
   - FRocketKnowledgeData (知识)
3. 命名数据表
4. 双击打开编辑器
5. 添加行并填充数据

### 导入CSV Import from CSV
也可以从CSV文件导入数据：
1. 准备CSV文件 (第一行为列名)
2. 右键数据表 -> Reimport
3. 选择CSV文件

**CSV格式示例:**
```csv
Name,PartNameCN,PartNameEN,PartType,AssemblyOrder,Mass
MainEngine_001,主发动机,Main Engine,MainEngineNozzle,2,500.0
FuelTank_LOX_001,液氧舱,LOX Tank,LOXTank,4,800.0
```

## 数据验证 Data Validation

### 零件数据验证
- [ ] 所有零件有唯一ID
- [ ] 零件类型正确
- [ ] 装配顺序合理
- [ ] 依赖关系正确
- [ ] 3D模型路径有效
- [ ] 质量和尺寸合理

### 步骤数据验证
- [ ] 步骤序号连续
- [ ] 零件ID存在于零件表中
- [ ] 装配点ID存在
- [ ] 文本内容完整
- [ ] 语音文件存在

### 知识数据验证
- [ ] 知识点ID唯一
- [ ] 内容准确完整
- [ ] 分类正确
- [ ] 资源文件存在

## 本地化 Localization

所有可见文本都提供中英文版本：
- 零件名称、描述
- 步骤说明
- 知识内容
- 成就文本

## 待办事项 TODO

### 数据创建 Data Creation
- [ ] 创建 DT_RocketParts 并添加至少10个零件
- [ ] 创建 DT_AssemblySteps 并定义完整流程
- [ ] 创建 DT_RocketKnowledge 并添加5+知识点
- [ ] 创建 DT_Achievements 并定义成就

### 数据完善 Data Refinement
- [ ] 补充所有零件的3D模型引用
- [ ] 添加所有语音讲解文件
- [ ] 添加科普图片和视频
- [ ] 完善依赖关系
- [ ] 调整装配顺序

### 数据测试 Data Testing
- [ ] 验证所有数据表可正常加载
- [ ] 测试零件生成
- [ ] 测试装配流程
- [ ] 测试知识显示

## 使用示例 Usage Examples

### 在蓝图中读取数据表
```
1. Get Data Table Row
   - Data Table: DT_RocketParts
   - Row Name: MainEngine_001
   - Out Row: FRocketPartData

2. 访问数据
   - PartData.PartNameCN
   - PartData.Mass
   - PartData.AssemblyOrder
```

### 在C++中读取数据表
```cpp
// 加载数据表
UDataTable* PartDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/RocketAssembly/Data/DT_RocketParts"));

// 获取行数据
FRocketPartData* PartData = PartDataTable->FindRow<FRocketPartData>(FName("MainEngine_001"), TEXT(""));

if (PartData)
{
    FText PartName = PartData->PartNameEN;
    float Mass = PartData->Mass;
}
```

## 最佳实践 Best Practices

1. **命名规范** - 使用一致的ID命名规则
2. **数据完整性** - 确保所有必需字段都填写
3. **引用有效性** - 验证资源引用路径
4. **版本控制** - 定期提交数据表更新
5. **文档记录** - 记录重要的数据变更
6. **测试验证** - 每次修改后测试功能
