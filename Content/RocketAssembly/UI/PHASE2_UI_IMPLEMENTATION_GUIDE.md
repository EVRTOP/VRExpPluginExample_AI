# Phase 2 - UMG UI Implementation Guide
# 火箭装配系统 Phase 2 UI 实现指南

## 📋 Overview 概述

This guide provides detailed specifications for implementing all UMG (Unreal Motion Graphics) user interface widgets for the VR Rocket Assembly Educational System.

本指南提供了为 VR 火箭装配教育系统实现所有 UMG（虚幻运动图形）用户界面小部件的详细规范。

---

## VR UI Design Principles VR UI 设计原则

### General Guidelines 通用指南

1. **Font Size 字体大小**
   - Minimum: 24pt for body text
   - Headings: 36-48pt
   - Distance-based scaling for 3D widgets

2. **Colors 颜色**
   - High contrast for readability
   - Avoid pure red/green (colorblind friendly)
   - Semi-transparent backgrounds (0.6-0.8 alpha)

3. **Layout 布局**
   - Keep important UI in center 60% of view
   - Avoid extreme edges
   - Minimum button size: 100x100 pixels

4. **Interaction 交互**
   - Ray pointer for VR controllers
   - Hover feedback (scale, glow)
   - Audio feedback on clicks
   - Haptic feedback on VR controllers

5. **3D Widget Positioning 3D Widget 定位**
   - Distance: 1-2 meters from player
   - Angle: Slightly below eye level
   - Can be world-locked or follow player

---

## 1. WBP_MainMenu - 主菜单界面

### Purpose 目的
Main menu for mode selection, settings, and game start.
用于模式选择、设置和游戏启动的主菜单。

### Location 位置
`Content/RocketAssembly/UI/WBP_MainMenu.uasset`

### Widget Type
User Widget (for 3D Widget Component)

### Layout Structure 布局结构

```
Canvas Panel (Root)
└── Overlay
    ├── Background (Image)
    │   └── Video Player (Background Animation)
    │
    ├── Title Section (Vertical Box)
    │   ├── Main Title (Text Block)
    │   │   - Text: "火箭装配 VR 科普教育"
    │   │   - Font Size: 72
    │   │   - Color: White
    │   │   - Shadow: 2px black
    │   ├── Subtitle (Text Block)
    │       - Text: "Rocket Assembly VR Educational System"
    │       - Font Size: 36
    │       - Color: Light Gray
    │
    ├── Mode Selection (Vertical Box)
    │   ├── Tutorial Mode Button
    │   ├── Free Mode Button
    │   ├── Assessment Mode Button
    │   └── Knowledge Mode Button
    │
    ├── Difficulty Selection (Horizontal Box)
    │   ├── Simple Button
    │   ├── Standard Button
    │   └── Professional Button
    │
    ├── Progress Section (Vertical Box)
    │   ├── Continue Progress Button
    │   ├── New Game Button
    │   └── History Button
    │
    └── Bottom Bar (Horizontal Box)
        ├── Settings Button
        ├── Language Toggle
        └── Exit Button
```

### Widget Variables 小部件变量

```
CurrentLanguage (String)
- Default: "CN"
- Options: "CN", "EN"

SelectedMode (Enum: EGameMode)
- Default: Tutorial

SelectedDifficulty (Enum: EDifficulty)
- Default: Standard

HasSaveData (Boolean)
- Determines if "Continue" button is enabled

GameModeReference (Object Reference)
- Reference to BP_RocketAssemblyGameMode
```

### Widget Components 小部件组件

#### Mode Selection Buttons 模式选择按钮

**Tutorial Mode Button (教学模式按钮)**
```
Style:
- Size: 400x120
- Background: Semi-transparent blue (0, 0.5, 1, 0.7)
- Icon: 🎓 (mortarboard)
- Text: "教学模式 Tutorial Mode"
- Font Size: 32

Hover Effect:
- Scale: 1.1
- Glow: Bright blue outline
- Sound: UI_Button_Hover

Click Event:
1. Play sound: UI_Button_Click
2. Set SelectedMode = Tutorial
3. Show mode description panel
4. Highlight button
```

**Free Mode Button (自由模式按钮)**
```
Style:
- Background: Semi-transparent green (0, 1, 0.5, 0.7)
- Icon: 🆓
- Text: "自由模式 Free Mode"

Click Event:
1. Set SelectedMode = Free
2. Show mode description
```

