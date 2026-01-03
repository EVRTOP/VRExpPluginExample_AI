// Copyright VRExpPluginExample. All Rights Reserved.

#include "InteractiveRocketPart.h"
#include "AssemblySocket.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/DataTable.h"

AInteractiveRocketPart::AInteractiveRocketPart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 启用 Tick
	PrimaryActorTick.bCanEverTick = true;

	// 初始化状态
	AssemblyState = EAssemblyState::Unassembled;
	bIsHovered = false;
	bCanBeAssembled = false;
	bIsLocked = false;
	TargetSocket = nullptr;

	// 初始化颜色
	HighlightColor = FLinearColor(1.0f, 0.8f, 0.0f, 1.0f);  // 黄色
	CanAssembleColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // 绿色
	ErrorColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);      // 红色
}

void AInteractiveRocketPart::BeginPlay()
{
	Super::BeginPlay();

	// 初始化材质
	InitializeMaterials();

	// 保存原始位置和旋转
	OriginalLocation = GetActorLocation();
	OriginalRotation = GetActorRotation();
}

void AInteractiveRocketPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 更新视觉效果
	UpdateVisuals();
}

void AInteractiveRocketPart::InitializeMaterials()
{
	if (GetStaticMeshComponent() && GetStaticMeshComponent()->GetMaterial(0))
	{
		// 保存默认材质
		if (!DefaultMaterial)
		{
			DefaultMaterial = GetStaticMeshComponent()->GetMaterial(0);
		}

		// 创建动态材质实例
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(DefaultMaterial, this);
		if (DynamicMaterialInstance)
		{
			GetStaticMeshComponent()->SetMaterial(0, DynamicMaterialInstance);
		}
	}
}

void AInteractiveRocketPart::EnableHighlight(bool bEnable)
{
	bIsHovered = bEnable;
	
	if (bEnable)
	{
		// 启用高亮
		if (DynamicMaterialInstance)
		{
			DynamicMaterialInstance->SetVectorParameterValue(FName("EmissiveColor"), HighlightColor);
			DynamicMaterialInstance->SetScalarParameterValue(FName("EmissiveStrength"), 2.0f);
		}
		else if (HighlightMaterial)
		{
			GetStaticMeshComponent()->SetMaterial(0, HighlightMaterial);
		}

		// 触发事件
		OnPartHovered.Broadcast(FBPActorGripInformation());
	}
	else
	{
		// 禁用高亮
		SetAssemblyStateMaterial(AssemblyState);

		// 触发事件
		OnPartUnhovered.Broadcast(FBPActorGripInformation());
	}
}

void AInteractiveRocketPart::SetAssemblyStateMaterial(EAssemblyState NewState)
{
	if (!GetStaticMeshComponent())
	{
		return;
	}

	switch (NewState)
	{
	case EAssemblyState::Unassembled:
		if (DefaultMaterial)
		{
			GetStaticMeshComponent()->SetMaterial(0, DefaultMaterial);
		}
		break;

	case EAssemblyState::Assembling:
		if (bCanBeAssembled)
		{
			if (DynamicMaterialInstance)
			{
				DynamicMaterialInstance->SetVectorParameterValue(FName("EmissiveColor"), CanAssembleColor);
				DynamicMaterialInstance->SetScalarParameterValue(FName("EmissiveStrength"), 3.0f);
			}
			else if (CanAssembleMaterial)
			{
				GetStaticMeshComponent()->SetMaterial(0, CanAssembleMaterial);
			}
		}
		break;

	case EAssemblyState::Assembled:
		if (DefaultMaterial)
		{
			GetStaticMeshComponent()->SetMaterial(0, DefaultMaterial);
		}
		break;

	case EAssemblyState::Error:
		if (DynamicMaterialInstance)
		{
			DynamicMaterialInstance->SetVectorParameterValue(FName("EmissiveColor"), ErrorColor);
			DynamicMaterialInstance->SetScalarParameterValue(FName("EmissiveStrength"), 3.0f);
		}
		else if (ErrorMaterial)
		{
			GetStaticMeshComponent()->SetMaterial(0, ErrorMaterial);
		}
		break;
	}
}

void AInteractiveRocketPart::ShowInfoLabel(bool bShow)
{
	// 此功能在蓝图中实现，显示3D Widget组件
	// This function is implemented in Blueprint to show 3D Widget component
}

void AInteractiveRocketPart::SetAssemblyState(EAssemblyState NewState)
{
	EAssemblyState OldState = AssemblyState;
	AssemblyState = NewState;

	// 更新材质
	SetAssemblyStateMaterial(NewState);

	// 触发相应事件
	if (NewState == EAssemblyState::Assembled && OldState != EAssemblyState::Assembled)
	{
		OnPartAssembled.Broadcast(FBPActorGripInformation());
		PlayAssemblySound(true);
		PlayAssemblyVFX(true);
	}
	else if (NewState == EAssemblyState::Error)
	{
		OnAssemblyFailed.Broadcast(FBPActorGripInformation());
		PlayAssemblySound(false);
		PlayAssemblyVFX(false);
	}
}

bool AInteractiveRocketPart::CanAssembleToSocket(UAssemblySocket* Socket)
{
	if (!Socket)
	{
		return false;
	}

	// 检查零件类型是否匹配
	// Check if part type matches (implemented in Socket)

	// 检查装配依赖
	if (!AreDependenciesMet())
	{
		return false;
	}

	// 检查是否已装配
	if (AssemblyState == EAssemblyState::Assembled)
	{
		return false;
	}

	return true;
}

