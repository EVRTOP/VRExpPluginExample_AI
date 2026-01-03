// Copyright VRExpPluginExample. All Rights Reserved.

#include "AssemblySocket.h"
#include "InteractiveRocketPart.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

UAssemblySocket::UAssemblySocket()
{
	// 启用 Tick
	PrimaryComponentTick.bCanEverTick = true;

	// 默认配置
	PositionTolerance = 5.0f;  // 5 cm
	RotationTolerance = 10.0f; // 10 degrees
	SnapDistance = 30.0f;      // 30 cm
	SnapStrength = 0.5f;       // 50% snap strength
	bEnableSnap = true;
	bShowIndicator = true;
	bIsOccupied = false;

	// 默认颜色
	IdleColor = FLinearColor(0.0f, 0.5f, 1.0f, 0.5f);        // 蓝色半透明
	CanAssembleColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.8f); // 绿色
	OccupiedColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.3f);    // 灰色
}

void UAssemblySocket::BeginPlay()
{
	Super::BeginPlay();

	// 初始化指示器
	InitializeIndicator();
}

void UAssemblySocket::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 更新附近零件列表
	UpdateNearbyParts(DeltaTime);

	// 更新指示器
	UpdateIndicator();
}

void UAssemblySocket::InitializeIndicator()
{
	if (!bShowIndicator)
	{
		return;
	}

	// 如果还没有指示器网格组件，创建一个
	if (!IndicatorMeshComponent)
	{
		IndicatorMeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass(), TEXT("SocketIndicator"));
		if (IndicatorMeshComponent)
		{
			IndicatorMeshComponent->RegisterComponent();
			IndicatorMeshComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
			IndicatorMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
			// 如果有指示器网格，设置它
			if (IndicatorMesh)
			{
				IndicatorMeshComponent->SetStaticMesh(IndicatorMesh);
			}

			// 设置初始颜色
			SetIndicatorColor(IdleColor);
		}
	}
}

bool UAssemblySocket::IsPartTypeAccepted(ERocketPartType PartType) const
{
	// 如果没有指定接受的类型，接受所有类型
	if (AcceptedPartTypes.Num() == 0)
	{
		return true;
	}

	// 检查类型是否在接受列表中
	return AcceptedPartTypes.Contains(PartType);
}

bool UAssemblySocket::IsPartIDAccepted(FName PartID) const
{
	// 如果没有指定接受的ID，只检查类型
	if (AcceptedPartIDs.Num() == 0)
	{
		return true;
	}

	// 检查ID是否在接受列表中
	return AcceptedPartIDs.Contains(PartID);
}

bool UAssemblySocket::CanAssemblePart(AInteractiveRocketPart* Part) const
{
	if (!Part)
	{
		return false;
	}

	// 检查是否已被占用
	if (bIsOccupied)
	{
		return false;
	}

	// 检查零件类型
	if (!IsPartTypeAccepted(Part->PartData.PartType))
	{
		return false;
	}

	// 检查零件ID
	if (!IsPartIDAccepted(Part->PartData.PartID))
	{
		return false;
	}

	// 检查位置对齐
	if (!IsPositionAligned(Part->GetActorLocation()))
	{
		return false;
	}

	// 检查旋转对齐
	if (!IsRotationAligned(Part->GetActorRotation()))
	{
		return false;
	}

	return true;
}

bool UAssemblySocket::IsPositionAligned(const FVector& PartLocation) const
{
	FVector SocketLocation = GetComponentLocation();
	float Distance = FVector::Dist(SocketLocation, PartLocation);
	return Distance <= PositionTolerance;
}

bool UAssemblySocket::IsRotationAligned(const FRotator& PartRotation) const
{
	FRotator SocketRotation = GetComponentRotation();
	
	// 计算旋转差异
	FRotator DeltaRotation = (PartRotation - SocketRotation).GetNormalized();
	
	// 检查每个轴的差异
	bool bPitchAligned = FMath::Abs(DeltaRotation.Pitch) <= RotationTolerance;
	bool bYawAligned = FMath::Abs(DeltaRotation.Yaw) <= RotationTolerance;
	bool bRollAligned = FMath::Abs(DeltaRotation.Roll) <= RotationTolerance;
	
	return bPitchAligned && bYawAligned && bRollAligned;
}

bool UAssemblySocket::IsInSnapRange(const FVector& PartLocation) const
{
	FVector SocketLocation = GetComponentLocation();
	float Distance = FVector::Dist(SocketLocation, PartLocation);
	return Distance <= SnapDistance;
}

bool UAssemblySocket::AssemblePart(AInteractiveRocketPart* Part)
{
	if (!CanAssemblePart(Part))
	{
		return false;
	}

	// 装配零件
	AssembledPart = Part;
	bIsOccupied = true;

	// 触发事件
	OnPartAssembled.Broadcast(this, Part);

	// 更新指示器
	UpdateIndicator();

	return true;
}