**Assessment Mode Button (考核模式按钮)**
```
Style:
- Background: Semi-transparent orange (1, 0.5, 0, 0.7)
- Icon: 📝
- Text: "考核模式 Assessment Mode"

Click Event:
1. Set SelectedMode = Assessment
2. Show mode description
3. Show warning about timer/no retries
```

**Knowledge Mode Button (知识浏览按钮)**
```
Style:
- Background: Semi-transparent purple (0.7, 0, 1, 0.7)
- Icon: 📚
- Text: "知识浏览 Knowledge Browser"

Click Event:
1. Set SelectedMode = Knowledge
2. Show mode description
```

#### Mode Description Panel 模式描述面板
```
Appears on button hover
Size: 500x200
Background: Black (0.8 alpha)
Contains:
- Mode name
- Brief description
- Features list
- Recommended for
```

#### Difficulty Buttons 难度按钮
```
Simple:
- Text: "简化版 Simple (5 parts)"
- Color: Green

Standard:
- Text: "标准版 Standard (10 parts)"
- Color: Yellow
- Default selected

Professional:
- Text: "专业版 Professional (20+ parts)"
- Color: Red
```

#### Settings Button 设置按钮
```
Icon: ⚙️
Size: 80x80
Position: Bottom-left
Click: Open WBP_Settings
```

#### Language Toggle 语言切换
```
Type: Button
Text: "中文/EN"
Position: Bottom-center
Click: Toggle between CN and EN
- Update all text in menu
- Save preference
```

#### Start Game Button 开始游戏按钮
```
Size: 300x80
Background: Bright green
Text: "开始游戏 Start Game"
Position: Center-bottom
Enabled: When mode selected

Click Event:
1. Play confirmation sound
2. Fade out menu (1 second)
3. Call GameMode.SetGameMode(SelectedMode)
4. Call GameMode.StartGame()
5. Remove widget from viewport
```

### Functions 函数

#### Event Construct
```
Implementation:
1. Load saved language preference
2. Apply language to all text
3. Check for save data:
   - If exists: Enable "Continue" button
   - Else: Disable "Continue" button
4. Set default selections (Tutorial + Standard)
5. Bind button events
6. Play menu music
```

#### OnModeButtonClicked(Mode: EGameMode)
```
Parameters:
- Mode: EGameMode

Implementation:
1. Set SelectedMode = Mode
2. Update button highlights (selected button glows)
3. Show mode description panel
4. Update start button text based on mode
```

#### OnDifficultyButtonClicked(Difficulty: EDifficulty)
```
Parameters:
- Difficulty: EDifficulty

Implementation:
1. Set SelectedDifficulty = Difficulty
2. Update button highlights
3. Update part count in description
```

#### OnContinueButtonClicked()
```
Implementation:
1. Load save data
2. Call GameMode.LoadProgress()
3. Close menu
4. Start game from saved state
```

#### OnNewGameButtonClicked()
```
Implementation:
1. If save data exists:
   - Show confirmation dialog ("Overwrite existing progress?")
2. Clear save data
3. Start new game with selected mode/difficulty
```

#### OnLanguageToggleClicked()
```
Implementation:
1. Toggle CurrentLanguage:
   - If "CN" → Set to "EN"
   - If "EN" → Set to "CN"
2. Update all text widgets:
   - Button texts
   - Descriptions
   - Tooltips
3. Save language preference to game settings
```

---

## 2. WBP_AssemblyHUD - HUD 界面

### Purpose 目的
Heads-up display showing current step, progress, and interaction prompts.
显示当前步骤、进度和交互提示的平视显示器。

### Location 位置
`Content/RocketAssembly/UI/WBP_AssemblyHUD.uasset`

### Widget Type
User Widget (attached to player viewport)

### Layout Structure 布局结构

