# Phase 2 - Blueprint Implementation Guide
# 火箭装配系统 Phase 2 蓝图实现指南

## 📋 Overview 概述

This guide provides step-by-step instructions for implementing all Blueprint systems for Phase 2 of the Rocket Assembly VR Educational System. Each Blueprint is described in detail with its purpose, variables, functions, and implementation logic.

本指南提供了火箭装配 VR 教育系统 Phase 2 所有蓝图系统的分步实现说明。详细描述了每个蓝图的目的、变量、函数和实现逻辑。

---

## 1. BP_RocketPartManager - 零件管理器

### Purpose 目的
Manages the lifecycle of all rocket parts including spawning, state tracking, inventory management, and save/load functionality.
管理所有火箭零件的生命周期，包括生成、状态跟踪、库存管理和保存/加载功能。

### Location 位置
`Content/RocketAssembly/Blueprints/BP_RocketPartManager.uasset`

### Parent Class 父类
Actor

### Components 组件
None (pure logic actor)

### Variables 变量

#### Part Data Management
```
PartDataTable (DataTable Reference)
- Type: Object Reference to DataTable
- Description: Reference to DT_RocketParts
- Category: Part Management
- Default: DT_RocketParts

SpawnedParts (Map)
- Type: Map<FName, Object Reference to AInteractiveRocketPart>
- Description: Maps part IDs to spawned part instances
- Category: Part Management

SpawnLocations (Map)
- Type: Map<FName, Transform>
- Description: Stores initial spawn locations for reset functionality
- Category: Part Management

AssembledParts (Array)
- Type: Array of Object Reference to AInteractiveRocketPart
- Description: List of currently assembled parts
- Category: Part Management

TotalPartsCount (Integer)
- Description: Total number of parts to assemble
- Category: Part Management
```

#### Events
```
OnPartSpawned (Event Dispatcher)
- Parameters: Part (AInteractiveRocketPart), PartID (FName)

OnPartGripped (Event Dispatcher)
- Parameters: Part (AInteractiveRocketPart), GrippingController (Actor)

OnPartAssembled (Event Dispatcher)
- Parameters: Part (AInteractiveRocketPart), Socket (UAssemblySocket)

OnAllPartsAssembled (Event Dispatcher)
- Parameters: None
```

### Functions 函数

#### BeginPlay
```
Implementation:
1. Get reference to PartDataTable
2. Validate data table exists
3. If auto-spawn enabled: Call SpawnAllParts()
4. Initialize empty containers (SpawnedParts, AssembledParts)
```

#### SpawnAllParts()
```
Return Type: Boolean
Implementation:
1. Get all row names from PartDataTable
2. For each row:
   a. Call SpawnPartByID(RowName)
   b. If spawn fails, log error
3. Return true if all parts spawned successfully

Notes:
- Use For Each loop on Data Table row names
- Store spawn location in SpawnLocations map
```

#### SpawnPartByID(PartID: FName) → AInteractiveRocketPart
```
Parameters:
- PartID: FName - The ID from data table

Return Type: Object Reference to AInteractiveRocketPart

Implementation:
1. Check if part already spawned (search SpawnedParts map)
   - If yes, return existing reference
2. Get row from PartDataTable using PartID
3. If row not found:
   - Log error
   - Return None
4. Get PartClass from row data (or use BP_RocketPart_Base)
5. Calculate spawn location:
   - Use SpawnLocationOffset for each part
   - Store in SpawnLocations map
6. Spawn actor of class PartClass at location
7. Set part's PartData from data table row
8. Add to SpawnedParts map
9. Bind to part's events:
   - OnGripped → HandlePartGripped
   - OnPartAssembled → HandlePartAssembled
10. Call OnPartSpawned event
11. Return spawned part reference
```

#### GetPartByID(PartID: FName) → AInteractiveRocketPart
```
Parameters:
- PartID: FName

Return Type: Object Reference to AInteractiveRocketPart

Implementation:
1. Find in SpawnedParts map using PartID
2. Return reference (or None if not found)
```

