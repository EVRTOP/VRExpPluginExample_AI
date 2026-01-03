// Copyright VRExpPluginExample. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RocketPartData.generated.h"

/**
 * 火箭零件类型枚举
 * Rocket Part Type Enumeration
 */
UENUM(BlueprintType)
enum class ERocketPartType : uint8
{
	// 主发动机系统 - Main Engine System
	MainEngineNozzle		UMETA(DisplayName = "Main Engine Nozzle"),
	TurboPump				UMETA(DisplayName = "Turbo Pump"),
	CombustionChamber		UMETA(DisplayName = "Combustion Chamber"),
	
	// 燃料系统 - Fuel System
	LOXTank					UMETA(DisplayName = "Liquid Oxygen Tank"),
	FuelTank				UMETA(DisplayName = "Fuel Tank (LH2/Kerosene)"),
	FuelPipeline			UMETA(DisplayName = "Fuel Pipeline"),
	
	// 箭体结构 - Rocket Body Structure
	FirstStageBody			UMETA(DisplayName = "First Stage Body"),
	SecondStageBody			UMETA(DisplayName = "Second Stage Body"),
	InterstageSection		UMETA(DisplayName = "Interstage Section"),
	
	// 控制系统 - Control System
	AttitudeControlEngine	UMETA(DisplayName = "Attitude Control Engine"),
	GyroscopeUnit			UMETA(DisplayName = "Gyroscope Unit"),
	ControlComputer			UMETA(DisplayName = "Control Computer"),
	
	// 整流罩与载荷 - Fairing & Payload
	FairingLeft				UMETA(DisplayName = "Fairing (Left)"),
	FairingRight			UMETA(DisplayName = "Fairing (Right)"),
	SatelliteAdapter		UMETA(DisplayName = "Satellite Adapter"),
	PayloadSimulator		UMETA(DisplayName = "Payload Simulator"),
	
	// 连接与分离系统 - Connection & Separation System
	StageSeparator			UMETA(DisplayName = "Stage Separator"),
	ExplosiveBolt			UMETA(DisplayName = "Explosive Bolt"),
	SupportStructure		UMETA(DisplayName = "Support Structure"),
	
	// 其他 - Other
	Other					UMETA(DisplayName = "Other")
};

/**
 * 装配状态枚举
 * Assembly State Enumeration
 */
UENUM(BlueprintType)
enum class EAssemblyState : uint8
{
	Unassembled		UMETA(DisplayName = "Unassembled"),		// 未装配
	Assembling		UMETA(DisplayName = "Assembling"),		// 装配中
	Assembled		UMETA(DisplayName = "Assembled"),		// 已装配
	Error			UMETA(DisplayName = "Error")			// 错误状态
};

/**
 * 火箭零件数据结构
 * Rocket Part Data Structure
 * 用于数据表定义火箭零件的所有属性
 */
USTRUCT(BlueprintType)
struct FRocketPartData : public FTableRowBase
{
	GENERATED_BODY()

	// 零件ID - Part ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info")
	FName PartID;

	// 零件名称(中文) - Part Name (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info")
	FText PartNameCN;

	// 零件名称(英文) - Part Name (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info")
	FText PartNameEN;

	// 零件类型 - Part Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info")
	ERocketPartType PartType;

	// 零件描述(中文) - Part Description (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info")
	FText DescriptionCN;

	// 零件描述(英文) - Part Description (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info")
	FText DescriptionEN;

	// 3D模型引用 - 3D Model Reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TSoftObjectPtr<UStaticMesh> PartMesh;

	// 材质引用 - Material Reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TSoftObjectPtr<UMaterialInterface> PartMaterial;

	// 质量(kg) - Mass in kg
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float Mass;

	// 尺寸(cm) - Dimensions in cm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	FVector Dimensions;

	// 装配点相对位置 - Assembly Socket Relative Location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly")
	FVector SocketRelativeLocation;

	// 装配点相对旋转 - Assembly Socket Relative Rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly")
	FRotator SocketRelativeRotation;

	// 装配顺序 - Assembly Order (lower number = earlier)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly")
	int32 AssemblyOrder;

	// 依赖零件ID列表 - Dependent Part IDs (must be assembled first)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly")
	TArray<FName> DependentPartIDs;

	// 科普知识内容(中文) - Educational Content (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Education", meta = (MultiLine = true))
	FText EducationalContentCN;

