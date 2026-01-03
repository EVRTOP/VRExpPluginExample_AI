# 材质 Materials

此目录包含火箭零件和UI的材质资产。
This directory contains material assets for rocket parts and UI.

## 材质列表 Material List

### 零件材质 Part Materials
- M_RocketMetal - 火箭金属基础材质
- M_RocketMetal_Painted - 涂装金属材质
- M_Engine - 发动机材质
- M_FuelTank - 燃料舱材质
- M_Fairing - 整流罩材质

### 交互材质 Interactive Materials
- M_PartHighlight - 高亮材质
- M_CanAssemble - 可装配状态材质(绿色)
- M_ErrorState - 错误状态材质(红色)
- M_Hologram - 半透明全息材质

### 装配点材质 Socket Materials
- M_SocketIndicator - 装配点指示器材质
- M_SocketIdle - 空闲状态
- M_SocketActive - 活跃状态
- M_SocketOccupied - 已占用状态

### UI材质 UI Materials
- M_UI_Button - 按钮材质
- M_UI_Panel - 面板材质
- M_UI_Icon - 图标材质

## 材质参数 Material Parameters

### 高亮材质参数
- EmissiveColor (Vector3) - 自发光颜色
- EmissiveStrength (Scalar) - 自发光强度
- BaseColor (Vector3) - 基础颜色
- Metallic (Scalar) - 金属度
- Roughness (Scalar) - 粗糙度

### 全息材质参数
- HologramOpacity (Scalar) - 透明度
- ScanlineSpeed (Scalar) - 扫描线速度
- FresnelIntensity (Scalar) - 菲涅尔强度

## 创建材质实例 Creating Material Instances

对于每个零件，创建材质实例以便独立调整参数：
```
MI_Part_<PartName>
例如: MI_Part_MainEngine
```

## 性能优化 Performance Optimization

- 使用材质实例而非独立材质
- 合并相似材质
- 减少材质指令数
- 使用简化材质用于LOD