#### ResetPart(PartID: FName)
```
Parameters:
- PartID: FName

Implementation:
1. Get part reference using GetPartByID
2. If part is None, return
3. If part is assembled:
   a. Call part's DisassembleFromSocket()
   b. Remove from AssembledParts array
4. Get original location from SpawnLocations map
5. Set part's transform to original location
6. Set part's velocity to zero
7. Reset part's AssemblyState to Unassembled
```

#### ResetAllParts()
```
Implementation:
1. For each part in SpawnedParts:
   a. Call ResetPart(PartID)
2. Clear AssembledParts array
3. Reset any counters/scores
```

#### HandlePartGripped(Part: AInteractiveRocketPart, Controller: Actor)
```
Implementation:
1. Broadcast OnPartGripped event
2. Optional: Play haptic feedback on controller
3. Optional: Update UI to show part info
```

#### HandlePartAssembled(Part: AInteractiveRocketPart, Socket: UAssemblySocket)
```
Implementation:
1. Add part to AssembledParts array
2. Broadcast OnPartAssembled event
3. Check if all parts assembled:
   - If AssembledParts.Length == TotalPartsCount:
     - Broadcast OnAllPartsAssembled
```

#### SaveProgress() → Boolean
```
Return Type: Boolean

Implementation:
1. Create or get save game object (USaveGame)
2. For each part in SpawnedParts:
   a. Save PartID
   b. Save current Transform
   c. Save AssemblyState
   d. Save which socket it's attached to (if any)
3. Call SaveGameToSlot with slot name "RocketAssemblySave"
4. Return success/failure
```

#### LoadProgress() → Boolean
```
Return Type: Boolean

Implementation:
1. Check if save file exists using DoesSaveGameExist
2. If not exists, return false
3. Load save game from slot "RocketAssemblySave"
4. For each saved part data:
   a. Get or spawn part by ID
   b. Set transform to saved transform
   c. Set AssemblyState to saved state
   d. If was assembled, re-attach to socket
5. Return true
```

---

## 2. BP_AssemblyTutorialManager - 装配教学管理器

### Purpose 目的
Controls the tutorial flow, displays step-by-step guidance, AR visualization, and voice instructions.
控制教学流程，显示分步指导、AR 可视化和语音说明。

### Location 位置
`Content/RocketAssembly/Blueprints/BP_AssemblyTutorialManager.uasset`

### Parent Class 父类
Actor

### Variables 变量

```
StepsDataTable (DataTable Reference)
- Type: Object Reference to DataTable
- Description: Reference to DT_AssemblySteps
- Default: DT_AssemblySteps

CurrentStepIndex (Integer)
- Description: Current step number (0-based)
- Default: 0

TotalSteps (Integer)
- Description: Total number of steps
- Transient

StepHistory (Array<Integer>)
- Description: History of completed steps
- Category: Progress Tracking

ErrorCount (Integer)
- Description: Number of assembly errors made
- Default: 0

PartManagerRef (Object Reference)
- Type: BP_RocketPartManager
- Description: Reference to part manager
- Expose on Spawn: true

CurrentStepData (FAssemblyStepData)
- Description: Current step's data
- Transient

ARGuideArrow (Actor Reference)
- Type: BP_ARGuideArrow
- Description: Reference to AR guide arrow actor

PreviewPartActor (Actor Reference)
- Type: BP_PartPreview
- Description: Reference to hologram preview

VoiceAudioComponent (Audio Component)
- Description: For playing voice instructions

IsVoicePlaying (Boolean)
- Default: false

CurrentLanguage (String)
- Description: "CN" or "EN"
- Default: "CN"
```

### Events 事件

```
OnStepCompleted (Event Dispatcher)
- Parameters: StepIndex (Integer), StepData (FAssemblyStepData)

OnTutorialCompleted (Event Dispatcher)
- Parameters: TotalTime (Float), TotalErrors (Integer)

OnTutorialStarted (Event Dispatcher)
- Parameters: TotalSteps (Integer)
```

### Functions 函数

#### BeginPlay
```
Implementation:
1. Get reference to PartManagerRef
2. Bind to PartManager events:
   - OnPartAssembled → CheckStepCompletion
3. Get TotalSteps from StepsDataTable row count
4. Spawn AR guide actors (ARGuideArrow, PreviewPartActor)
5. Initially hide AR guides
```

