// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "MatrixActor.h"

AMatrixActor::AMatrixActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMatrixActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMatrixActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PreviousOrder != CurrentOrder)
	{
		PreviousOrder = CurrentOrder;

		// 일단 초기화
		SetActorLocation(FVector::ZeroVector);
		SetActorRotation(FRotator::ZeroRotator);
		SetActorScale3D(FVector::OneVector);
		
		ApplyTransformation();
	}
	
	this->UpdateTransformInterpolation(DeltaTime);
}

FString AMatrixActor::GetTransformModeName(EMatrixDemoType Mode) const
{
	switch (Mode)
	{
	case EMatrixDemoType::E_Identity:	return TEXT("Identity");
	case EMatrixDemoType::E_Rotation:   return TEXT("Rotation");
	case EMatrixDemoType::E_Reflection: return TEXT("TReflection");
	case EMatrixDemoType::E_Orthogonal: return TEXT("Orthogonal");
	default:
		return TEXT("Unknown Order");
	}
}

void AMatrixActor::ApplyTransformation()
{
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("Current Transform Order: %s"), *GetTransformModeName(CurrentOrder));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Orange, Msg);
	}
	
	switch (CurrentOrder)
	{
	case EMatrixDemoType::E_Identity:
		ResultMatrix = FMatrix::Identity;
    	
		SetActorLocation(FVector::ZeroVector);
		SetActorRotation(FRotator::ZeroRotator);
		SetActorScale3D(FVector::OneVector);
		break;
		
	case EMatrixDemoType::E_Rotation:
		{
			FVector Translation(200.f, 0.f, 0.f);
			FRotator Rotation(0.f, 90.f, 0.f);
			FVector Scale(1.f, 1.f, 1.f);

			TranslationMatrix = FTranslationMatrix(Translation);
			RotationMatrix = FRotationMatrix::Make(Rotation);
			ScaleMatrix = FScaleMatrix(Scale);

			ResultMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
		}
		break;
		
	case EMatrixDemoType::E_Reflection:
		{
			// 단순 Mirror 예시
			FVector Translation(0.f, 200.f, 0.f);
			FVector MirrorAxis(1.f, 0.f, 0.f);

			TranslationMatrix = FTranslationMatrix(Translation);
			RotationMatrix = FMatrix::Identity;
			ScaleMatrix = FScaleMatrix(FVector(1.f, -1.f, 1.f)); // Y축 반전

			ResultMatrix = TranslationMatrix * ScaleMatrix;
		}
		break;
		
	case EMatrixDemoType::E_Orthogonal:
		{
			// 직교 행렬 예시 (Quat + Matrix)
			FQuat Quat = FRotator(0.f, 45.f, 0.f).Quaternion();
			FMatrix QuatMatrix = FQuatRotationMatrix(Quat);
			TranslationMatrix = FTranslationMatrix(FVector(0.f, 0.f, 200.f));
			RotationMatrix = QuatMatrix;
			ScaleMatrix = FScaleMatrix(FVector(1.f, 1.f, 1.f));

			ResultMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
		}
		break;
	}

	// 위치 추출
	FVector NewLocation = ResultMatrix.GetOrigin();

	// 회전 추출 (쿼터니언)
	FQuat NewRotation(ResultMatrix);

	// 스케일 추출
	FVector NewScale;
	NewScale.X = ResultMatrix.GetScaledAxis(EAxis::X).Size();
	NewScale.Y = ResultMatrix.GetScaledAxis(EAxis::Y).Size();
	NewScale.Z = ResultMatrix.GetScaledAxis(EAxis::Z).Size();

	// 개별 적용
	SetActorLocation(NewLocation);
	SetActorRotation(NewRotation);
	SetActorScale3D(NewScale);
}