# Phase 2 - SaveGame System Implementation Guide
# 火箭装配系统 Phase 2 存档系统实现指南

## 📋 Overview 概述

This guide provides implementation details for the save/load system that preserves player progress, settings, and statistics.

本指南提供存档/加载系统的实现细节，用于保存玩家进度、设置和统计数据。

---

## C++ SaveGame Class - RocketAssemblySaveGame

### File Location 文件位置
- Header: `Source/VRExpPluginExample/RocketAssembly/RocketAssemblySaveGame.h`
- Implementation: `Source/VRExpPluginExample/RocketAssembly/RocketAssemblySaveGame.cpp`

### Purpose 目的
Stores all persistent game data including assembly progress, settings, and player statistics.
存储所有持久化游戏数据，包括装配进度、设置和玩家统计。

---

## Header File - RocketAssemblySaveGame.h

```cpp
// Copyright VRExpPluginExample. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RocketPartData.h"
#include "RocketAssemblySaveGame.generated.h"

/**
 * 零件保存数据
 * Saved Part Data
 */
USTRUCT(BlueprintType)
struct FSavedPartData
{
	GENERATED_BODY()

	// 零件ID - Part ID
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FName PartID;

	// 当前位置 - Current Transform
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FTransform CurrentTransform;

	// 装配状态 - Assembly State
	UPROPERTY(SaveGame, BlueprintReadWrite)
	EAssemblyState AssemblyState;

	// 装配到的SocketID（如果已装配）- Socket ID (if assembled)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FName AttachedSocketID;

	// 是否被锁定 - Is Locked
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bIsLocked = false;

	FSavedPartData()
		: PartID(NAME_None)
		, CurrentTransform(FTransform::Identity)
		, AssemblyState(EAssemblyState::Unassembled)
		, AttachedSocketID(NAME_None)
		, bIsLocked(false)
	{
	}
};

/**
 * 玩家统计数据
 * Player Statistics
 */
USTRUCT(BlueprintType)
struct FPlayerStatistics
{
	GENERATED_BODY()

	// 总游戏时长（秒）- Total Play Time (seconds)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	float TotalPlayTime = 0.0f;

	// 完成次数 - Times Completed
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 CompletionCount = 0;

	// 最高分数 - Best Score
	UPROPERTY(SaveGame, BlueprintReadWrite)
	float BestScore = 0.0f;

	// 最快完成时间（秒）- Fastest Completion Time (seconds)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	float FastestTime = 0.0f;

	// 总错误次数 - Total Errors
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 TotalErrors = 0;

	// 查看知识点总次数 - Total Knowledge Views
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 TotalKnowledgeViews = 0;
};

/**
 * 游戏设置
 * Game Settings
 */
USTRUCT(BlueprintType)
struct FGameSettings
{
	GENERATED_BODY()

	// 语言设置 - Language Setting
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString Language = TEXT("CN"); // "CN" or "EN"

	// 主音量 - Master Volume (0-1)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	float MasterVolume = 0.8f;

	// 音效音量 - SFX Volume (0-1)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	float SFXVolume = 0.7f;

	// 语音音量 - Voice Volume (0-1)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	float VoiceVolume = 1.0f;

	// 音乐音量 - Music Volume (0-1)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	float MusicVolume = 0.5f;

	// 左右手设置 - Handedness
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bIsLeftHanded = false;

	// 移动方式 - Movement Type (false = Teleport, true = Smooth)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bUseSmoothLocomotion = false;

	// 视野渐变（防眩晕）- Vignette (anti-nausea)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bEnableVignette = true;

	// 字幕开关 - Subtitles Enabled
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bEnableSubtitles = true;

	// 图形质量 - Graphics Quality (0=Low, 1=Medium, 2=High, 3=Ultra)
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 GraphicsQuality = 2;
};

/**
 * 火箭装配存档类
 * Rocket Assembly Save Game Class
 */
UCLASS()
class VREXPPLUGINEXAMPLE_API URocketAssemblySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	URocketAssemblySaveGame();

public:
	// ========== 存档元数据 Save Metadata ==========

	// 存档槽名称 - Save Slot Name
	UPROPERTY(VisibleAnywhere, Category = "Save Metadata")
	FString SaveSlotName;

	// 玩家索引 - Player Index
	UPROPERTY(VisibleAnywhere, Category = "Save Metadata")
	uint32 PlayerIndex;

	// 存档时间戳 - Save Timestamp
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save Metadata")
	FDateTime SaveTimestamp;

	// 游戏版本 - Game Version
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save Metadata")
	FString GameVersion;

	// ========== 游戏进度 Game Progress ==========

	// 当前游戏模式 - Current Game Mode
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Game Progress")
	FString CurrentGameMode; // "Free", "Tutorial", "Assessment", "Knowledge"

	// 当前难度 - Current Difficulty
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Game Progress")
	FString CurrentDifficulty; // "Simple", "Standard", "Professional"

	// 当前步骤索引（教学模式）- Current Step Index (Tutorial Mode)
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Game Progress")
	int32 CurrentStepIndex = 0;

	// 已完成步骤列表 - Completed Steps List
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Game Progress")
	TArray<int32> CompletedSteps;

	// 零件数据列表 - Parts Data List
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Game Progress")
	TArray<FSavedPartData> SavedParts;

	// 是否有活跃游戏 - Has Active Game
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Game Progress")
	bool bHasActiveGame = false;

	// 游戏开始时间戳 - Game Start Timestamp
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Game Progress")
	FDateTime GameStartTime;

	// ========== 成就系统 Achievement System ==========

	// 已解锁成就列表 - Unlocked Achievements
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Achievements")
	TArray<FName> UnlockedAchievements;

	// ========== 玩家统计 Player Statistics ==========

	// 统计数据 - Statistics
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Statistics")
	FPlayerStatistics Statistics;

	// ========== 游戏设置 Game Settings ==========

	// 设置 - Settings
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Settings")
	FGameSettings Settings;

	// ========== 历史记录 History ==========

	// 历史最高分记录 - High Score History (last 10)
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "History")
	TArray<float> ScoreHistory;

	// 历史完成时间记录 - Completion Time History (last 10)
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "History")
	TArray<float> TimeHistory;

public:
	// ========== 辅助函数 Helper Functions ==========

	// 更新存档时间戳 - Update Save Timestamp
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void UpdateTimestamp();

	// 清除游戏进度 - Clear Game Progress
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void ClearGameProgress();

	// 添加成就 - Add Achievement
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	bool AddAchievement(FName AchievementID);

	// 检查成就是否解锁 - Check if Achievement is Unlocked
	UFUNCTION(BlueprintPure, Category = "Save Game")
	bool IsAchievementUnlocked(FName AchievementID) const;

	// 添加分数到历史 - Add Score to History
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void AddScoreToHistory(float Score);

	// 添加时间到历史 - Add Time to History
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void AddTimeToHistory(float Time);

	// 获取最高分 - Get Best Score
	UFUNCTION(BlueprintPure, Category = "Save Game")
	float GetBestScore() const { return Statistics.BestScore; }

	// 获取最快时间 - Get Fastest Time
	UFUNCTION(BlueprintPure, Category = "Save Game")
	float GetFastestTime() const { return Statistics.FastestTime; }
};
```