#### StartTutorial()
```
Implementation:
1. Set CurrentStepIndex = 0
2. Clear StepHistory
3. Set ErrorCount = 0
4. Load first step using LoadStep(0)
5. Broadcast OnTutorialStarted
6. Show first step UI
```

#### LoadStep(StepIndex: Integer)
```
Parameters:
- StepIndex: Integer

Implementation:
1. Get row from StepsDataTable using StepIndex
2. Store in CurrentStepData
3. Update UI with step information:
   - Step number and description
   - Target part name
   - Instruction text
4. Call ShowStepHint()
5. Call HighlightTargetPart()
6. Call ShowARGuide()
7. Call PlayStepVoice()
```

#### NextStep()
```
Implementation:
1. If CurrentStepIndex >= TotalSteps - 1:
   - Call OnTutorialCompleted
   - Return
2. Increment CurrentStepIndex
3. Call LoadStep(CurrentStepIndex)
```

#### PreviousStep()
```
Implementation:
1. If CurrentStepIndex <= 0:
   - Return (already at first step)
2. Decrement CurrentStepIndex
3. Call LoadStep(CurrentStepIndex)
```

#### CheckStepCompletion(Part: AInteractiveRocketPart, Socket: UAssemblySocket)
```
Parameters:
- Part: AInteractiveRocketPart
- Socket: UAssemblySocket

Implementation:
1. Get expected part ID from CurrentStepData
2. Get expected socket ID from CurrentStepData
3. Check if Part.PartData.PartID matches expected:
   - If NO:
     - Increment ErrorCount
     - Show error message
     - Call OnAssemblyFailed with error reason
     - Return
4. Check if Socket.SocketID matches expected:
   - If NO:
     - Increment ErrorCount
     - Show error message
     - Return
5. If both match:
   - Add CurrentStepIndex to StepHistory
   - Broadcast OnStepCompleted
   - Hide AR guides
   - Show success feedback (VFX, audio)
   - Call NextStep() after 2 seconds delay
```

#### ShowStepHint()
```
Implementation:
1. Get hint text from CurrentStepData:
   - Use InstructionTextCN or InstructionTextEN based on language
2. Update HUD with hint text
3. Optional: Show 3D text in world space
```

#### HighlightTargetPart()
```
Implementation:
1. Get target PartID from CurrentStepData
2. Get part reference from PartManager.GetPartByID
3. If part exists:
   - Call part.EnableHighlight(true)
4. Optionally highlight target socket as well
```

#### ShowARGuide()
```
Implementation:
1. If ARGuideArrow exists:
   a. Get target part location
   b. Set arrow location to point to target part
   c. Set arrow visible
   d. Start arrow animation (pulse, float)
2. If PreviewPartActor exists:
   a. Get target socket location
   b. Set preview at socket location
   c. Set preview visible
   d. Set hologram material with correct mesh
```

#### HideARGuide()
```
Implementation:
1. If ARGuideArrow exists:
   - Set visibility to false
2. If PreviewPartActor exists:
   - Set visibility to false
```

#### PlayStepVoice()
```
Implementation:
1. If IsVoicePlaying:
   - Stop current voice
2. Get voice file path from CurrentStepData:
   - Use VoiceFileCN or VoiceFileEN based on language
3. Load sound wave from path
4. If sound loaded:
   - Play on VoiceAudioComponent
   - Set IsVoicePlaying = true
   - Bind to OnAudioFinished → Set IsVoicePlaying = false
5. Display subtitles synchronized with voice
```

---

## 3. BP_AssessmentManager - 评估管理器

### Purpose 目的
Tracks performance metrics, calculates scores, generates grade reports, and manages achievements.
跟踪性能指标、计算分数、生成成绩报告并管理成就。

### Location 位置
`Content/RocketAssembly/Blueprints/BP_AssessmentManager.uasset`

### Parent Class 父类
Actor

### Variables 变量

