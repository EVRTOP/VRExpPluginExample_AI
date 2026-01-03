// Copyright VRExpPluginExample. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RocketPartData.h"
#include "AssemblyValidator.generated.h"

// Forward declarations
class AInteractiveRocketPart;
class UAssemblySocket;

/**
 * 装配验证结果
 * Assembly Validation Result
 */
USTRUCT(BlueprintType)
struct FAssemblyValidationResult
{
	GENERATED_BODY()

	// 是否验证通过 - Is Valid
	UPROPERTY(BlueprintReadWrite, Category = "Validation")
	bool bIsValid;

	// 错误消息(中文) - Error Message (Chinese)
	UPROPERTY(BlueprintReadWrite, Category = "Validation")
	FText ErrorMessageCN;

	// 错误消息(英文) - Error Message (English)
	UPROPERTY(BlueprintReadWrite, Category = "Validation")
	FText ErrorMessageEN;

	// 验证分数 (0-100) - Validation Score
	UPROPERTY(BlueprintReadWrite, Category = "Validation")
	float Score;

	// 位置偏差(cm) - Position Deviation (cm)
	UPROPERTY(BlueprintReadWrite, Category = "Validation")
	float PositionDeviation;

	// 角度偏差(度) - Rotation Deviation (degrees)
	UPROPERTY(BlueprintReadWrite, Category = "Validation")
	float RotationDeviation;

	// 构造函数
	FAssemblyValidationResult()
		: bIsValid(false)
		, Score(0.0f)
		, PositionDeviation(0.0f)
		, RotationDeviation(0.0f)
	{
	}
};

/**
 * 装配验证器
 * Assembly Validator
 * 
 * 负责验证零件装配的正确性
 * 包括碰撞检测、顺序验证、对齐检测等
 */
UCLASS(Blueprintable, BlueprintType)
class VREXPPLUGINEXAMPLE_API UAssemblyValidator : public UObject
{
	GENERATED_BODY()

public:
	// 构造函数
	UAssemblyValidator();

public:
	// ========== 验证配置 Validation Configuration ==========

	// 位置对齐容差(cm) - Position Tolerance (cm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation|Settings")
	float PositionTolerance;

	// 角度对齐容差(度) - Rotation Tolerance (degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation|Settings")
	float RotationTolerance;

	// 是否检查装配顺序 - Check Assembly Order
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation|Settings")
	bool bCheckAssemblyOrder;

	// 是否检查碰撞 - Check Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation|Settings")
	bool bCheckCollision;

	// 是否检查依赖关系 - Check Dependencies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation|Settings")
	bool bCheckDependencies;

public:
	// ========== 主要验证函数 Main Validation Functions ==========

	// 验证零件装配 - Validate part assembly
	UFUNCTION(BlueprintCallable, Category = "Assembly Validator")
	FAssemblyValidationResult ValidatePartAssembly(
		AInteractiveRocketPart* Part,
		UAssemblySocket* Socket,
		const TArray<AInteractiveRocketPart*>& AssembledParts
	);

	// 验证位置对齐 - Validate position alignment
	UFUNCTION(BlueprintCallable, Category = "Assembly Validator")
	bool ValidatePositionAlignment(
		const FVector& PartLocation,
		const FVector& SocketLocation,
		float& OutDeviation
	);

	// 验证旋转对齐 - Validate rotation alignment
	UFUNCTION(BlueprintCallable, Category = "Assembly Validator")
	bool ValidateRotationAlignment(
		const FRotator& PartRotation,
		const FRotator& SocketRotation,
		float& OutDeviation
	);

	// 验证装配顺序 - Validate assembly order
	UFUNCTION(BlueprintCallable, Category = "Assembly Validator")
	bool ValidateAssemblyOrder(
		AInteractiveRocketPart* Part,
		const TArray<AInteractiveRocketPart*>& AssembledParts
	);

	// 验证碰撞 - Validate collision
	UFUNCTION(BlueprintCallable, Category = "Assembly Validator")
	bool ValidateNoCollision(
		AInteractiveRocketPart* Part,
		const TArray<AInteractiveRocketPart*>& AssembledParts
	);

	// 验证依赖关系 - Validate dependencies
	UFUNCTION(BlueprintCallable, Category = "Assembly Validator")
	bool ValidateDependencies(
		AInteractiveRocketPart* Part,
		const TArray<AInteractiveRocketPart*>& AssembledParts
	);

public:
	// ========== 评分函数 Scoring Functions ==========

	// 计算装配精度分数 - Calculate assembly precision score
	UFUNCTION(BlueprintCallable, Category = "Assembly Validator|Scoring")
	float CalculatePrecisionScore(
		float PositionDeviation,
		float RotationDeviation
	);

	// 计算整体装配质量 - Calculate overall assembly quality
	UFUNCTION(BlueprintCallable, Category = "Assembly Validator|Scoring")
	float CalculateOverallQuality(
		const TArray<FAssemblyValidationResult>& ValidationResults
	);

public:
	// ========== 辅助函数 Helper Functions ==========

	// 获取本地化错误消息 - Get localized error message
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Assembly Validator")
	FText GetLocalizedErrorMessage(const FAssemblyValidationResult& Result) const;

	// 创建错误结果 - Create error result
	FAssemblyValidationResult CreateErrorResult(
		const FText& ErrorMessageCN,
		const FText& ErrorMessageEN,
		float PositionDeviation = 0.0f,
		float RotationDeviation = 0.0f
	);

	// 创建成功结果 - Create success result
	FAssemblyValidationResult CreateSuccessResult(
		float PositionDeviation,
		float RotationDeviation
	);
};