	// 科普知识内容(英文) - Educational Content (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Education", meta = (MultiLine = true))
	FText EducationalContentEN;

	// 技术参数(中文) - Technical Specifications (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Education", meta = (MultiLine = true))
	FText TechnicalSpecsCN;

	// 技术参数(英文) - Technical Specifications (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Education", meta = (MultiLine = true))
	FText TechnicalSpecsEN;

	// 科普图片引用 - Educational Image Reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Education")
	TSoftObjectPtr<UTexture2D> EducationalImage;

	// 语音讲解文件(中文) - Voice Audio File (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Education")
	TSoftObjectPtr<USoundWave> VoiceAudioCN;

	// 语音讲解文件(英文) - Voice Audio File (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Education")
	TSoftObjectPtr<USoundWave> VoiceAudioEN;

	// 是否为必需零件 - Is Required Part
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly")
	bool bIsRequired;

	// 构造函数初始化默认值
	FRocketPartData()
		: PartID(NAME_None)
		, PartType(ERocketPartType::Other)
		, Mass(1.0f)
		, Dimensions(FVector(100.0f, 100.0f, 100.0f))
		, SocketRelativeLocation(FVector::ZeroVector)
		, SocketRelativeRotation(FRotator::ZeroRotator)
		, AssemblyOrder(0)
		, bIsRequired(true)
	{
	}
};

/**
 * 装配步骤数据结构
 * Assembly Step Data Structure
 */
USTRUCT(BlueprintType)
struct FAssemblyStepData : public FTableRowBase
{
	GENERATED_BODY()

	// 步骤序号 - Step Number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Step Info")
	int32 StepNumber;

	// 步骤名称(中文) - Step Name (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Step Info")
	FText StepNameCN;

	// 步骤名称(英文) - Step Name (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Step Info")
	FText StepNameEN;

	// 需要装配的零件ID - Part ID to Assemble
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Step Info")
	FName PartIDToAssemble;

	// 目标装配点ID - Target Socket ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Step Info")
	FName TargetSocketID;

	// 教学文本(中文) - Tutorial Text (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial", meta = (MultiLine = true))
	FText TutorialTextCN;

	// 教学文本(英文) - Tutorial Text (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial", meta = (MultiLine = true))
	FText TutorialTextEN;

	// 语音文件引用(中文) - Voice File Reference (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	TSoftObjectPtr<USoundWave> VoiceFileCN;

	// 语音文件引用(英文) - Voice File Reference (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	TSoftObjectPtr<USoundWave> VoiceFileEN;

	// 提示视频/动画 - Hint Video/Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	TSoftObjectPtr<UObject> HintAnimation;

	// 是否显示AR引导 - Show AR Guidance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool bShowARGuidance;

	// 构造函数
	FAssemblyStepData()
		: StepNumber(0)
		, PartIDToAssemble(NAME_None)
		, TargetSocketID(NAME_None)
		, bShowARGuidance(true)
	{
	}
};

/**
 * 科普知识数据结构
 * Educational Knowledge Data Structure
 */
USTRUCT(BlueprintType)
struct FRocketKnowledgeData : public FTableRowBase
{
	GENERATED_BODY()

	// 知识点ID - Knowledge ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge")
	FName KnowledgeID;

	// 知识点标题(中文) - Knowledge Title (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge")
	FText TitleCN;

	// 知识点标题(英文) - Knowledge Title (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge")
	FText TitleEN;

	// 知识点内容(中文) - Knowledge Content (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge", meta = (MultiLine = true))
	FText ContentCN;

	// 知识点内容(英文) - Knowledge Content (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge", meta = (MultiLine = true))
	FText ContentEN;

	// 类别 - Category
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge")
	FText Category;

	// 相关图片 - Related Images
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge")
	TArray<TSoftObjectPtr<UTexture2D>> RelatedImages;

	// 相关3D模型 - Related 3D Models
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge")
	TArray<TSoftObjectPtr<UStaticMesh>> Related3DModels;

	// 语音讲解(中文) - Voice Explanation (Chinese)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge")
	TSoftObjectPtr<USoundWave> VoiceExplanationCN;

	// 语音讲解(英文) - Voice Explanation (English)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge")
	TSoftObjectPtr<USoundWave> VoiceExplanationEN;

	// 构造函数
	FRocketKnowledgeData()
		: KnowledgeID(NAME_None)
	{
	}
};