```
StartTime (Float)
- Description: Game start timestamp
- Default: 0.0

EndTime (Float)
- Description: Game end timestamp
- Default: 0.0

StepTimings (Map<Integer, Float>)
- Description: Time taken for each step

ErrorCount (Integer)
- Description: Total errors made
- Default: 0

RetryCount (Integer)
- Description: Number of retries
- Default: 0

KnowledgeViewCount (Integer)
- Description: Times knowledge base was viewed
- Default: 0

AssemblyAccuracyScore (Float)
- Description: Accuracy score (0-40)
- Default: 0.0

SequenceScore (Float)
- Description: Sequence score (0-30)
- Default: 0.0

TimeScore (Float)
- Description: Time score (0-15)
- Default: 0.0

ErrorScore (Float)
- Description: Error penalty score (0-15)
- Default: 0.0

TotalScore (Float)
- Description: Final total score (0-100)
- Default: 0.0

Grade (String)
- Description: "Excellent", "Good", "Pass", "Fail"
- Default: ""

ValidationResults (Array<FAssemblyValidationResult>)
- Description: All validation results for analysis

UnlockedAchievements (Array<FName>)
- Description: List of achievement IDs unlocked
```

### Functions 函数

#### BeginPlay
```
Implementation:
1. Initialize all counters to 0
2. Clear collections
```

#### StartAssessment()
```
Implementation:
1. Get current time using GetGameTimeInSeconds
2. Store in StartTime
3. Initialize all score variables to 0
4. Clear StepTimings map
5. Clear ValidationResults array
```

#### EndAssessment()
```
Implementation:
1. Get current time and store in EndTime
2. Calculate all scores:
   - Call CalculateAccuracyScore()
   - Call CalculateSequenceScore()
   - Call CalculateTimeScore()
   - Call CalculateErrorScore()
3. Call CalculateTotalScore()
4. Call DetermineGrade()
5. Call UnlockAchievements()
6. Call GenerateReport()
```

#### RecordStepTime(StepIndex: Integer, TimeSpent: Float)
```
Parameters:
- StepIndex: Integer
- TimeSpent: Float

Implementation:
1. Add or update entry in StepTimings map
2. Log the timing for debugging
```

#### RecordError()
```
Implementation:
1. Increment ErrorCount
2. Log error for analysis
```

#### RecordRetry()
```
Implementation:
1. Increment RetryCount
```

#### RecordKnowledgeView()
```
Implementation:
1. Increment KnowledgeViewCount
```

#### CalculateAccuracyScore() → Float
```
Return Type: Float (0-40)

Implementation:
1. If ValidationResults is empty:
   - Return 0
2. Calculate average precision from all ValidationResults:
   - Sum all ValidationResult.Score values
   - Divide by count
3. Scale to 0-40 range:
   - AccuracyScore = (AveragePrecision / 100) * 40
4. Store in AssemblyAccuracyScore
5. Return AssemblyAccuracyScore
```

#### CalculateSequenceScore() → Float
```
Return Type: Float (0-30)

Implementation:
1. Get expected sequence from data table
2. Compare with actual assembly order (from StepTimings)
3. Calculate percentage of correct order:
   - CorrectSequenceCount / TotalSteps
4. Scale to 0-30 range:
   - SequenceScore = CorrectPercentage * 30
5. Store in SequenceScore
6. Return SequenceScore
```

#### CalculateTimeScore() → Float
```
Return Type: Float (0-15)

Implementation:
1. Calculate total time:
   - TotalTime = EndTime - StartTime
2. Define time tiers:
   - Excellent: < 300s (5 min) → 15 points
   - Good: 300-600s → 12 points
   - Average: 600-900s → 9 points
   - Slow: 900-1200s → 6 points
   - Very Slow: > 1200s → 3 points
3. Assign score based on tier
4. Store in TimeScore
5. Return TimeScore
```

#### CalculateErrorScore() → Float
```
Return Type: Float (0-15)

Implementation:
1. Calculate error penalty:
   - 0 errors → 15 points
   - 1-2 errors → 12 points
   - 3-5 errors → 9 points
   - 6-9 errors → 6 points
   - 10+ errors → 3 points
2. Store in ErrorScore
3. Return ErrorScore
```

