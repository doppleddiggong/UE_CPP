// // Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Entity.generated.h"

UENUM(BlueprintType)
enum class ETransformOrder : uint8
{
	E_Identity UMETA(DisplayName = "Identity"),			// Represents no transformation, just the identity matrix
	E_TRS UMETA(DisplayName = "Translation * Rotation * Scale"),	// Apply Scale, then Rotate, then Translate (common for local space)
	E_TSR UMETA(DisplayName = "Translation * Scale * Rotation"),	// Apply Rotation, then Scale, then Translate
	E_RTS UMETA(DisplayName = "Rotation * Translation * Scale"),	// Apply Scale, then Translate, then Rotate
	E_SRT UMETA(DisplayName = "Scale * Rotation * Translation")		// Apply Translation, then Rotation, then Scale (common for world space)
};

enum class ETransformType : uint8
{
	Translate,
	Rotate,
	Scale
};

UCLASS()
class UE_CPP_API AEntity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation")
	ETransformOrder CurrentOrder = ETransformOrder::E_Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation")
	FVector Translation = FVector(100.f, 100.f, -100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation")
	FRotator Rotation = FRotator(45.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation")
	FVector Scale = FVector(2.f, 3.f, 1.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation Duration")
	float StepDuration = 1.f;
	
private:
	FMatrix TranslationMatrix;
	FMatrix RotationMatrix;
	FMatrix ScaleMatrix;

	ETransformOrder PreviousTransformOrder = ETransformOrder::E_Identity;
	
	float InterpElapsedTime = 0.f;
	int32 InterpStep = 0;

	FVector StartLocation;
	FRotator StartRotation;
	FVector StartScale;

	FVector TargetLocation;
	FRotator TargetRotation;
	FVector TargetScale;

	TArray<ETransformType> TransformOrder = {
		ETransformType::Translate,
		ETransformType::Rotate,
		ETransformType::Scale
	};

private:
	void InitData();
	void ApplyTransformation();

	void StartTransformInterpolation(const FMatrix& ResultMatrix);
	void UpdateTransformInterpolation(float DeltaTime);
	
	static FString GetTransformModeName(ETransformOrder Mode)
	{
		switch (Mode)
		{
		case ETransformOrder::E_Identity:	return TEXT("Identity");
		case ETransformOrder::E_TRS:		return TEXT("Translation * Rotation * Scale (TRS)");
		case ETransformOrder::E_TSR:		return TEXT("Translation * Scale * Rotation (TSR)");
		case ETransformOrder::E_RTS:		return TEXT("Rotation * Translation * Scale (RTS)");
		case ETransformOrder::E_SRT:		return TEXT("Scale * Rotation * Translation (SRT)");
		default:
			return TEXT("Unknown Order");
		}
	}
};