```
Canvas Panel (Root)
├── Top Bar (Horizontal Box)
│   ├── Step Information (Vertical Box)
│   │   ├── Current Step Text
│   │   └── Progress Bar
│   └── Mode Badge
│
├── Center Area
│   └── Part Info Card (appear on gaze)
│       ├── Part Name
│       ├── Part Description
│       └── "View Details" Button
│
├── Bottom Prompts (Horizontal Box)
│   ├── Controller Icon + Action 1
│   ├── Controller Icon + Action 2
│   └── Controller Icon + Action 3
│
├── Top-Right Status
│   ├── Timer (if Assessment mode)
│   ├── Current Score
│   └── Error Count
│
└── Top-Left Menu
    ├── Pause Button
    ├── Restart Button
    ├── Settings Button
    └── Help Button
```

### Widget Variables 小部件变量

```
TutorialManagerRef (Object Reference)
- Reference to BP_AssemblyTutorialManager

AssessmentManagerRef (Object Reference)
- Reference to BP_AssessmentManager

CurrentStepIndex (Integer)
- Current step number

TotalSteps (Integer)
- Total number of steps

CurrentPartInfo (FRocketPartData)
- Data for currently gazed part

ShowPartInfo (Boolean)
- Toggle part info card visibility

ElapsedTime (Float)
- For timer display

CurrentScore (Float)
- Real-time score
```

### Widget Components 小部件组件

#### Current Step Display 当前步骤显示
```
Component: Rich Text Block
Size: 800x80
Position: Top-center
Background: Semi-transparent black (0.7 alpha)
Font Size: 32

Text Format:
"第 {StepIndex}/{TotalSteps} 步: {StepDescription}"
"Step {StepIndex}/{TotalSteps}: {StepDescription}"

Example:
"第 3/10 步: 安装液氧燃料舱"
"Step 3/10: Install LOX Fuel Tank"

Update: When TutorialManager fires OnStepCompleted
```

#### Progress Bar 进度条
```
Component: Progress Bar
Size: 600x20
Position: Below step text
Fill Color: Green gradient
Background Color: Dark gray

Value: CurrentStepIndex / TotalSteps
Animation: Smooth fill (0.5 second transition)
```

#### Mode Badge 模式标识
```
Component: Border with Text
Size: 150x40
Position: Top-right of step info
Background: Mode-specific color
Text: Mode name

Colors:
- Tutorial: Blue
- Free: Green
- Assessment: Orange
- Knowledge: Purple
```

#### Part Info Card 零件信息卡片
```
Component: Border (appears in center on gaze)
Size: 400x300
Background: Semi-transparent black (0.8 alpha)
Visibility: Hidden by default

Content:
- Part Icon (Image, 80x80)
- Part Name (Text, Size 28)
  - Chinese name
  - English name
- Brief Description (Text, Size 18)
  - 2-3 lines
- "View Details" Button
  - Opens WBP_PartInfoPanel

Trigger: When player gazes at part for 0.5 seconds
Hide: When gaze leaves part or button clicked
```

#### Controller Prompts 手柄提示
```
Component: Horizontal Box with Image+Text pairs
Position: Bottom-center
Background: Semi-transparent black (0.6 alpha)

Format: [Controller Icon] - [Action Text]

Dynamic content based on context:
Default:
- [Trigger] - 抓取 Grip
- [A Button] - 查看信息 View Info
- [Menu] - 暂停 Pause

When holding part:
- [Trigger] - 放下 Release
- [A Button] - 旋转 Rotate
- [B Button] - 重置 Reset
```

#### Timer Display 计时器显示
```
Component: Text Block
Position: Top-right
Font Size: 36
Color: White (yellow if running out of time)
Format: "MM:SS"

Visible: Only in Assessment mode
Update: Every 0.1 seconds in Tick

Implementation:
1. Get elapsed time from AssessmentManager
2. Format as minutes:seconds
3. If time > threshold, change color to yellow/red
```

#### Score Display 分数显示
```
Component: Text Block
Position: Below timer
Font Size: 28
Color: White (green if high, yellow if medium, red if low)
Format: "Score: XXX"

Visible: In Assessment and Tutorial modes
Update: Real-time from AssessmentManager
```

#### Error Counter 错误计数器
```
Component: Horizontal Box (Icon + Number)
Position: Below score
Icon: ❌ red X
Font Size: 24
Format: "Errors: X"

Visible: All modes except Free
Update: When AssessmentManager.RecordError() called
```