#### CalculateTotalScore() → Float
```
Return Type: Float (0-100)

Implementation:
1. Sum all component scores:
   - TotalScore = AssemblyAccuracyScore + SequenceScore + TimeScore + ErrorScore
2. Clamp to 0-100 range
3. Store in TotalScore
4. Return TotalScore
```

#### DetermineGrade() → String
```
Return Type: String

Implementation:
1. Based on TotalScore, assign grade:
   - >= 90: "Excellent" (优秀)
   - >= 75: "Good" (良好)
   - >= 60: "Pass" (及格)
   - < 60: "Fail" (不及格)
2. Store in Grade
3. Return Grade
```

#### GenerateReport() → FAssessmentReport (Struct)
```
Return Type: FAssessmentReport structure

Implementation:
1. Create structure containing:
   - TotalScore
   - Grade
   - Component scores (Accuracy, Sequence, Time, Error)
   - Total time
   - Error count
   - Average step time
   - StepTimings map
   - Improvement suggestions based on weak areas
2. Return the report structure
```

#### UnlockAchievements()
```
Implementation:
1. Check conditions for each achievement:
   - "First Launch": Complete first assembly (always unlock)
   - "Perfect Engineer": ErrorCount == 0
   - "Speed King": TotalTime < 600s
   - "Knowledge Seeker": KnowledgeViewCount >= 10
   - "Persistent": RetryCount >= 10 and succeeded
   - "Master Assembler": TotalScore >= 95 on hard difficulty
2. For each achieved:
   - Add to UnlockedAchievements array
   - Show achievement notification
   - Save to persistent data
```

---

## 4. BP_RocketAssemblyGameMode - 游戏模式控制器

### Purpose 目的
Manages game modes (Free, Tutorial, Assessment, Knowledge), coordinates all systems, handles game flow.
管理游戏模式（自由、教学、考核、知识浏览），协调所有系统，处理游戏流程。

### Location 位置
`Content/RocketAssembly/Blueprints/BP_RocketAssemblyGameMode.uasset`

### Parent Class 父类
GameModeBase

### Variables 变量

```
CurrentMode (Enum: EGameMode)
- Values: Free, Tutorial, Assessment, Knowledge
- Default: Free

PartManagerRef (Actor Reference)
- Type: BP_RocketPartManager

TutorialManagerRef (Actor Reference)
- Type: BP_AssemblyTutorialManager

AssessmentManagerRef (Actor Reference)
- Type: BP_AssessmentManager

IsPaused (Boolean)
- Default: false

MainMenuWidgetRef (User Widget)
- Type: WBP_MainMenu

HUDWidgetRef (User Widget)
- Type: WBP_AssemblyHUD

CurrentDifficulty (Enum: EDifficulty)
- Values: Simple, Standard, Professional
- Default: Standard
```

### Functions 函数

#### BeginPlay
```
Implementation:
1. Call InitializeGame()
2. Show main menu or auto-start based on config
```

#### InitializeGame()
```
Implementation:
1. Spawn PartManager if not exists
2. Spawn TutorialManager if not exists
3. Spawn AssessmentManager if not exists
4. Create and cache widget references:
   - MainMenu
   - HUD
   - InfoPanel
   - ScoreReport
5. Bind to manager events:
   - PartManager.OnAllPartsAssembled → OnGameCompleted
   - TutorialManager.OnTutorialCompleted → OnGameCompleted
6. Hide all UI initially
```

#### SetGameMode(Mode: EGameMode)
```
Parameters:
- Mode: EGameMode (Free, Tutorial, Assessment, Knowledge)

Implementation:
1. Store previous mode
2. Set CurrentMode = Mode
3. Hide all mode-specific UI
4. Based on Mode:
   
   Free Mode:
   - Disable tutorial manager
   - Disable assessment manager
   - Enable free part spawning
   - Show basic HUD only
   
   Tutorial Mode:
   - Enable tutorial manager
   - Enable assessment manager (for tracking)
   - Start tutorial flow
   - Show HUD with step guidance
   
   Assessment Mode:
   - Disable tutorial hints
   - Enable assessment manager
   - Start timer
   - Show HUD with timer and score
   - Disable retry functionality
   
   Knowledge Mode:
   - Disable part interaction
   - Show knowledge library UI
   - Enable part info viewing only
4. Call appropriate manager start functions
```