void UAssemblySocket::DisassemblePart()
{
	if (!bIsOccupied || !AssembledPart)
	{
		return;
	}

	AInteractiveRocketPart* Part = AssembledPart;

	// 卸下零件
	AssembledPart = nullptr;
	bIsOccupied = false;

	// 触发事件
	OnPartDisassembled.Broadcast(this, Part);

	// 更新指示器
	UpdateIndicator();
}

FTransform UAssemblySocket::ApplySnapEffect(const FTransform& PartTransform, float DeltaTime)
{
	if (!bEnableSnap)
	{
		return PartTransform;
	}

	FVector PartLocation = PartTransform.GetLocation();
	FRotator PartRotation = PartTransform.GetRotation().Rotator();

	// 检查是否在磁吸范围内
	if (!IsInSnapRange(PartLocation))
	{
		return PartTransform;
	}

	// 计算目标位置和旋转
	FVector TargetLocation = GetComponentLocation();
	FRotator TargetRotation = GetComponentRotation();

	// 应用插值磁吸
	float Alpha = SnapStrength * DeltaTime * 10.0f; // 调整磁吸速度
	FVector NewLocation = FMath::VInterpTo(PartLocation, TargetLocation, DeltaTime, Alpha);
	FRotator NewRotation = FMath::RInterpTo(PartRotation, TargetRotation, DeltaTime, Alpha);

	// 如果非常接近，直接对齐
	if (FVector::Dist(NewLocation, TargetLocation) < 1.0f)
	{
		NewLocation = TargetLocation;
	}

	FTransform NewTransform = PartTransform;
	NewTransform.SetLocation(NewLocation);
	NewTransform.SetRotation(NewRotation.Quaternion());

	return NewTransform;
}

void UAssemblySocket::UpdateIndicator()
{
	if (!bShowIndicator || !IndicatorMeshComponent)
	{
		return;
	}

	// 根据状态设置颜色
	if (bIsOccupied)
	{
		SetIndicatorColor(OccupiedColor);
	}
	else if (NearbyParts.Num() > 0)
	{
		// 检查是否有可装配的零件在附近
		bool bHasValidPart = false;
		for (AInteractiveRocketPart* Part : NearbyParts)
		{
			if (Part && CanAssemblePart(Part))
			{
				bHasValidPart = true;
				break;
			}
		}
		
		SetIndicatorColor(bHasValidPart ? CanAssembleColor : IdleColor);
	}
	else
	{
		SetIndicatorColor(IdleColor);
	}
}

void UAssemblySocket::SetIndicatorVisible(bool bVisible)
{
	if (IndicatorMeshComponent)
	{
		IndicatorMeshComponent->SetVisibility(bVisible);
	}
}

void UAssemblySocket::SetIndicatorColor(const FLinearColor& Color)
{
	if (IndicatorMeshComponent)
	{
		// 创建动态材质实例并设置颜色
		UMaterialInstanceDynamic* DynMaterial = IndicatorMeshComponent->CreateDynamicMaterialInstance(0);
		if (DynMaterial)
		{
			DynMaterial->SetVectorParameterValue(FName("BaseColor"), Color);
			DynMaterial->SetVectorParameterValue(FName("EmissiveColor"), Color);
		}
	}
}

void UAssemblySocket::DetectNearbyParts()
{
	// 清空当前列表
	TArray<TObjectPtr<AInteractiveRocketPart>> OldNearbyParts = NearbyParts;
	NearbyParts.Empty();

	// 在范围内搜索零件
	TArray<AActor*> FoundActors;
	UClass* PartClass = AInteractiveRocketPart::StaticClass();
	
	// 使用球形检测
	FVector SocketLocation = GetComponentLocation();
	
	// 简化实现：遍历世界中的所有 InteractiveRocketPart
	if (GetWorld())
	{
		for (TActorIterator<AInteractiveRocketPart> It(GetWorld()); It; ++It)
		{
			AInteractiveRocketPart* Part = *It;
			if (Part && !Part->IsAssembled())
			{
				float Distance = FVector::Dist(SocketLocation, Part->GetActorLocation());
				if (Distance <= SnapDistance * 2.0f) // 检测范围是磁吸距离的两倍
				{
					NearbyParts.Add(Part);
					
					// 如果是新进入的零件，触发事件
					if (!OldNearbyParts.Contains(Part))
					{
						OnPartEnterRange.Broadcast(this, Part);
					}
				}
			}
		}
	}

	// 检查离开范围的零件
	for (AInteractiveRocketPart* OldPart : OldNearbyParts)
	{
		if (OldPart && !NearbyParts.Contains(OldPart))
		{
			OnPartLeaveRange.Broadcast(this, OldPart);
		}
	}
}

void UAssemblySocket::UpdateNearbyParts(float DeltaTime)
{
	// 每帧检测附近的零件
	DetectNearbyParts();
}