#### Quick Menu Buttons 快捷菜单按钮
```
Component: Vertical Box
Position: Top-left
Each button: 60x60

Pause Button:
- Icon: ⏸️
- Tooltip: "Pause"
- Click: Call GameMode.PauseGame()

Restart Button:
- Icon: 🔄
- Tooltip: "Restart"
- Click: Show confirmation → GameMode.RestartGame()

Settings Button:
- Icon: ⚙️
- Tooltip: "Settings"
- Click: Open WBP_Settings

Help Button:
- Icon: ❓
- Tooltip: "Help"
- Click: Show help overlay
```

### Functions 函数

#### Event Construct
```
Implementation:
1. Get references to managers:
   - TutorialManagerRef
   - AssessmentManagerRef
2. Bind to manager events:
   - TutorialManager.OnStepCompleted → UpdateStepDisplay
   - AssessmentManager.RecordError → UpdateErrorCount
3. Initialize displays:
   - Hide Part Info Card
   - Update controller prompts
4. Start timer if Assessment mode
```

#### Event Tick
```
Implementation:
1. If Assessment mode:
   - Update timer display
   - Update score display
2. Check for part gaze:
   - Raycast from player view
   - If hit InteractiveRocketPart:
     - If gaze time > 0.5s:
       - Show Part Info Card
   - Else:
     - Hide Part Info Card
```

#### UpdateStepDisplay(StepIndex: Integer, StepData: FAssemblyStepData)
```
Parameters:
- StepIndex: Integer
- StepData: FAssemblyStepData

Implementation:
1. Set CurrentStepIndex = StepIndex
2. Update step text with StepData description
3. Update progress bar value
4. Play update animation (brief highlight)
5. Update controller prompts based on step
```

#### ShowPartInfoCard(PartData: FRocketPartData)
```
Parameters:
- PartData: FRocketPartData

Implementation:
1. Set CurrentPartInfo = PartData
2. Update card content:
   - Part name (localized)
   - Part description (localized)
   - Part icon
3. Animate card appearance (fade in + scale)
4. Set ShowPartInfo = true
```

#### HidePartInfoCard()
```
Implementation:
1. Animate card disappearance (fade out)
2. Set ShowPartInfo = false
```

#### UpdateControllerPrompts(Context: String)
```
Parameters:
- Context: String ("Default", "HoldingPart", "NearSocket")

Implementation:
1. Clear current prompts
2. Based on Context, add appropriate prompts:
   - Load controller button images
   - Set action text (localized)
3. Update layout
```

---

## 3. WBP_PartInfoPanel - 零件信息面板

### Purpose 目的
Detailed information panel for rocket parts including specs, description, and multimedia.
火箭零件的详细信息面板，包括规格、描述和多媒体。

### Location 位置
`Content/RocketAssembly/UI/WBP_PartInfoPanel.uasset`

### Widget Type
User Widget (3D Widget or Overlay)

### Layout Structure 布局结构

```
Canvas Panel (Root)
└── Border (Main Panel)
    ├── Title Bar (Horizontal Box)
    │   ├── Part Icon
    │   ├── Part Name (CN + EN)
    │   └── Close Button
    │
    ├── Content Area (Scroll Box)
    │   ├── Parameters Section
    │   │   └── Data Table
    │   │       ├── Dimension Row
    │   │       ├── Mass Row
    │   │       └── Special Parameters Rows
    │   │
    │   ├── Description Section
    │   │   ├── Function Description
    │   │   └── Working Principle
    │   │
    │   ├── Multimedia Section
    │   │   ├── 3D Model Preview
    │   │   ├── Cross-section Button
    │   │   ├── Animation Play Button
    │   │   └── Video Links
    │   │
    │   └── Knowledge Section
    │       ├── Related Knowledge List
    │       └── Click to expand
    │
    └── Footer (Horizontal Box)
        ├── Favorite Button
        └── Share Button
```

### Widget Variables 小部件变量

```
CurrentPartData (FRocketPartData)
- Data for displayed part

Is3DPreviewActive (Boolean)
- Default: false

CanBeMoved (Boolean)
- Default: true (for VR dragging)

CurrentLanguage (String)
- From game settings
```

### Widget Components 小部件组件

#### Title Bar 标题栏
```
Height: 80px
Background: Dark gradient

Part Icon:
- Size: 60x60
- Displays part's icon image

Part Name:
- Font Size: 32
- Format: "{PartNameCN}\n{PartNameEN}"
- Example: "主发动机喷嘴\nMain Engine Nozzle"

Close Button:
- Icon: ❌
- Size: 50x50
- Position: Right-aligned
- Click: Close panel with fade animation
```

