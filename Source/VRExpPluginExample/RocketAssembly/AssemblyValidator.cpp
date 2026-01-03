// Copyright VRExpPluginExample. All Rights Reserved.

#include "AssemblyValidator.h"
#include "InteractiveRocketPart.h"
#include "AssemblySocket.h"
#include "Kismet/KismetMathLibrary.h"

UAssemblyValidator::UAssemblyValidator()
{
	// 默认配置
	PositionTolerance = 5.0f;  // 5 cm
	RotationTolerance = 10.0f; // 10 degrees
	bCheckAssemblyOrder = true;
	bCheckCollision = true;
	bCheckDependencies = true;
}

FAssemblyValidationResult UAssemblyValidator::ValidatePartAssembly(
	AInteractiveRocketPart* Part,
	UAssemblySocket* Socket,
	const TArray<AInteractiveRocketPart*>& AssembledParts)
{
	if (!Part || !Socket)
	{
		return CreateErrorResult(
			FText::FromString(TEXT("零件或装配点无效")),
			FText::FromString(TEXT("Invalid part or socket"))
		);
	}

	// 1. 验证位置对齐
	float PositionDeviation = 0.0f;
	if (!ValidatePositionAlignment(Part->GetActorLocation(), Socket->GetComponentLocation(), PositionDeviation))
	{
		return CreateErrorResult(
			FText::FromString(FString::Printf(TEXT("位置偏差过大: %.2f cm"), PositionDeviation)),
			FText::FromString(FString::Printf(TEXT("Position deviation too large: %.2f cm"), PositionDeviation)),
			PositionDeviation,
			0.0f
		);
	}

	// 2. 验证旋转对齐
	float RotationDeviation = 0.0f;
	if (!ValidateRotationAlignment(Part->GetActorRotation(), Socket->GetComponentRotation(), RotationDeviation))
	{
		return CreateErrorResult(
			FText::FromString(FString::Printf(TEXT("角度偏差过大: %.2f 度"), RotationDeviation)),
			FText::FromString(FString::Printf(TEXT("Rotation deviation too large: %.2f degrees"), RotationDeviation)),
			PositionDeviation,
			RotationDeviation
		);
	}

	// 3. 验证装配顺序
	if (bCheckAssemblyOrder && !ValidateAssemblyOrder(Part, AssembledParts))
	{
		return CreateErrorResult(
			FText::FromString(TEXT("装配顺序错误，请先装配前置零件")),
			FText::FromString(TEXT("Wrong assembly order, please assemble prerequisite parts first")),
			PositionDeviation,
			RotationDeviation
		);
	}

	// 4. 验证依赖关系
	if (bCheckDependencies && !ValidateDependencies(Part, AssembledParts))
	{
		return CreateErrorResult(
			FText::FromString(TEXT("依赖零件未装配")),
			FText::FromString(TEXT("Dependent parts not assembled")),
			PositionDeviation,
			RotationDeviation
		);
	}

	// 5. 验证碰撞
	if (bCheckCollision && !ValidateNoCollision(Part, AssembledParts))
	{
		return CreateErrorResult(
			FText::FromString(TEXT("检测到碰撞")),
			FText::FromString(TEXT("Collision detected")),
			PositionDeviation,
			RotationDeviation
		);
	}

	// 所有验证通过
	return CreateSuccessResult(PositionDeviation, RotationDeviation);
}

bool UAssemblyValidator::ValidatePositionAlignment(
	const FVector& PartLocation,
	const FVector& SocketLocation,
	float& OutDeviation)
{
	OutDeviation = FVector::Dist(PartLocation, SocketLocation);
	return OutDeviation <= PositionTolerance;
}

bool UAssemblyValidator::ValidateRotationAlignment(
	const FRotator& PartRotation,
	const FRotator& SocketRotation,
	float& OutDeviation)
{
	// 计算旋转差异
	FRotator DeltaRotation = (PartRotation - SocketRotation).GetNormalized();
	
	// 计算总偏差（简化为欧拉角的绝对值之和）
	OutDeviation = FMath::Abs(DeltaRotation.Pitch) + FMath::Abs(DeltaRotation.Yaw) + FMath::Abs(DeltaRotation.Roll);
	
	// 检查每个轴的偏差是否在容差范围内
	bool bPitchValid = FMath::Abs(DeltaRotation.Pitch) <= RotationTolerance;
	bool bYawValid = FMath::Abs(DeltaRotation.Yaw) <= RotationTolerance;
	bool bRollValid = FMath::Abs(DeltaRotation.Roll) <= RotationTolerance;
	
	return bPitchValid && bYawValid && bRollValid;
}

bool UAssemblyValidator::ValidateAssemblyOrder(
	AInteractiveRocketPart* Part,
	const TArray<AInteractiveRocketPart*>& AssembledParts)
{
	if (!Part)
	{
		return false;
	}

	// 获取当前零件的装配顺序
	int32 CurrentOrder = Part->PartData.AssemblyOrder;

	// 检查是否有装配顺序更靠前但尚未装配的零件
	// 这需要访问所有可用零件的列表，这里简化实现
	// 实际应该由 RocketPartManager 提供完整的零件列表

	// 简化实现：假设顺序较小的零件应该先装配
	for (AInteractiveRocketPart* AssembledPart : AssembledParts)
	{
		if (AssembledPart && AssembledPart->PartData.AssemblyOrder > CurrentOrder)
		{
			// 有顺序更靠后的零件已经装配，但当前零件还没装配，这是不合理的
			// 但这个逻辑可能需要更复杂的检查
		}
	}

	// 暂时返回 true，具体逻辑在管理器中实现
	return true;
}

