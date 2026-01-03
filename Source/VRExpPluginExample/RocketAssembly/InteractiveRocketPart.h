// Copyright VRExpPluginExample. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Grippables/GrippableStaticMeshActor.h"
#include "RocketPartData.h"
#include "InteractiveRocketPart.generated.h"

// Forward declarations
class UAssemblySocket;
class UMaterialInstanceDynamic;

/**
 * 可交互火箭零件 Actor
 * Interactive Rocket Part Actor
 * 
 * 继承自 VRExpansionPlugin 的 AGrippableStaticMeshActor
 * 添加火箭零件特定功能：高亮显示、装配状态管理、信息提示
 */
UCLASS(Blueprintable, ClassGroup = (RocketAssembly))
class VREXPPLUGINEXAMPLE_API AInteractiveRocketPart : public AGrippableStaticMeshActor
{
	GENERATED_BODY()

public:
	// 构造函数
	AInteractiveRocketPart(const FObjectInitializer& ObjectInitializer);

	// 开始游戏时调用
	virtual void BeginPlay() override;

	// 每帧调用
	virtual void Tick(float DeltaTime) override;

public:
	// ========== 零件数据 Part Data ==========
	
	// 零件数据引用 - Part Data Reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket Part")
	FRocketPartData PartData;

	// 当前装配状态 - Current Assembly State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rocket Part")
	EAssemblyState AssemblyState;

	// 是否被选中/悬停 - Is Hovered/Selected
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rocket Part")
	bool bIsHovered;

	// 是否可以被装配 - Can Be Assembled
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rocket Part")
	bool bCanBeAssembled;

	// 目标装配点 - Target Assembly Socket
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rocket Part")
	TObjectPtr<UAssemblySocket> TargetSocket;

public:
	// ========== 高亮与材质 Highlight & Materials ==========

	// 默认材质 - Default Material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UMaterialInterface> DefaultMaterial;

	// 高亮材质 - Highlight Material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UMaterialInterface> HighlightMaterial;

	// 可装配材质(绿色) - Can Assemble Material (Green)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UMaterialInterface> CanAssembleMaterial;

	// 错误材质(红色) - Error Material (Red)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UMaterialInterface> ErrorMaterial;

	// 动态材质实例 - Dynamic Material Instance
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

	// 高亮颜色 - Highlight Color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FLinearColor HighlightColor;

	// 可装配颜色 - Can Assemble Color (Green)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FLinearColor CanAssembleColor;

	// 错误颜色 - Error Color (Red)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FLinearColor ErrorColor;

	// 启用高亮显示 - Enable Highlight
	UFUNCTION(BlueprintCallable, Category = "Rocket Part|Visual")
	void EnableHighlight(bool bEnable);

	// 设置装配状态材质 - Set Assembly State Material
	UFUNCTION(BlueprintCallable, Category = "Rocket Part|Visual")
	void SetAssemblyStateMaterial(EAssemblyState NewState);

	// 显示信息标签 - Show Info Label
	UFUNCTION(BlueprintCallable, Category = "Rocket Part|Visual")
	void ShowInfoLabel(bool bShow);

public:
	// ========== 装配功能 Assembly Functions ==========

	// 设置装配状态 - Set Assembly State
	UFUNCTION(BlueprintCallable, Category = "Rocket Part|Assembly")
	void SetAssemblyState(EAssemblyState NewState);

	// 检查是否可以装配到目标点 - Check if can assemble to target socket
	UFUNCTION(BlueprintCallable, Category = "Rocket Part|Assembly")
	bool CanAssembleToSocket(UAssemblySocket* Socket);

	// 装配到目标点 - Assemble to target socket
	UFUNCTION(BlueprintCallable, Category = "Rocket Part|Assembly")
	bool AssembleToSocket(UAssemblySocket* Socket);

	// 从装配点卸下 - Disassemble from socket
	UFUNCTION(BlueprintCallable, Category = "Rocket Part|Assembly")
	void DisassembleFromSocket();

	// 锁定零件(防止误操作) - Lock part (prevent accidental manipulation)
	UFUNCTION(BlueprintCallable, Category = "Rocket Part|Assembly")
	void LockPart(bool bLock);

	// 检查装配依赖是否满足 - Check if assembly dependencies are met
	UFUNCTION(BlueprintCallable, Category = "Rocket Part|Assembly")
	bool AreDependenciesMet() const;

public:
	// ========== 事件 Events ==========

	// 零件被悬停时触发 - Triggered when part is hovered
	UPROPERTY(BlueprintAssignable, Category = "Rocket Part|Events")
	FVROnGripSignature OnPartHovered;

	// 零件悬停结束时触发 - Triggered when hover ends
	UPROPERTY(BlueprintAssignable, Category = "Rocket Part|Events")
	FVROnGripSignature OnPartUnhovered;

	// 零件装配完成时触发 - Triggered when part is assembled
	UPROPERTY(BlueprintAssignable, Category = "Rocket Part|Events")
	FVROnGripSignature OnPartAssembled;

	// 零件卸下时触发 - Triggered when part is disassembled
	UPROPERTY(BlueprintAssignable, Category = "Rocket Part|Events")
	FVROnGripSignature OnPartDisassembled;

	// 装配失败时触发 - Triggered when assembly fails
	UPROPERTY(BlueprintAssignable, Category = "Rocket Part|Events")
	FVROnGripSignature OnAssemblyFailed;

public:
	// ========== 辅助功能 Helper Functions ==========

	// 获取零件名称(本地化) - Get part name (localized)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rocket Part|Info")
	FText GetLocalizedPartName() const;

	// 获取零件描述(本地化) - Get part description (localized)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rocket Part|Info")
	FText GetLocalizedPartDescription() const;

	// 获取零件类型字符串 - Get part type string
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rocket Part|Info")
	FString GetPartTypeString() const;

	// 是否已装配 - Is assembled
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rocket Part|Info")
	bool IsAssembled() const { return AssemblyState == EAssemblyState::Assembled; }

	// 是否被锁定 - Is locked
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rocket Part|Info")
	bool IsLocked() const { return bIsLocked; }

protected:
	// ========== 内部变量 Internal Variables ==========

	// 是否被锁定 - Is locked
	UPROPERTY()
	bool bIsLocked;

	// 原始位置(装配前) - Original location (before assembly)
	UPROPERTY()
	FVector OriginalLocation;

	// 原始旋转(装配前) - Original rotation (before assembly)
	UPROPERTY()
	FRotator OriginalRotation;

protected:
	// ========== 内部函数 Internal Functions ==========

	// 初始化材质 - Initialize materials
	void InitializeMaterials();

	// 更新视觉效果 - Update visual effects
	void UpdateVisuals();

	// 播放装配音效 - Play assembly sound
	void PlayAssemblySound(bool bSuccess);

	// 播放特效 - Play visual effects
	void PlayAssemblyVFX(bool bSuccess);
};