#### Parameters Section 参数区域
```
Component: Data Table-like layout
Background: Slightly lighter than main panel

Table Structure:
┌────────────────┬─────────────────────┐
│ Dimension      │ 2.5m × 2.5m × 4.0m │
│ 尺寸           │                     │
├────────────────┼─────────────────────┤
│ Mass           │ 1,200 kg            │
│ 质量           │                     │
├────────────────┼─────────────────────┤
│ Thrust         │ 450 kN              │
│ 推力           │                     │
└────────────────┴─────────────────────┘

Rows dynamically generated from PartData
Font Size: 18
Row Height: 40px
```

#### Description Section 描述区域
```
Component: Vertical Box with Text Blocks
Max Height: 300px (scrollable)

Function Description:
- Title: "功能描述 Function Description"
- Font Size: 20
- Text: Multi-line from PartData.EducationalContentCN/EN

Working Principle:
- Title: "工作原理 Working Principle"
- Font Size: 20
- Text: Multi-line from PartData.TechnicalDetails
```

#### 3D Model Preview 3D 模型预览
```
Component: Image (rendered from SceneCapture2D)
Size: 400x400
Background: Dark with grid

Features:
- Rotatable with mouse/touch
- Zoom in/out
- Reset view button

Implementation:
1. Spawn part actor in isolated scene
2. Setup SceneCapture2D camera
3. Render to render target
4. Display in Image widget
5. Handle input for rotation:
   - Mouse drag: Rotate model
   - Scroll: Zoom
```

#### Multimedia Buttons 多媒体按钮
```
Cross-section Button:
- Icon: 📐
- Text: "剖面图 Cross-section"
- Click: Toggle mesh section view

Animation Button:
- Icon: ▶️
- Text: "工作动画 Working Animation"
- Click: Play part's animation timeline

Video Button:
- Icon: 🎬
- Text: "相关视频 Related Videos"
- Click: Open video player or external link
```

#### Knowledge Section 科普知识区域
```
Component: Expandable list

List Item Format:
┌─────────────────────────────────────┐
│ ▶ 火箭发动机工作原理                │
│   Rocket Engine Working Principle   │
└─────────────────────────────────────┘

Click to expand:
┌─────────────────────────────────────┐
│ ▼ 火箭发动机工作原理                │
│   Rocket Engine Working Principle   │
│                                     │
│   火箭发动机通过燃烧燃料产生...     │
│   Rocket engines generate thrust... │
│                                     │
│   [查看详情 View Details]           │
└─────────────────────────────────────┘
```

### Functions 函数

#### Event Construct
```
Implementation:
1. Set default visibility (hidden)
2. Setup close button event
3. Initialize 3D preview (if enabled)
```

#### ShowPartInfo(PartData: FRocketPartData)
```
Parameters:
- PartData: FRocketPartData

Implementation:
1. Set CurrentPartData = PartData
2. Update all UI elements:
   - Title (part name)
   - Icon
   - Parameters table
   - Description text
   - Load 3D model for preview
3. Load related knowledge points from DT_RocketKnowledge
4. Animate panel appearance (slide in from right)
5. Set visibility to visible
6. Play sound: UI_Panel_Open
```

#### ClosePanel()
```
Implementation:
1. Animate panel disappearance (slide out)
2. Clear 3D preview (destroy spawned actor)
3. After animation:
   - Set visibility to hidden
   - Broadcast OnPanelClosed event
4. Play sound: UI_Panel_Close
```

#### On3DPreviewRotate(Delta: Vector2D)
```
Parameters:
- Delta: Vector2D (mouse/touch drag delta)

Implementation:
1. If not Is3DPreviewActive, return
2. Rotate preview model:
   - Yaw: Delta.X * RotationSpeed
   - Pitch: Delta.Y * RotationSpeed
3. Update SceneCapture2D
```

#### OnCrossSectionToggled()
```
Implementation:
1. Toggle mesh section planes
2. Update preview
3. Update button text ("Show Full" / "Show Section")
```

