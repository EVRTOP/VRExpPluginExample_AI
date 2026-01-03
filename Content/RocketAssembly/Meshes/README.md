# 火箭零件 3D 模型 Rocket Parts 3D Meshes

## 目录说明 Directory Description

此目录用于存放火箭零件的 3D 模型文件。
This directory contains 3D mesh assets for rocket parts.

## 子目录结构 Subdirectories

### Engines/ - 发动机模型
- 主发动机喷嘴 Main Engine Nozzles
- 涡轮泵 Turbo Pumps
- 燃烧室 Combustion Chambers

### FuelTanks/ - 燃料舱模型
- 液氧舱 LOX Tanks
- 液氢/煤油舱 LH2/Kerosene Tanks
- 燃料管路 Fuel Pipelines

### Fairings/ - 整流罩模型
- 整流罩左半 Fairing Left Half
- 整流罩右半 Fairing Right Half
- 卫星适配器 Satellite Adapters

### ControlSystems/ - 控制系统模型
- 姿态控制发动机 Attitude Control Engines
- 陀螺仪 Gyroscopes
- 控制计算机 Control Computers

### Structures/ - 结构件模型
- 一级火箭筒体 First Stage Body
- 二级火箭筒体 Second Stage Body
- 级间段 Interstage Sections
- 支撑结构 Support Structures

## 模型要求 Model Requirements

### 多边形数量 Polygon Count
- 简化模型(LOD3): < 1000 tris
- 中等模型(LOD2): 1000-5000 tris
- 详细模型(LOD1): 5000-15000 tris
- 高精度模型(LOD0): < 30000 tris

### 材质 Materials
- 使用基于物理的材质 PBR materials
- 包含法线贴图 Normal maps
- 粗糙度和金属度贴图 Roughness and Metallic maps

### 命名规范 Naming Convention
```
SM_<PartType>_<PartName>_LOD<N>
例如: SM_Engine_MainNozzle_LOD0
```

### 碰撞体 Collision
- 每个模型应包含简化碰撞体
- 使用 UCX_ 前缀命名碰撞网格
- 保持碰撞体多边形数量最小化

### 中心点 Pivot Point
- 设置合适的中心点位置
- 对于对称零件，中心点应在对称轴上
- 考虑装配点的位置

## 当前状态 Current Status

⚠️ **占位符阶段** - 等待3D美术资产
使用简单几何体作为占位符：
- 立方体 Cubes
- 圆柱体 Cylinders
- 球体 Spheres

## 待办事项 TODO

- [ ] 创建主发动机模型
- [ ] 创建燃料舱模型
- [ ] 创建整流罩模型
- [ ] 创建控制系统模型
- [ ] 创建结构件模型
- [ ] 优化LOD层级
- [ ] 创建碰撞体
- [ ] 烘焙光照贴图UV

## 参考资料 References

参考真实火箭模型：
- 长征系列 Long March series
- 猎鹰9号 Falcon 9
- 土星五号 Saturn V
- 阿丽亚娜5号 Ariane 5