bool AInteractiveRocketPart::AssembleToSocket(UAssemblySocket* Socket)
{
	if (!CanAssembleToSocket(Socket))
	{
		SetAssemblyState(EAssemblyState::Error);
		return false;
	}

	TargetSocket = Socket;

	// 对齐到装配点
	if (Socket && Socket->GetOwner())
	{
		FVector SocketLocation = Socket->GetComponentLocation();
		FRotator SocketRotation = Socket->GetComponentRotation();

		SetActorLocation(SocketLocation);
		SetActorRotation(SocketRotation);

		// 附加到装配点
		AttachToActor(Socket->GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	}

	// 设置状态为已装配
	SetAssemblyState(EAssemblyState::Assembled);

	// 锁定零件
	LockPart(true);

	return true;
}

void AInteractiveRocketPart::DisassembleFromSocket()
{
	if (AssemblyState != EAssemblyState::Assembled)
	{
		return;
	}

	// 解除附加
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 解锁零件
	LockPart(false);

	// 设置状态为未装配
	SetAssemblyState(EAssemblyState::Unassembled);

	TargetSocket = nullptr;

	// 触发事件
	OnPartDisassembled.Broadcast(FBPActorGripInformation());
}

void AInteractiveRocketPart::LockPart(bool bLock)
{
	bIsLocked = bLock;

	// 设置是否可抓取
	if (bLock)
	{
		// 禁用抓取
		SetDenyGripping(true);

		// 禁用物理
		if (GetStaticMeshComponent())
		{
			GetStaticMeshComponent()->SetSimulatePhysics(false);
		}
	}
	else
	{
		// 启用抓取
		SetDenyGripping(false);

		// 启用物理
		if (GetStaticMeshComponent())
		{
			GetStaticMeshComponent()->SetSimulatePhysics(true);
		}
	}
}

bool AInteractiveRocketPart::AreDependenciesMet() const
{
	// 检查所有依赖零件是否已装配
	// 此功能需要访问零件管理器来检查其他零件的状态
	// 暂时返回 true，具体实现在蓝图或管理器中完成
	
	// TODO: Implement dependency checking with RocketPartManager
	return true;
}

FText AInteractiveRocketPart::GetLocalizedPartName() const
{
	// 根据当前语言返回对应的零件名称
	// TODO: Implement proper localization based on game settings
	// 默认返回英文，如果英文为空则返回中文
	return PartData.PartNameEN.IsEmpty() ? PartData.PartNameCN : PartData.PartNameEN;
}

FText AInteractiveRocketPart::GetLocalizedPartDescription() const
{
	// 根据当前语言返回对应的零件描述
	// TODO: Implement proper localization based on game settings
	// 默认返回英文，如果英文为空则返回中文
	return PartData.DescriptionEN.IsEmpty() ? PartData.DescriptionCN : PartData.DescriptionEN;
}

FString AInteractiveRocketPart::GetPartTypeString() const
{
	// 将枚举转换为字符串 - 使用 StaticEnum 更可靠
	return StaticEnum<ERocketPartType>()->GetNameStringByValue((int64)PartData.PartType);
}

void AInteractiveRocketPart::UpdateVisuals()
{
	// 持续更新视觉效果
	// 例如，如果接近装配点，显示可装配状态
}

void AInteractiveRocketPart::PlayAssemblySound(bool bSuccess)
{
	// 播放装配音效
	// TODO: 在蓝图中实现音效播放
	// 使用 UGameplayStatics::PlaySoundAtLocation
}

void AInteractiveRocketPart::PlayAssemblyVFX(bool bSuccess)
{
	// 播放视觉特效
	// TODO: 在蓝图中实现特效播放
	// 使用 UGameplayStatics::SpawnEmitterAtLocation
}

bool AInteractiveRocketPart::LoadPartDataFromTable(UDataTable* DataTable, FName RowName)
{
	if (!DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadPartDataFromTable: DataTable is null"));
		return false;
	}

	// 从数据表中查找行数据
	FRocketPartData* RowData = DataTable->FindRow<FRocketPartData>(RowName, TEXT(""));
	if (!RowData)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadPartDataFromTable: Row '%s' not found in data table"), *RowName.ToString());
		return false;
	}

	// 复制数据
	PartData = *RowData;

	// 如果有3D模型引用，加载并设置
	if (PartData.PartMesh.IsValid() || !PartData.PartMesh.IsNull())
	{
		UStaticMesh* Mesh = PartData.PartMesh.LoadSynchronous();
		if (Mesh && GetStaticMeshComponent())
		{
			GetStaticMeshComponent()->SetStaticMesh(Mesh);
		}
	}

	// 如果有材质引用，加载并设置
	if (PartData.PartMaterial.IsValid() || !PartData.PartMaterial.IsNull())
	{
		UMaterialInterface* Material = PartData.PartMaterial.LoadSynchronous();
		if (Material && GetStaticMeshComponent())
		{
			DefaultMaterial = Material;
			GetStaticMeshComponent()->SetMaterial(0, Material);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("LoadPartDataFromTable: Successfully loaded part data for '%s'"), *PartData.PartNameEN.ToString());
	return true;
}