#### OnKnowledgeItemClicked(KnowledgeID: FName)
```
Parameters:
- KnowledgeID: FName

Implementation:
1. Get knowledge data from DT_RocketKnowledge
2. Open WBP_KnowledgeLibrary widget
3. Navigate to specific knowledge item
```

---

## 4. WBP_ScoreReport - 成绩报告界面

### Purpose 目的
Displays detailed score breakdown and statistics after completing assembly.
显示完成装配后的详细分数分解和统计信息。

### Location 位置
`Content/RocketAssembly/UI/WBP_ScoreReport.uasset`

### Widget Type
User Widget (Full-screen overlay)

### Layout Structure 布局结构

```
Canvas Panel (Root)
└── Border (Main Panel)
    ├── Header Section
    │   ├── Total Score (Large)
    │   ├── Grade Badge
    │   └── Star Rating
    │
    ├── Score Breakdown (Vertical Box)
    │   ├── Accuracy Score Row
    │   ├── Sequence Score Row
    │   ├── Time Score Row
    │   └── Error Score Row
    │
    ├── Statistics Section
    │   ├── Total Time
    │   ├── Average Step Time
    │   ├── Error Count
    │   ├── Retry Count
    │   └── Knowledge Views
    │
    ├── Error Analysis Section
    │   └── Scroll Box
    │       └── Error Item List
    │
    ├── Achievements Section
    │   └── Horizontal Box
    │       └── Achievement Icons
    │
    └── Footer Buttons
        ├── Restart Button
        ├── Certificate Button
        ├── Share Button
        └── Main Menu Button
```

### Widget Variables 小部件变量

```
AssessmentData (FAssessmentReport)
- Complete assessment report

TotalScore (Float)
- Final score (0-100)

Grade (String)
- "Excellent", "Good", "Pass", "Fail"

StarCount (Integer)
- 1-5 stars based on score

UnlockedAchievements (Array<FName>)
- List of achieved IDs

ShowCelebration (Boolean)
- Trigger confetti if high score
```

### Widget Components 小部件组件

#### Total Score Display 总分展示
```
Component: Text Block
Font Size: 120
Color: Dynamic based on score
- >= 90: Gold
- >= 75: Silver
- >= 60: Bronze
- < 60: Gray

Format: "{Score}"
Example: "87"

Animation: Count up from 0 with easing
Duration: 2 seconds
Sound: Score counting tick
```

#### Grade Badge 评级标识
```
Component: Image + Text
Size: 200x200
Position: Next to total score

Grades:
- Excellent (优秀): Gold star badge
- Good (良好): Silver badge
- Pass (及格): Bronze badge
- Fail (不及格): Gray badge

Animation: Appear with scale up + rotation
```

#### Star Rating 星级评价
```
Component: Horizontal Box of Star Images
Star Count: 1-5

Mapping:
- 95-100: ⭐⭐⭐⭐⭐
- 85-94:  ⭐⭐⭐⭐
- 75-84:  ⭐⭐⭐
- 60-74:  ⭐⭐
- < 60:   ⭐

Animation: Stars appear one by one with sound
```

#### Score Breakdown Rows 详细评分行
```
Each Row Structure:
┌────────────────────────────────────────────┐
│ Accuracy Score  装配正确率                  │
│ ████████████████████░░  32 / 40           │
└────────────────────────────────────────────┘

Components per row:
- Label (Text): Score category name
- Progress Bar: Visual representation
- Score Text: "X / Y"

Accuracy Score:
- Label: "Accuracy Score 装配正确率"
- Max: 40 points
- Color: Green

Sequence Score:
- Label: "Sequence Score 装配顺序"
- Max: 30 points
- Color: Blue

Time Score:
- Label: "Time Score 完成时间"
- Max: 15 points
- Color: Yellow

Error Score:
- Label: "Error Score 失误次数"
- Max: 15 points
- Color: Orange

Animation: Bars fill sequentially with delay
```

#### Statistics Section 统计数据
```
Component: Grid Panel (2 columns)

Format:
┌──────────────────┬────────────┐
│ Total Time       │ 08:45      │
│ 总耗时           │            │
├──────────────────┼────────────┤
│ Avg Step Time    │ 52.5s      │
│ 平均每步耗时     │            │
├──────────────────┼────────────┤
│ Errors           │ 3          │
│ 错误次数         │            │
├──────────────────┼────────────┤
│ Retries          │ 1          │
│ 重试次数         │            │
├──────────────────┼────────────┤
│ Knowledge Views  │ 5          │
│ 查看知识点       │            │
└──────────────────┴────────────┘

Font Size: 20
Cell Padding: 10px
```

