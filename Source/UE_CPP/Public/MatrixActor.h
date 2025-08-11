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
	E_Reflection_X	UMETA(DisplayName = "Reflection_X, YZ 평면 대칭 , X축 좌표 반전"),
	E_Reflection_Y	UMETA(DisplayName = "Reflection_Y, XZ 평면 대칭 , Y축 좌표 반전"),
	E_Reflection_Z	UMETA(DisplayName = "Reflection_Z, XY 평면 대칭 , Z축 좌표 반전"),

	E_Orthogonal_X	UMETA(DisplayName = "Orthogonal_X"),
	E_Orthogonal_Y	UMETA(DisplayName = "Orthogonal_Y"),
	E_Orthogonal_Z	UMETA(DisplayName = "Orthogonal_Z"),
};

UCLASS()
class UE_CPP_API AMatrixActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMatrixActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 현재 데모 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation")
	EMatrixDemoType CurrentOrder = EMatrixDemoType::E_Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation")
	FVector Translation = FVector(100.f, 100.f, -100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation")
	FRotator Rotation = FRotator(45.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation")
	FVector Scale = FVector(2.f, 3.f, 1.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transformation Duration")
	float Duration = 5.0f;

	
private:
	EMatrixDemoType PreviousOrder = EMatrixDemoType::E_Identity;

	FTransform CurrentTransform;
	FTransform TargetTransform;

	// 행렬
	FMatrix TranslationMatrix;
	FMatrix RotationMatrix;
	FMatrix ScaleMatrix;
	FMatrix ResultMatrix;

	FVector TargetLocation;
	FRotator TargetRotation;
	FQuat TargetQuat;
	FVector TargetScale;
	
	float LerpAlpha;
	

private:
	void InitData();
	void ApplyTransformation();
	void UpdateTransform(const float DeltaTime);

	void GenerateTargetTransform();
	
	static FString GetTransformModeName(EMatrixDemoType Mode)
	{
		switch (Mode)
		{
		case EMatrixDemoType::E_Identity:	return TEXT("Identity");
		case EMatrixDemoType::E_Rotation:   return TEXT("Rotation");

		case EMatrixDemoType::E_Reflection_X: return TEXT("Reflection_X");
		case EMatrixDemoType::E_Reflection_Y: return TEXT("Reflection_Y");
		case EMatrixDemoType::E_Reflection_Z: return TEXT("Reflection_Z");

		case EMatrixDemoType::E_Orthogonal_X: return TEXT("Orthogonal_X");
		case EMatrixDemoType::E_Orthogonal_Y: return TEXT("Orthogonal_Y");
		case EMatrixDemoType::E_Orthogonal_Z: return TEXT("Orthogonal_Z");

		default:
			return TEXT("Unknown Order");
		}
	}
};