bool UAssemblyValidator::ValidateNoCollision(
	AInteractiveRocketPart* Part,
	const TArray<AInteractiveRocketPart*>& AssembledParts)
{
	if (!Part || !Part->GetWorld())
	{
		return false;
	}

	// 使用碰撞检测检查是否与已装配的零件重叠
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Part);

	// 获取零件的边界
	FVector Origin;
	FVector BoxExtent;
	Part->GetActorBounds(false, Origin, BoxExtent);

	// 执行重叠检测
	bool bHasOverlap = Part->GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Part->GetActorLocation(),
		Part->GetActorRotation().Quaternion(),
		ECC_WorldDynamic,
		FCollisionShape::MakeBox(BoxExtent),
		QueryParams
	);

	if (bHasOverlap)
	{
		// 检查重叠的物体是否是已装配的零件
		for (const FOverlapResult& Overlap : Overlaps)
		{
			if (AInteractiveRocketPart* OverlappingPart = Cast<AInteractiveRocketPart>(Overlap.GetActor()))
			{
				if (AssembledParts.Contains(OverlappingPart) || OverlappingPart->IsAssembled())
				{
					// 与已装配的零件发生碰撞
					return false;
				}
			}
		}
	}

	return true;
}

bool UAssemblyValidator::ValidateDependencies(
	AInteractiveRocketPart* Part,
	const TArray<AInteractiveRocketPart*>& AssembledParts)
{
	if (!Part)
	{
		return false;
	}

	// 检查所有依赖零件是否已装配
	for (const FName& DependentPartID : Part->PartData.DependentPartIDs)
	{
		bool bDependencyMet = false;

		// 在已装配的零件中查找依赖零件
		for (AInteractiveRocketPart* AssembledPart : AssembledParts)
		{
			if (AssembledPart && AssembledPart->PartData.PartID == DependentPartID)
			{
				if (AssembledPart->IsAssembled())
				{
					bDependencyMet = true;
					break;
				}
			}
		}

		// 如果有任何依赖未满足，返回 false
		if (!bDependencyMet)
		{
			return false;
		}
	}

	return true;
}

float UAssemblyValidator::CalculatePrecisionScore(
	float PositionDeviation,
	float RotationDeviation)
{
	// 计算位置精度分数 (0-50分)
	float PositionScore = 50.0f;
	if (PositionDeviation > 0.0f)
	{
		// 偏差越大，分数越低
		float PositionPenalty = (PositionDeviation / PositionTolerance) * 50.0f;
		PositionScore = FMath::Max(0.0f, 50.0f - PositionPenalty);
	}

	// 计算角度精度分数 (0-50分)
	float RotationScore = 50.0f;
	if (RotationDeviation > 0.0f)
	{
		// 偏差越大，分数越低
		float RotationPenalty = (RotationDeviation / (RotationTolerance * 3.0f)) * 50.0f;
		RotationScore = FMath::Max(0.0f, 50.0f - RotationPenalty);
	}

	return PositionScore + RotationScore;
}

float UAssemblyValidator::CalculateOverallQuality(
	const TArray<FAssemblyValidationResult>& ValidationResults)
{
	if (ValidationResults.Num() == 0)
	{
		return 0.0f;
	}

	float TotalScore = 0.0f;
	int32 ValidCount = 0;

	for (const FAssemblyValidationResult& Result : ValidationResults)
	{
		if (Result.bIsValid)
		{
			TotalScore += Result.Score;
			ValidCount++;
		}
	}

	return ValidCount > 0 ? (TotalScore / ValidationResults.Num()) : 0.0f;
}

FText UAssemblyValidator::GetLocalizedErrorMessage(const FAssemblyValidationResult& Result) const
{
	// TODO: 实现基于游戏语言设置的本地化
	// 暂时返回英文消息
	return Result.ErrorMessageEN.IsEmpty() ? Result.ErrorMessageCN : Result.ErrorMessageEN;
}

FAssemblyValidationResult UAssemblyValidator::CreateErrorResult(
	const FText& ErrorMessageCN,
	const FText& ErrorMessageEN,
	float PositionDeviation,
	float RotationDeviation)
{
	FAssemblyValidationResult Result;
	Result.bIsValid = false;
	Result.ErrorMessageCN = ErrorMessageCN;
	Result.ErrorMessageEN = ErrorMessageEN;
	Result.PositionDeviation = PositionDeviation;
	Result.RotationDeviation = RotationDeviation;
	Result.Score = 0.0f;
	return Result;
}

FAssemblyValidationResult UAssemblyValidator::CreateSuccessResult(
	float PositionDeviation,
	float RotationDeviation)
{
	FAssemblyValidationResult Result;
	Result.bIsValid = true;
	Result.PositionDeviation = PositionDeviation;
	Result.RotationDeviation = RotationDeviation;
	Result.Score = CalculatePrecisionScore(PositionDeviation, RotationDeviation);
	Result.ErrorMessageCN = FText::FromString(TEXT("装配成功"));
	Result.ErrorMessageEN = FText::FromString(TEXT("Assembly successful"));
	return Result;
}