#### Error Analysis 错误分析
```
Component: Scroll Box with Error Items

Error Item Structure:
┌─────────────────────────────────────────┐
│ ❌ Step 3: Wrong Part Used             │
│                                         │
│ Expected: Fuel Tank                     │
│ Used: LOX Tank                          │
│                                         │
│ Suggestion: Check part labels carefully │
└─────────────────────────────────────────┘

Shows top 5 errors
Each item:
- Step number
- Error type
- What went wrong
- Suggestion for improvement

If no errors:
Display: "🎉 Perfect! No errors made!"
```

#### Achievements Section 成就解锁
```
Component: Wrap Box of Achievement Icons

Achievement Icon:
- Size: 100x100
- Image: Achievement badge
- Tooltip: Achievement name + description

States:
- Unlocked: Full color, glow effect
- Locked (shown): Grayscale, semi-transparent

Animation:
- Newly unlocked: Fly in + scale + rotate
- Sound: Achievement unlock jingle

Show only newly unlocked + next 3 locked
```

#### Footer Buttons 操作按钮
```
Restart Button:
- Text: "重新开始 Restart"
- Size: 200x60
- Color: Blue
- Click: Confirm → GameMode.RestartGame()

Certificate Button:
- Text: "查看证书 View Certificate"
- Size: 200x60
- Color: Gold
- Enabled: If score >= 60
- Click: Open WBP_Certificate

Share Button:
- Text: "分享成绩 Share"
- Size: 200x60
- Color: Green
- Click: Screenshot + Share options

Main Menu Button:
- Text: "返回主菜单 Main Menu"
- Size: 200x60
- Color: Gray
- Click: Confirm → GameMode.ExitToMainMenu()
```

### Functions 函数

#### ShowReport(ReportData: FAssessmentReport)
```
Parameters:
- ReportData: FAssessmentReport

Implementation:
1. Store AssessmentData = ReportData
2. Extract scores:
   - TotalScore = ReportData.TotalScore
   - Grade = ReportData.Grade
3. Calculate star count
4. Start animation sequence:
   a. Fade in panel (0.5s)
   b. Count up total score (2s)
   c. Show grade badge (0.5s)
   d. Reveal stars one by one (0.3s each)
   e. Fill score breakdown bars (1s each)
   f. Show statistics (instant)
   g. Show error analysis (instant)
   h. Animate achievements (0.5s each)
5. Play background music: Success theme
6. If TotalScore >= 90:
   - Trigger celebration VFX (confetti)
```

#### OnRestartClicked()
```
Implementation:
1. Show confirmation dialog:
   - "Restart? Current progress will be lost."
2. If confirmed:
   - Close report
   - Call GameMode.RestartGame()
```

#### OnCertificateClicked()
```
Implementation:
1. Create WBP_Certificate widget
2. Pass score and grade data
3. Add to viewport
4. Close report (or keep in background)
```

#### OnShareClicked()
```
Implementation:
1. Take screenshot of report
2. Show share options:
   - Save to file
   - Copy to clipboard
   - (Future: Social media integration)
3. Show confirmation message
```

---

## 5. WBP_Certificate - 虚拟证书界面

### Purpose 目的
Generates a certificate of completion with score and grade.
生成带有分数和成绩的完成证书。

### Location 位置
`Content/RocketAssembly/UI/WBP_Certificate.uasset`

### Layout Structure 布局结构

```
Canvas Panel (Root)
└── Border (Certificate)
    ├── Background (Decorative Image)
    ├── Border Frame (Ornate edge)
    ├── Title (Text)
    ├── Recipient Name (Editable Text)
    ├── Achievement Text (Text)
    ├── Score and Grade (Text)
    ├── Completion Date (Text)
    ├── Certificate ID (Text)
    ├── Seal/Badge (Image)
    └── Buttons (Horizontal Box)
        ├── Save Screenshot
        ├── Share
        └── Close
```

### Implementation Details

