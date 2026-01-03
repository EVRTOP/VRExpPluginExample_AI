// Copyright VRExpPluginExample. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RocketPartData.h"
#include "AssemblySocket.generated.h"

// Forward declarations
class AInteractiveRocketPart;
class UStaticMeshComponent;

/**
 * 装配点组件
 * Assembly Socket Component
 * 
 * 定义火箭零件的装配位置和验证规则
 * 提供对齐检测、磁吸效果和装配验证功能
 */
UCLASS(Blueprintable, ClassGroup = (RocketAssembly), meta = (BlueprintSpawnableComponent))
class VREXPPLUGINEXAMPLE_API UAssemblySocket : public USceneComponent
{
	GENERATED_BODY()

public:
	// 构造函数
	UAssemblySocket();

	// 开始游戏时调用
	virtual void BeginPlay() override;

	// 每帧调用
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// ========== 装配点配置 Socket Configuration ==========

	// 装配点ID - Socket ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket")
	FName SocketID;

	// 接受的零件类型 - Accepted Part Types
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket")
	TArray<ERocketPartType> AcceptedPartTypes;

	// 接受的零件ID列表 - Accepted Part IDs (if empty, use type check)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket")
	TArray<FName> AcceptedPartIDs;

	// 位置对齐容差(cm) - Position Alignment Tolerance (cm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Alignment")
	float PositionTolerance;

	// 角度对齐容差(度) - Rotation Alignment Tolerance (degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Alignment")
	float RotationTolerance;

	// 磁吸距离(cm) - Snap Distance (cm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Alignment")
	float SnapDistance;

	// 磁吸强度 - Snap Strength (0-1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Alignment")
	float SnapStrength;

	// 是否启用磁吸 - Enable Snap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Alignment")
	bool bEnableSnap;

	// 是否显示可视化指示器 - Show Visual Indicator
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Visual")
	bool bShowIndicator;

	// 指示器网格 - Indicator Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Visual")
	TObjectPtr<UStaticMesh> IndicatorMesh;

	// 指示器颜色(空闲) - Indicator Color (Idle)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Visual")
	FLinearColor IdleColor;

	// 指示器颜色(可装配) - Indicator Color (Can Assemble)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Visual")
	FLinearColor CanAssembleColor;

	// 指示器颜色(已占用) - Indicator Color (Occupied)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly Socket|Visual")
	FLinearColor OccupiedColor;

	// 指示器网格组件 - Indicator Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assembly Socket|Visual")
	TObjectPtr<UStaticMeshComponent> IndicatorMeshComponent;

public:
	// ========== 装配状态 Assembly State ==========

	// 是否已被占用 - Is Occupied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assembly Socket|State")
	bool bIsOccupied;

	// 当前装配的零件 - Currently Assembled Part
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assembly Socket|State")
	TObjectPtr<AInteractiveRocketPart> AssembledPart;

	// 附近的零件(候选) - Nearby Parts (Candidates)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assembly Socket|State")
	TArray<TObjectPtr<AInteractiveRocketPart>> NearbyParts;

public:
	// ========== 装配验证 Assembly Validation ==========

	// 检查零件类型是否匹配 - Check if part type matches
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Validation")
	bool IsPartTypeAccepted(ERocketPartType PartType) const;

	// 检查零件ID是否匹配 - Check if part ID matches
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Validation")
	bool IsPartIDAccepted(FName PartID) const;

	// 检查零件是否可以装配 - Check if part can be assembled
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Validation")
	bool CanAssemblePart(AInteractiveRocketPart* Part) const;

	// 检查位置对齐 - Check position alignment
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Validation")
	bool IsPositionAligned(const FVector& PartLocation) const;

	// 检查旋转对齐 - Check rotation alignment
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Validation")
	bool IsRotationAligned(const FRotator& PartRotation) const;

	// 检查零件是否在磁吸范围内 - Check if part is in snap range
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Validation")
	bool IsInSnapRange(const FVector& PartLocation) const;

public:
	// ========== 装配操作 Assembly Operations ==========

	// 装配零件 - Assemble part
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Operations")
	bool AssemblePart(AInteractiveRocketPart* Part);

	// 卸下零件 - Disassemble part
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Operations")
	void DisassemblePart();

	// 应用磁吸效果 - Apply snap effect
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Operations")
	FTransform ApplySnapEffect(const FTransform& PartTransform, float DeltaTime);

public:
	// ========== 可视化 Visualization ==========

	// 更新指示器 - Update indicator
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Visual")
	void UpdateIndicator();

	// 显示/隐藏指示器 - Show/Hide indicator
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Visual")
	void SetIndicatorVisible(bool bVisible);

	// 设置指示器颜色 - Set indicator color
	UFUNCTION(BlueprintCallable, Category = "Assembly Socket|Visual")
	void SetIndicatorColor(const FLinearColor& Color);

public:
	// ========== 事件 Events ==========

	// 零件装配完成时触发 - Triggered when part is assembled
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPartAssembled, UAssemblySocket*, Socket, AInteractiveRocketPart*, Part);
	UPROPERTY(BlueprintAssignable, Category = "Assembly Socket|Events")
	FOnPartAssembled OnPartAssembled;

	// 零件卸下时触发 - Triggered when part is disassembled
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPartDisassembled, UAssemblySocket*, Socket, AInteractiveRocketPart*, Part);
	UPROPERTY(BlueprintAssignable, Category = "Assembly Socket|Events")
	FOnPartDisassembled OnPartDisassembled;

	// 零件进入范围时触发 - Triggered when part enters range
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPartEnterRange, UAssemblySocket*, Socket, AInteractiveRocketPart*, Part);
	UPROPERTY(BlueprintAssignable, Category = "Assembly Socket|Events")
	FOnPartEnterRange OnPartEnterRange;

	// 零件离开范围时触发 - Triggered when part leaves range
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPartLeaveRange, UAssemblySocket*, Socket, AInteractiveRocketPart*, Part);
	UPROPERTY(BlueprintAssignable, Category = "Assembly Socket|Events")
	FOnPartLeaveRange OnPartLeaveRange;

protected:
	// ========== 内部函数 Internal Functions ==========

	// 初始化指示器 - Initialize indicator
	void InitializeIndicator();

	// 检测附近的零件 - Detect nearby parts
	void DetectNearbyParts();

	// 更新附近零件列表 - Update nearby parts list
	void UpdateNearbyParts(float DeltaTime);
};