---

## Implementation File - RocketAssemblySaveGame.cpp

```cpp
// Copyright VRExpPluginExample. All Rights Reserved.

#include "RocketAssemblySaveGame.h"

URocketAssemblySaveGame::URocketAssemblySaveGame()
{
	// 默认存档槽名称 - Default Save Slot Name
	SaveSlotName = TEXT("RocketAssemblySaveSlot");
	PlayerIndex = 0;

	// 设置游戏版本 - Set Game Version
	GameVersion = TEXT("1.0.0");

	// 初始化时间戳 - Initialize Timestamp
	UpdateTimestamp();
}

void URocketAssemblySaveGame::UpdateTimestamp()
{
	SaveTimestamp = FDateTime::Now();
}

void URocketAssemblySaveGame::ClearGameProgress()
{
	// 清除所有游戏进度数据 - Clear All Game Progress Data
	CurrentGameMode = TEXT("Free");
	CurrentDifficulty = TEXT("Standard");
	CurrentStepIndex = 0;
	CompletedSteps.Empty();
	SavedParts.Empty();
	bHasActiveGame = false;
	GameStartTime = FDateTime::MinValue();

	UpdateTimestamp();

	UE_LOG(LogTemp, Log, TEXT("Game progress cleared"));
}

bool URocketAssemblySaveGame::AddAchievement(FName AchievementID)
{
	// 检查是否已存在 - Check if Already Exists
	if (UnlockedAchievements.Contains(AchievementID))
	{
		return false; // 已解锁 - Already Unlocked
	}

	// 添加到列表 - Add to List
	UnlockedAchievements.Add(AchievementID);

	UpdateTimestamp();

	UE_LOG(LogTemp, Log, TEXT("Achievement unlocked: %s"), *AchievementID.ToString());

	return true;
}

bool URocketAssemblySaveGame::IsAchievementUnlocked(FName AchievementID) const
{
	return UnlockedAchievements.Contains(AchievementID);
}

void URocketAssemblySaveGame::AddScoreToHistory(float Score)
{
	// 添加到历史 - Add to History
	ScoreHistory.Add(Score);

	// 保持最多10条记录 - Keep Max 10 Records
	if (ScoreHistory.Num() > 10)
	{
		ScoreHistory.RemoveAt(0);
	}

	// 更新最高分 - Update Best Score
	if (Score > Statistics.BestScore)
	{
		Statistics.BestScore = Score;
	}

	UpdateTimestamp();
}

void URocketAssemblySaveGame::AddTimeToHistory(float Time)
{
	// 添加到历史 - Add to History
	TimeHistory.Add(Time);

	// 保持最多10条记录 - Keep Max 10 Records
	if (TimeHistory.Num() > 10)
	{
		TimeHistory.RemoveAt(0);
	}

	// 更新最快时间 - Update Fastest Time
	if (Statistics.FastestTime == 0.0f || Time < Statistics.FastestTime)
	{
		Statistics.FastestTime = Time;
	}

	UpdateTimestamp();
}
```