#### StartGame()
```
Implementation:
1. Based on CurrentMode:
   - Free: Spawn all parts, show HUD
   - Tutorial: Start tutorial flow
   - Assessment: Start assessment timer
   - Knowledge: Open knowledge library
2. Hide main menu
3. Show appropriate HUD
4. Unpause game
```

#### PauseGame()
```
Implementation:
1. Set IsPaused = true
2. Set game paused (affects time dilation)
3. Show pause menu overlay
4. Pause audio
```

#### ResumeGame()
```
Implementation:
1. Set IsPaused = false
2. Unpause game
3. Hide pause menu
4. Resume audio
```

#### RestartGame()
```
Implementation:
1. Call PartManager.ResetAllParts()
2. If Tutorial mode:
   - Call TutorialManager.StartTutorial()
3. If Assessment mode:
   - Call AssessmentManager.StartAssessment()
4. Reset HUD displays
5. Resume game
```

#### ExitToMainMenu()
```
Implementation:
1. Save progress (if applicable)
2. Destroy all spawned parts
3. Reset all managers
4. Hide HUD
5. Show main menu
6. Reset CurrentMode to Free
```

#### OnGameCompleted()
```
Implementation:
1. Based on CurrentMode:
   
   Free Mode:
   - Show completion message
   - Offer to restart or exit
   
   Tutorial Mode:
   - Stop tutorial
   - Show tutorial complete message
   - Transition to score report
   
   Assessment Mode:
   - End assessment
   - Calculate scores
   - Show detailed score report
   - Offer certificate if score >= 60
   
   Knowledge Mode:
   - N/A (no completion)
2. Optional: Trigger celebration effects
3. Save progress
```

---

## 5. BP_SocketIndicator - 装配点指示器

### Purpose 目的
Visual actor that shows assembly socket locations with dynamic colors and effects.
显示装配点位置的可视化 Actor，具有动态颜色和效果。

### Location 位置
`Content/RocketAssembly/Blueprints/BP_SocketIndicator.uasset`

### Parent Class 父类
Actor

### Components 组件

```
SceneRoot (Scene Component)
- Root component

IndicatorMesh (Static Mesh Component)
- Mesh: SM_Sphere or SM_Ring
- Material: M_SocketIndicator
- Scale: (0.3, 0.3, 0.3)

ParticleEffect (Niagara Component)
- System: NS_SocketIndicator
- Auto activate: true

ArrowComponent (Arrow Component)
- For directional indication
- Hidden in game by default
```

### Variables 变量

```
IndicatorState (Enum: EIndicatorState)
- Values: Idle, Ready, Occupied, Error
- Default: Idle

IdleColor (Linear Color)
- Default: Yellow (1, 1, 0, 0.5)

ReadyColor (Linear Color)
- Default: Green (0, 1, 0, 0.8)

OccupiedColor (Linear Color)
- Default: Gray (0.5, 0.5, 0.5, 0.3)

ErrorColor (Linear Color)
- Default: Red (1, 0, 0, 0.8)

PulseSpeed (Float)
- Default: 2.0

PulseAmplitude (Float)
- Default: 0.2

DynamicMaterial (Material Instance Dynamic)
- Runtime material instance
```

### Functions 函数

#### BeginPlay
```
Implementation:
1. Create dynamic material instance from M_SocketIndicator
2. Apply to IndicatorMesh
3. Start pulse animation
4. Set initial state to Idle
```

#### Tick
```
Implementation:
1. If IndicatorState != Occupied:
   - Calculate pulse value using sine wave
   - Update material opacity parameter
   - Update scale slightly for visual feedback
```

#### SetIndicatorState(NewState: EIndicatorState)
```
Parameters:
- NewState: EIndicatorState

Implementation:
1. Set IndicatorState = NewState
2. Based on NewState:
   - Idle: Set color to IdleColor, normal pulse
   - Ready: Set color to ReadyColor, faster pulse
   - Occupied: Set color to OccupiedColor, no pulse, reduce scale
   - Error: Set color to ErrorColor, rapid blink
3. Update material parameters:
   - EmissiveColor
   - Opacity
4. Update particle effect:
   - Spawn rate based on state
   - Color based on state
```

