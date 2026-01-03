# 视觉特效 Visual Effects (VFX)

此目录包含 Niagara 粒子系统和视觉特效。
This directory contains Niagara particle systems and visual effects.

## 特效列表 VFX List

### 装配特效 Assembly VFX

#### NS_PartHighlight
零件高亮特效
- 效果: 发光粒子环绕零件
- 颜色: 黄色/绿色/红色(可调)
- 持续: 循环

#### NS_AssemblySuccess
装配成功特效
- 效果: 爆炸式粒子扩散
- 颜色: 绿色/金色
- 持续: 1-2秒

#### NS_AssemblyFail
装配失败特效
- 效果: 红色闪烁粒子
- 颜色: 红色
- 持续: 0.5秒

### 引导特效 Guidance VFX

#### NS_SocketIndicator
装配点指示器
- 效果: 脉冲光环
- 颜色: 蓝色(空闲)、绿色(可用)、灰色(占用)
- 持续: 循环

#### NS_ARArrow
AR引导箭头
- 效果: 动态箭头指向目标
- 颜色: 黄色/绿色
- 持续: 循环

#### NS_PartPreview
零件预览全息
- 效果: 半透明网格
- 颜色: 蓝色/绿色
- 持续: 循环

### 环境特效 Environment VFX

#### NS_Dust
灰尘粒子
- 效果: 漂浮灰尘
- 颜色: 白色/灰色
- 持续: 循环

#### NS_Sparks
火花特效
- 效果: 装配时火花
- 颜色: 橙色/黄色
- 持续: 0.5-1秒

### UI特效 UI VFX

#### NS_ButtonHover
按钮悬停效果
- 效果: 光晕
- 颜色: 白色/蓝色
- 持续: 循环

#### NS_StepComplete
步骤完成特效
- 效果: 勾选标记+粒子
- 颜色: 绿色
- 持续: 1秒

## 特效参数 VFX Parameters

### 通用参数
- Spawn Rate - 生成速率
- Lifetime - 粒子生命周期
- Size - 粒子大小
- Color - 颜色
- Velocity - 速度

### 高级参数
- Emissive Strength - 自发光强度
- Opacity - 不透明度
- Turbulence - 湍流
- Collision - 碰撞

## 性能优化 Performance Optimization

- 限制粒子数量
- 使用GPU粒子
- 控制生成速率
- 及时销毁特效
- 使用LOD系统

## 创建特效 Creating VFX

1. 创建 Niagara System
2. 选择模板或从空白开始
3. 配置 Emitter
4. 调整参数
5. 测试效果
6. 优化性能
