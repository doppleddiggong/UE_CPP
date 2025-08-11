// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "MatrixActor.h"

AMatrixActor::AMatrixActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMatrixActor::BeginPlay()
{
	Super::BeginPlay();

	this->InitData();
}

void AMatrixActor::InitData()
{
	PreviousOrder = EMatrixDemoType::E_Identity;
	CurrentOrder = EMatrixDemoType::E_Identity;
	
	SetActorLocation(FVector::ZeroVector);
	SetActorRotation(FRotator::ZeroRotator);
	SetActorScale3D(FVector::OneVector);

	TranslationMatrix = FTranslationMatrix(Translation);
	RotationMatrix = FRotationMatrix::Make(Rotation);
	ScaleMatrix = FScaleMatrix(Scale);
	ResultMatrix = FMatrix::Identity;
}

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

		TranslationMatrix = FTranslationMatrix(Translation);
		RotationMatrix = FRotationMatrix::Make(Rotation);
		ScaleMatrix = FScaleMatrix(Scale);
		ResultMatrix = FMatrix::Identity;
		
		ApplyTransformation();
	}

	UpdateTransform(DeltaTime);
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

		TargetTransform.SetLocation(FVector::ZeroVector);
		TargetTransform.SetRotation(FQuat::Identity);
		TargetTransform.SetScale3D(FVector::OneVector);
		break;
		
	case EMatrixDemoType::E_Rotation:
		{
			ResultMatrix = FMatrix::Identity;
			ResultMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
			
			GenerateTargetTransform();
		}
		break;
		
	case EMatrixDemoType::E_Reflection_X:
		{
			FMatrix ReflectionMatrix = FMatrix::Identity;
			// X축 대칭 (X 성분 부호 반전)
			ReflectionMatrix.M[0][0] = -1.0f;

			ResultMatrix = FMatrix::Identity;
			ResultMatrix = TranslationMatrix * ReflectionMatrix * RotationMatrix * ScaleMatrix;
			
			GenerateTargetTransform();
		}
		break;
		
	case EMatrixDemoType::E_Reflection_Y:
		{
			FMatrix ReflectionMatrix = FMatrix::Identity;
			// Y축 대칭( Y 성분 부호 반전)
			ReflectionMatrix.M[1][1] = -1.0f;

			ResultMatrix = FMatrix::Identity;
			ResultMatrix = TranslationMatrix * ReflectionMatrix * RotationMatrix * ScaleMatrix;
			
			GenerateTargetTransform();		
		}
		break;
		
	case EMatrixDemoType::E_Reflection_Z:
		{
			FMatrix ReflectionMatrix = FMatrix::Identity;
			// Z축 대칭( Z 성분 부호 반전)
			ReflectionMatrix.M[2][2] = -1.0f;

			ResultMatrix = FMatrix::Identity;
			ResultMatrix = TranslationMatrix * ReflectionMatrix * RotationMatrix * ScaleMatrix;
			
			GenerateTargetTransform();					
		}
		break;

	case EMatrixDemoType::E_Orthogonal_X:
		{
			FMatrix OrthogonalProjectionMatrix = FMatrix::Identity;
			// X축 방향으로 투영 (X 좌표 0으로 고정)
			OrthogonalProjectionMatrix.M[0][0] = 0.0f;

			ResultMatrix = FMatrix::Identity;
			ResultMatrix = TranslationMatrix * OrthogonalProjectionMatrix * RotationMatrix * ScaleMatrix;
			
			GenerateTargetTransform();
		}
		break;

	case EMatrixDemoType::E_Orthogonal_Y:
		{
			FMatrix OrthogonalProjectionMatrix = FMatrix::Identity;
			// Y축 방향으로 투영 (Y 좌표 0으로 고정)
			OrthogonalProjectionMatrix.M[1][1] = 0.0f;

			ResultMatrix = FMatrix::Identity;
			ResultMatrix = TranslationMatrix * OrthogonalProjectionMatrix * RotationMatrix * ScaleMatrix;
			
			GenerateTargetTransform();
		}
		break;

		
	case EMatrixDemoType::E_Orthogonal_Z:
		{
			FMatrix OrthogonalProjectionMatrix = FMatrix::Identity;
			// Z축 방향으로 투영 (Z 좌표 0으로 고정)
			OrthogonalProjectionMatrix.M[2][2] = 0.0f;

			ResultMatrix = FMatrix::Identity;
			ResultMatrix = TranslationMatrix * OrthogonalProjectionMatrix * RotationMatrix * ScaleMatrix;
			
			GenerateTargetTransform();
		}
		break;

	default:
		{
			ResultMatrix = FMatrix::Identity;
    	
			SetActorLocation(FVector::ZeroVector);
			SetActorRotation(FRotator::ZeroRotator);
			SetActorScale3D(FVector::OneVector);
		}
		break;
	}
}

void AMatrixActor::GenerateTargetTransform()
{
	TargetLocation = ResultMatrix.GetOrigin();

	const FVector ScaledX = ResultMatrix.GetScaledAxis(EAxis::X);
	const FVector ScaledY = ResultMatrix.GetScaledAxis(EAxis::Y);
	const FVector ScaledZ = ResultMatrix.GetScaledAxis(EAxis::Z);

	const FVector RotationX = ScaledX / Scale.X;
	const FVector RotationY = ScaledY / Scale.Y;
	const FVector RotationZ = ScaledZ / Scale.Z;

	FMatrix RotationOnly;
	RotationOnly.SetAxes(&RotationX, &RotationY, &RotationZ);
	RotationOnly.SetOrigin(FVector::ZeroVector); // 위치 정보는 0으로 설정
	TargetRotation = RotationOnly.Rotator();
	TargetQuat = TargetRotation.Quaternion();

	TargetScale.X = ResultMatrix.GetScaledAxis(EAxis::X).Size();
	TargetScale.Y = ResultMatrix.GetScaledAxis(EAxis::Y).Size();
	TargetScale.Z = ResultMatrix.GetScaledAxis(EAxis::Z).Size();

	TargetTransform.SetLocation(TargetLocation);
	TargetTransform.SetRotation(TargetQuat);
	TargetTransform.SetScale3D(TargetScale);
}

void AMatrixActor::UpdateTransform(float DeltaTime)
{
	// LerpAlpha 증가 (0~1)
	LerpAlpha = FMath::Clamp(LerpAlpha + DeltaTime / Duration , 0.f, 1.f);

	// 보간된 Transform 계산 (StartTransform과 TargetTransform 사이)
	CurrentTransform.Blend(CurrentTransform, TargetTransform, LerpAlpha);

	// 결과 적용
	SetActorTransform(CurrentTransform);

	// Lerp 완료 시 리셋(필요시)
	if (LerpAlpha >= 1.f)
	{
		LerpAlpha = 0.f;
		CurrentTransform = TargetTransform;
	}
}