---

## 6. BP_ARGuideArrow - AR 引导箭头

### Purpose 目的
3D arrow that points to the next part or assembly location during tutorial.
在教学过程中指向下一个零件或装配位置的 3D 箭头。

### Location 位置
`Content/RocketAssembly/Blueprints/BP_ARGuideArrow.uasset`

### Parent Class 父类
Actor

### Components 组件

```
SceneRoot (Scene Component)

ArrowMesh (Static Mesh Component)
- Mesh: SM_Arrow or custom arrow model
- Material: M_Hologram

GlowEffect (Niagara Component)
- Optional particle trail

AudioComponent (Audio Component)
- For attention-grabbing sound
```

### Variables 变量

```
TargetLocation (Vector)
- Where the arrow points to

TargetActor (Actor Reference)
- Optional: Auto-track an actor

HoverHeight (Float)
- Height above target
- Default: 50.0

RotationSpeed (Float)
- Rotation animation speed
- Default: 90.0 (degrees/sec)

BobSpeed (Float)
- Up-down animation speed
- Default: 2.0

BobAmplitude (Float)
- Up-down distance
- Default: 10.0

IsAnimating (Boolean)
- Default: true
```

### Functions 函数

#### BeginPlay
```
Implementation:
1. Set initial visibility to hidden
2. Set dynamic material with hologram effect
3. Start animation timeline if IsAnimating
```

#### Tick
```
Implementation:
1. If TargetActor is valid:
   - Update TargetLocation from TargetActor
2. Calculate direction from self to target
3. Set rotation to point at target
4. If IsAnimating:
   - Apply bobbing animation using sine wave
   - Apply rotation animation
5. Update glow effect direction
```

#### SetTarget(Location: Vector, Actor: Actor)
```
Parameters:
- Location: Vector (optional)
- Actor: Actor Reference (optional)

Implementation:
1. If Actor is valid:
   - Set TargetActor = Actor
   - TargetLocation will be updated in Tick
2. Else if Location is valid:
   - Set TargetLocation = Location
   - Clear TargetActor
3. Show arrow
4. Start animation
```

#### Hide()
```
Implementation:
1. Set visibility to false
2. Stop animation
3. Clear target
```

---

## 7. BP_PartPreview - 零件预览

### Purpose 目的
Shows a holographic preview of where a part should be placed during assembly.
在装配过程中显示零件应该放置位置的全息预览。

### Location 位置
`Content/RocketAssembly/Blueprints/BP_PartPreview.uasset`

### Parent Class 父类
Actor

### Components 组件

```
PreviewMesh (Static Mesh Component)
- Material: M_Hologram
- Collision: No Collision

ScanlineEffect (Niagara Component)
- Animated scanline effect
```

### Variables 变量

```
TargetPartMesh (Static Mesh)
- Mesh to preview

HologramMaterial (Material)
- M_Hologram reference

DynamicMaterial (Material Instance Dynamic)
- Runtime material

RotationSpeed (Float)
- Default: 30.0

ScanlineSpeed (Float)
- Default: 1.0
```

### Functions 函数

#### SetPreviewMesh(Mesh: Static Mesh, CorrectRotation: Rotator)
```
Parameters:
- Mesh: Static Mesh
- CorrectRotation: Rotator

Implementation:
1. Set PreviewMesh static mesh to Mesh
2. Create dynamic material instance
3. Set material parameters:
   - Opacity: 0.3
   - EmissiveColor: Cyan
   - ScanlineSpeed: ScanlineSpeed
4. Set rotation to CorrectRotation
5. Show preview
```

#### Tick
```
Implementation:
1. Apply slow rotation animation
2. Update scanline effect position
3. Optional: Pulse opacity slightly
```

---

## 8. BP_AudioManager - 音频管理器

### Purpose 目的
Centralized audio management for all sound effects, voice, and music in the game.
游戏中所有音效、语音和音乐的集中音频管理。

### Location 位置
`Content/RocketAssembly/Blueprints/BP_AudioManager.uasset`

### Parent Class 父类
Actor

### Variables 变量

