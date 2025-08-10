// // Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatrixActor.generated.h"

UENUM(BlueprintType)
enum class EMatrixDemoType : uint8
{
	E_Identity		UMETA(DisplayName = "Identity"),
	E_Rotation		UMETA(DisplayName = "Rotation"),
	E_Reflection	UMETA(DisplayName = "Reflection"),
	E_Orthogonal	UMETA(DisplayName = "Orthogonal"),
};

UENUM(BlueprintType)
enum class ETRSType : uint8
{
	Translate,
	Rotate,
	Scale
};

UCLASS()
class UE_CPP_API AMatrixActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMatrixActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 현재 데모 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation")
	EMatrixDemoType CurrentOrder = EMatrixDemoType::E_Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation Duration")
	float StepDuration = 1.f;

	UFUNCTION(BlueprintCallable, Category="Transformation Demo")
	void ApplyTransformation();

	FString GetTransformModeName(EMatrixDemoType Mode) const;

private:
	EMatrixDemoType PreviousOrder = EMatrixDemoType::E_Identity;
	
	FVector StartLocation;
	FRotator StartRotation;
	FVector StartScale;

	FVector TargetLocation;
	FRotator TargetRotation;
	FVector TargetScale;

	// 행렬
	FMatrix TranslationMatrix;
	FMatrix RotationMatrix;
	FMatrix ScaleMatrix;

	FMatrix ResultMatrix;

	// 애니메이션
	int32 InterpStep = 0;
	float InterpElapsedTime = 0.f;

	TArray<ETRSType> TransformOrder = { ETRSType::Translate, ETRSType::Rotate, ETRSType::Scale };

private:
	void StartTransformInterpolation(const FMatrix& InMatrix);
	void UpdateTransformInterpolation(float DeltaTime);
};