---

## Blueprint Integration - 蓝图集成

### SaveGame Manager (可在 BP_RocketPartManager 或 BP_GameMode 中实现)

#### Function: SaveGameProgress

```
Blueprint Implementation:

1. Get or Create Save Game Object:
   - Does Save Game Exist?
     → If YES: Load Game from Slot "RocketAssemblySaveSlot"
     → If NO: Create Save Game Object (Class: URocketAssemblySaveGame)

2. Set Save Game Data:
   - Set CurrentGameMode (from GameMode)
   - Set CurrentDifficulty (from GameMode)
   - Set CurrentStepIndex (from TutorialManager)
   - Set CompletedSteps (from TutorialManager)

3. Save Part Data:
   - For Each Part in SpawnedParts:
     a. Create FSavedPartData struct
     b. Set PartID = Part.PartData.PartID
     c. Set CurrentTransform = Part.GetActorTransform()
     d. Set AssemblyState = Part.AssemblyState
     e. If Part is assembled:
        - Set AttachedSocketID = Part.AttachedSocket.SocketID
     f. Set bIsLocked = Part.bIsLocked
     g. Add to SavedParts array

4. Set Metadata:
   - Call SaveGame.UpdateTimestamp()
   - Set bHasActiveGame = true

5. Save to Disk:
   - Save Game to Slot "RocketAssemblySaveSlot"
   - User Index: 0
   - Return Success/Failure

6. Show Confirmation:
   - If Success: Show "Progress Saved" message
   - If Failure: Log error
```

#### Function: LoadGameProgress

```
Blueprint Implementation:

1. Check if Save Exists:
   - Does Save Game Exist "RocketAssemblySaveSlot"?
   - If NO: Return false

2. Load Save Game:
   - Load Game from Slot "RocketAssemblySaveSlot"
   - User Index: 0
   - Cast to URocketAssemblySaveGame

3. Validate Save Data:
   - Check GameVersion compatibility
   - Check bHasActiveGame flag
   - If invalid: Return false

4. Restore Game State:
   - Set GameMode.CurrentMode = SaveGame.CurrentGameMode
   - Set GameMode.CurrentDifficulty = SaveGame.CurrentDifficulty
   - Set TutorialManager.CurrentStepIndex = SaveGame.CurrentStepIndex

5. Restore Parts:
   - For Each FSavedPartData in SaveGame.SavedParts:
     a. Spawn or Get Part by PartID
     b. Set Part Transform = SavedPartData.CurrentTransform
     c. Set Part.AssemblyState = SavedPartData.AssemblyState
     d. If SavedPartData.AttachedSocketID is valid:
        - Find Socket by ID
        - Call Part.AssembleToSocket(Socket)
     e. Set Part.bIsLocked = SavedPartData.bIsLocked

6. Restore UI State:
   - Update HUD to show current step
   - Update progress indicators

7. Return Success:
   - Show "Progress Loaded" message
   - Return true
```

#### Function: SaveSettings

```
Blueprint Implementation:

1. Get or Create Save Game Object

2. Update Settings:
   - SaveGame.Settings.Language = CurrentLanguage
   - SaveGame.Settings.MasterVolume = MasterVolume
   - SaveGame.Settings.SFXVolume = SFXVolume
   - SaveGame.Settings.VoiceVolume = VoiceVolume
   - SaveGame.Settings.MusicVolume = MusicVolume
   - SaveGame.Settings.bIsLeftHanded = bIsLeftHanded
   - SaveGame.Settings.bUseSmoothLocomotion = bUseSmoothLocomotion
   - SaveGame.Settings.bEnableVignette = bEnableVignette
   - SaveGame.Settings.bEnableSubtitles = bEnableSubtitles
   - SaveGame.Settings.GraphicsQuality = GraphicsQuality

3. Save to Disk:
   - Save Game to Slot "RocketAssemblySaveSlot"

4. Apply Settings:
   - Call AudioManager.SetMasterVolume(MasterVolume)
   - Update graphics settings
   - etc.
```

#### Function: LoadSettings