```
MasterVolume (Float)
- Range: 0.0 - 1.0
- Default: 0.8

SFXVolume (Float)
- Range: 0.0 - 1.0
- Default: 0.7

VoiceVolume (Float)
- Range: 0.0 - 1.0
- Default: 1.0

MusicVolume (Float)
- Range: 0.0 - 1.0
- Default: 0.5

CurrentMusic (Audio Component)
- Background music component

VoiceQueue (Array<Sound Wave>)
- Queue for voice playback

IsVoicePlaying (Boolean)
- Default: false

SoundLibrary (Map<FName, Sound Wave>)
- Preloaded sound effects
```

### Functions 函数

#### BeginPlay
```
Implementation:
1. Load sound library from asset paths
2. Create audio components
3. Load volume settings from save game
4. Start background music if in menu
```

#### PlaySFX(SoundName: FName, Location: Vector, Volume: Float)
```
Parameters:
- SoundName: FName (e.g., "GrabMetal", "SnapSuccess")
- Location: Vector (for 3D sound, optional)
- Volume: Float (0-1, optional, default 1.0)

Implementation:
1. Get sound from SoundLibrary
2. If not found, log error and return
3. Calculate final volume: Volume * SFXVolume * MasterVolume
4. If Location is valid:
   - Play at location (3D spatial audio)
5. Else:
   - Play 2D sound
```

#### PlayVoice(SoundWave: Sound Wave, ShowSubtitles: Boolean)
```
Parameters:
- SoundWave: Sound Wave
- ShowSubtitles: Boolean (default true)

Implementation:
1. If IsVoicePlaying:
   - Add to VoiceQueue
   - Return
2. Set IsVoicePlaying = true
3. Calculate volume: VoiceVolume * MasterVolume
4. Play on dedicated voice audio component
5. If ShowSubtitles:
   - Display subtitle UI with text
6. Bind to OnAudioFinished:
   - Set IsVoicePlaying = false
   - Play next in queue if any
```

#### PlayMusic(MusicName: FName, FadeTime: Float)
```
Parameters:
- MusicName: FName
- FadeTime: Float (fade transition time, default 1.0)

Implementation:
1. If CurrentMusic is playing:
   - Fade out over FadeTime
2. Load new music sound
3. Fade in new music over FadeTime
4. Set volume: MusicVolume * MasterVolume
5. Set looping to true
```

#### SetMasterVolume(Volume: Float)
```
Parameters:
- Volume: Float (0-1)

Implementation:
1. Clamp Volume to 0-1
2. Set MasterVolume = Volume
3. Update all active audio components
4. Save to settings
```

#### SetSFXVolume, SetVoiceVolume, SetMusicVolume
```
Similar to SetMasterVolume, but for specific audio types
```

---

## Implementation Notes 实现注意事项

### Unreal Editor Workflow
1. Create Blueprint by right-clicking in Content Browser
2. Select "Blueprint Class"
3. Choose appropriate parent class
4. Name according to this guide
5. Open Blueprint editor
6. Add variables from Variables section
7. Add functions from Functions section
8. Implement logic in Event Graph

### Testing Strategy
1. Test each Blueprint individually first
2. Create test level with minimal setup
3. Use Print String nodes for debugging
4. Use breakpoints in Blueprint debugger
5. Verify events fire correctly
6. Test integration with other Blueprints

### Performance Considerations
- Minimize Tick usage (use timers where possible)
- Cache references in BeginPlay
- Use object pooling for frequently spawned actors
- Optimize material instances
- Profile with Unreal Insights

### Best Practices
- Comment complex logic sections
- Use descriptive variable names
- Organize functions into categories
- Handle edge cases (null checks)
- Validate data table references
- Log errors for debugging

---

**Next Steps**: 
1. Implement BP_RocketPartManager first (core system)
2. Create test level to verify part spawning
3. Implement remaining Blueprints in order
4. Integrate with UI system (see UI implementation guide)
5. Test complete workflow

**下一步**:
1. 首先实现 BP_RocketPartManager（核心系统）
2. 创建测试关卡验证零件生成
3. 按顺序实现其余蓝图
4. 与 UI 系统集成（参见 UI 实现指南）
5. 测试完整工作流程