```
Background:
- Image: Certificate parchment texture
- Size: 1200x900
- Color: Off-white/cream

Border Frame:
- Image: Ornate border
- Color: Gold/Blue

Title:
- Text: "火箭装配技能认证证书"
        "Rocket Assembly Skill Certificate"
- Font: Formal/serif font
- Size: 48
- Color: Dark blue

Recipient Name:
- Component: Editable Text Box
- Placeholder: "Enter your name / 输入姓名"
- Font Size: 36
- Centered

Achievement Text:
- Text: "特此证明，{Name}已成功完成火箭装配VR教育课程"
        "This certifies that {Name} has successfully completed
         the Rocket Assembly VR Educational Course"
- Font Size: 24

Score Display:
- Text: "评级: {Grade}  分数: {Score}/100"
        "Grade: {Grade}  Score: {Score}/100"
- Font Size: 28

Date:
- Text: "完成日期 Date: {CurrentDate}"
- Font Size: 20

Certificate ID:
- Text: "证书编号 ID: {RandomID}"
- Font Size: 16
- RandomID: Generated UUID-like string
```

### Functions 函数

#### Event Construct
```
Implementation:
1. Load score and grade data
2. Generate certificate ID (random hash)
3. Get current date
4. Set default name from player profile (if available)
```

#### OnSaveScreenshotClicked()
```
Implementation:
1. Request high-resolution screenshot
2. Save to: Saved/Screenshots/Certificate_{Date}_{ID}.png
3. Show file save success message
```

---

## Additional UI Widgets 其他 UI 小部件

For brevity, here are abbreviated specifications for remaining widgets:

### 6. WBP_KnowledgeLibrary
- Tree view of knowledge categories
- Content viewer with text, images, videos
- Bookmark system
- Search functionality

### 7. WBP_Settings
- Audio sliders (Master, SFX, Voice, Music)
- Language toggle
- VR comfort settings
- Graphics quality presets
- Controls remapping

### 8. WBP_PartLabel (3D Widget)
- Small billboard text
- Shows part name only
- Fades with distance
- Minimal design

---

## Implementation Notes 实现注意事项

### Creating Widgets in Unreal

1. **Create Widget Blueprint**
   - Right-click in Content Browser
   - User Interface → Widget Blueprint
   - Name according to this guide

2. **Design Mode**
   - Drag components from Palette
   - Set properties in Details panel
   - Use anchors for responsive design

3. **Graph Mode**
   - Implement functions and events
   - Bind button clicks
   - Handle data binding

4. **Testing**
   - Use Widget Reflector for debugging
   - Test in VR Preview mode
   - Check performance with Stat UI

### Best Practices

- **Localization**: Use String Tables for all text
- **Performance**: Minimize Tick usage
- **VR Optimization**: Large fonts, high contrast
- **Accessibility**: Color-blind friendly colors
- **Caching**: Store widget references, don't create repeatedly
- **Animation**: Use Widget Animations, not Tick

---

## Testing Checklist 测试清单

### Functionality
- [ ] All buttons respond correctly
- [ ] Text updates with language change
- [ ] Data binds from Blueprint managers
- [ ] Animations play smoothly
- [ ] Sound effects trigger appropriately

### VR Experience
- [ ] Readable from 1-2 meters
- [ ] Comfortable viewing angle
- [ ] Ray pointer works accurately
- [ ] No performance drops
- [ ] Haptic feedback works

### Edge Cases
- [ ] Long text doesn't overflow
- [ ] Handle missing data gracefully
- [ ] Works with different resolutions
- [ ] Localization complete
- [ ] Save/load preserves state

---

**Implementation Order Recommendation**:
1. WBP_MainMenu (game entry point)
2. WBP_AssemblyHUD (core gameplay UI)
3. WBP_PartInfoPanel (information display)
4. WBP_ScoreReport (end game feedback)
5. WBP_Certificate (reward)
6. WBP_KnowledgeLibrary (educational content)
7. WBP_Settings (user preferences)
8. WBP_PartLabel (polish)

**实施顺序建议**：
1. WBP_MainMenu（游戏入口）
2. WBP_AssemblyHUD（核心游戏 UI）
3. WBP_PartInfoPanel（信息显示）
4. WBP_ScoreReport（游戏结束反馈）
5. WBP_Certificate（奖励）
6. WBP_KnowledgeLibrary（教育内容）
7. WBP_Settings（用户偏好）
8. WBP_PartLabel（润色）