```
Blueprint Implementation:

1. Check if Save Exists:
   - If NO: Use default settings, return

2. Load Save Game

3. Apply Settings:
   - Language = SaveGame.Settings.Language
   - MasterVolume = SaveGame.Settings.MasterVolume
   - etc.

4. Update UI:
   - Update settings UI to reflect loaded values
   - Apply language to all text

5. Apply to Engine:
   - Set audio volumes
   - Set graphics quality
   - Configure VR settings
```

#### Function: SaveCompletionData

```
Blueprint Implementation:

1. Get or Create Save Game Object

2. Update Statistics:
   - Increment SaveGame.Statistics.CompletionCount
   - Add TotalPlayTime
   - Call SaveGame.AddScoreToHistory(FinalScore)
   - Call SaveGame.AddTimeToHistory(CompletionTime)

3. Update Achievements:
   - For Each Unlocked Achievement:
     - Call SaveGame.AddAchievement(AchievementID)

4. Clear Active Game:
   - Set SaveGame.bHasActiveGame = false

5. Save to Disk
```

---

## Usage Examples - 使用示例

### Example 1: Auto-Save on Step Completion

```
In BP_AssemblyTutorialManager:

Event OnStepCompleted:
1. Get GameMode reference
2. Call GameMode.SaveGameProgress()
3. Show brief "Saved" indicator
```

### Example 2: Continue from Last Session

```
In WBP_MainMenu:

On "Continue" Button Clicked:
1. Call GameMode.LoadGameProgress()
2. If load successful:
   - Close menu
   - Resume game
3. If load failed:
   - Show error message
   - Offer to start new game
```

### Example 3: Settings Persistence

```
In WBP_Settings:

On "Apply" Button Clicked:
1. Get all setting values from UI
2. Call GameMode.SaveSettings()
3. Show "Settings Saved" confirmation
4. Close settings panel
```

---

## Save File Location - 存档文件位置

### Windows
```
C:\Users\{Username}\AppData\Local\VRExpPluginExample\Saved\SaveGames\RocketAssemblySaveSlot.sav
```

### Android (Quest)
```
/sdcard/UE4Game/VRExpPluginExample/VRExpPluginExample/Saved/SaveGames/RocketAssemblySaveSlot.sav
```

---

## Best Practices - 最佳实践

### 1. Frequent Auto-Save
- Save after each step completion
- Save on pause
- Save before exiting

### 2. Error Handling
```cpp
if (!USaveGameSystem::Get()->SaveGameToSlot(SaveGameObject, SlotName, 0))
{
    UE_LOG(LogTemp, Error, TEXT("Failed to save game"));
    // Show error message to player
    // Try alternative save slot
}
```

### 3. Save Data Migration
- Include GameVersion field
- Check version on load
- Implement migration logic for old saves

### 4. Testing
- Test save/load in various states
- Test with corrupted save files
- Test with no save file
- Test cross-session persistence

### 5. Performance
- Save asynchronously if possible (UE5 async save)
- Don't save on every frame (Tick)
- Cache save game object, don't recreate constantly

---

## Troubleshooting - 故障排除

### Issue: Save file not found
**Solution**: 
- Ensure save directory exists
- Check platform-specific paths
- Verify write permissions

### Issue: Load returns old data
**Solution**:
- Ensure UpdateTimestamp() is called before save
- Check if correct slot name is used
- Verify save was successful

### Issue: Settings don't persist
**Solution**:
- Ensure SaveSettings() is called after changes
- Check if settings struct is properly saved
- Verify load happens in correct initialization phase

---

## Testing Checklist - 测试清单

### Save Functionality
- [ ] Save progress mid-game
- [ ] Save completes without errors
- [ ] Save file created in correct location
- [ ] Multiple save/load cycles work

### Load Functionality
- [ ] Load restores part positions
- [ ] Load restores part assembly states
- [ ] Load restores current step
- [ ] Load restores settings
- [ ] Load handles missing parts gracefully

### Settings Persistence
- [ ] Volume settings persist
- [ ] Language choice persists
- [ ] VR preferences persist
- [ ] Graphics settings persist

### Cross-Session
- [ ] Save persists after game restart
- [ ] Save persists after system restart
- [ ] Save works on different user accounts

### Edge Cases
- [ ] First-time launch (no save file)
- [ ] Corrupted save file handling
- [ ] Full disk handling
- [ ] Concurrent saves prevention

---

**Implementation Priority**:
1. Basic save/load (progress and parts)
2. Settings persistence
3. Statistics tracking
4. Achievement system
5. History tracking
6. Error handling and edge cases

**实施优先级**：
1. 基本保存/加载（进度和零件）
2. 设置持久化
3. 统计跟踪
4. 成就系统
5. 历史记录
6. 错误处理和边缘情况
