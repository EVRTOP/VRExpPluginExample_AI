# 音频资产 Audio Assets

此目录包含音效、语音和音乐资产。
This directory contains sound effects, voice, and music assets.

## 目录结构 Directory Structure

### SFX/ - 音效
交互和反馈音效

### Voice/ - 语音
- CN/ - 中文语音讲解
- EN/ - 英文语音讲解

### Music/ - 音乐
背景音乐

## 音效列表 Sound Effects List

### 交互音效 Interaction SFX
- SFX_Grab - 抓取音效
- SFX_Release - 释放音效
- SFX_AssembleSuccess - 装配成功
- SFX_AssembleFail - 装配失败
- SFX_PartHover - 零件悬停

### UI音效 UI SFX
- SFX_ButtonClick - 按钮点击
- SFX_MenuOpen - 菜单打开
- SFX_MenuClose - 菜单关闭
- SFX_StepComplete - 步骤完成

### 环境音效 Ambient SFX
- SFX_Workshop_Ambient - 车间环境音
- SFX_Footsteps - 脚步声

## 语音文件 Voice Files

### 命名规范
```
Voice_<StepNumber>_<Language>.wav
例如: Voice_001_CN.wav, Voice_001_EN.wav
```

### 内容要求
- 采样率: 44.1kHz
- 格式: WAV or OGG
- 长度: 5-30秒
- 清晰度: 高

## 音乐文件 Music Files

### M_MainMenu
主菜单背景音乐
- 风格: 轻松、科技感
- 循环: 是

### M_Assembly
装配过程音乐
- 风格: 专注、轻音乐
- 循环: 是

### M_Success
完成庆祝音乐
- 风格: 激励、欢快
- 循环: 否

## 音频设置 Audio Settings

### 空间音效
- 启用3D音效
- 衰减距离: 500-2000 cm
- 空气吸收

### 音量平衡
- SFX: 70%
- Voice: 100%
- Music: 40%
