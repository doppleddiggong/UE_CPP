// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "Entity.h"
#include "SceneView.h"

AEntity::AEntity()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEntity::BeginPlay()
{
	Super::BeginPlay();

	this->InitData();
}

void AEntity::InitData()
{
	CurrentOrder = ETransformOrder::E_Identity;
	PreviousTransformOrder = CurrentOrder;

	TranslationMatrix = FTranslationMatrix(Translation);
	RotationMatrix = FRotationMatrix::Make(Rotation);
	ScaleMatrix = FScaleMatrix(Scale);

	// 일단 초기화
	SetActorLocation(FVector::ZeroVector);
	SetActorRotation(FRotator::ZeroRotator);
	SetActorScale3D(FVector::OneVector);

	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
	StartScale = GetActorScale3D();

	TargetLocation = StartLocation;
	TargetRotation = StartRotation;
	TargetScale = StartScale;
}

void AEntity::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PreviousTransformOrder != CurrentOrder)
	{
		PreviousTransformOrder = CurrentOrder;

		TranslationMatrix = FTranslationMatrix(Translation);
		RotationMatrix = FRotationMatrix::Make(Rotation);
		ScaleMatrix = FScaleMatrix(Scale);

		// 일단 초기화
		SetActorLocation(FVector::ZeroVector);
		SetActorRotation(FRotator::ZeroRotator);
		SetActorScale3D(FVector::OneVector);
		
		ApplyTransformation();
	}
	
	this->UpdateTransformInterpolation(DeltaTime);
}

void AEntity::ApplyTransformation()
{
    if (GEngine)
    {
       FString Msg = FString::Printf(TEXT("Current Transform Order: %s"), *GetTransformModeName(CurrentOrder));
       GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Orange, Msg);
    }

    FMatrix ResultMatrix;

    switch (CurrentOrder)
    {
    case ETransformOrder::E_Identity:
       ResultMatrix = FMatrix::Identity;
    	
       SetActorLocation(FVector::ZeroVector);
       SetActorRotation(FRotator::ZeroRotator);
       SetActorScale3D(FVector::OneVector);
       break;
       
    case ETransformOrder::E_TRS:
    	ResultMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

		TransformOrder[0] = ETransformType::Translate;
    	TransformOrder[1] = ETransformType::Rotate;
    	TransformOrder[2] = ETransformType::Scale;
    	break;

    case ETransformOrder::E_TSR:
    	ResultMatrix = TranslationMatrix * ScaleMatrix * RotationMatrix;

		TransformOrder[0] = ETransformType::Translate;
		TransformOrder[1] = ETransformType::Scale;
		TransformOrder[2] = ETransformType::Rotate;
    	break;

    case ETransformOrder::E_RTS:
    	ResultMatrix = RotationMatrix * TranslationMatrix * ScaleMatrix;

    	TransformOrder[0] = ETransformType::Rotate;
		TransformOrder[1] = ETransformType::Translate;
		TransformOrder[2] = ETransformType::Scale;
    	break;

    case ETransformOrder::E_SRT:
    	ResultMatrix = ScaleMatrix * RotationMatrix * TranslationMatrix;

    	TransformOrder[0] = ETransformType::Scale;
    	TransformOrder[1] = ETransformType::Rotate;
    	TransformOrder[2] = ETransformType::Translate;
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

	StartTransformInterpolation(ResultMatrix);
	
	// --- 시각적인 차이를 명확히 보여주기 위한 디버그 출력 추가 ---
	if (GEngine)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("Location: %s"), *NewLocation.ToString()));
		// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Rotation: %s"), *NewRotation.Rotator().ToString()));
		// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Scale: %s"), *NewScale.ToString()));

		FString Debug_ResultMatrix = ResultMatrix.ToString();
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("TRS Matrix: %s"), *Debug_ResultMatrix));
	}
	// -----------------------------------------------------------------
}

void AEntity::StartTransformInterpolation(const FMatrix& ResultMatrix)
{
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
	StartScale = GetActorScale3D();

	TargetLocation = ResultMatrix.GetOrigin();

	// ResultMatrix에서 스케일이 곱해진 축을 가져온다.
	FVector ScaledX = ResultMatrix.GetScaledAxis(EAxis::X);
	FVector ScaledY = ResultMatrix.GetScaledAxis(EAxis::Y);
	FVector ScaledZ = ResultMatrix.GetScaledAxis(EAxis::Z);
	// 스케일로 나누어 순수한 회전 축 벡터를 얻는다.
	FVector RotationX = ScaledX / Scale.X;
	FVector RotationY = ScaledY / Scale.Y;
	FVector RotationZ = ScaledZ / Scale.Z;
	// 회전 행렬을 생성 (SetAxes 함수를 사용)
	FMatrix RotationOnly;
	RotationOnly.SetAxes(&RotationX, &RotationY, &RotationZ);
	RotationOnly.SetOrigin(FVector::ZeroVector); // 위치 정보는 0으로 설정
	TargetRotation = RotationOnly.Rotator();

	TargetScale.X = ResultMatrix.GetScaledAxis(EAxis::X).Size();
	TargetScale.Y = ResultMatrix.GetScaledAxis(EAxis::Y).Size();
	TargetScale.Z = ResultMatrix.GetScaledAxis(EAxis::Z).Size();

	InterpElapsedTime = 0.f;
	InterpStep = 0;
}

// Tick에서 호출
void AEntity::UpdateTransformInterpolation(float DeltaTime)
{
	if (InterpStep >= TransformOrder.Num())
		return;

	InterpElapsedTime += DeltaTime;
	float Alpha = FMath::Clamp(InterpElapsedTime / StepDuration, 0.f, 1.f);

	switch (TransformOrder[InterpStep])
	{
	case ETransformType::Translate:
		{
			FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
			SetActorLocation(NewLocation);

			if (Alpha >= 1.f)
			{
				InterpStep++;
				InterpElapsedTime = 0.f;

				// 다음 변환 단계 초기값 갱신
				if (InterpStep < TransformOrder.Num())
				{
					switch (TransformOrder[InterpStep])
					{
					case ETransformType::Rotate:
						StartRotation = GetActorRotation();
						break;
					case ETransformType::Scale:
						StartScale = GetActorScale3D();
						break;
					default:
						break;
					}
				}
			}
		}
		break;

	case ETransformType::Rotate:
		{
			FQuat StartQuat = StartRotation.Quaternion();
			FQuat TargetQuat = TargetRotation.Quaternion();
			FQuat NewQuat = FQuat::Slerp(StartQuat, TargetQuat, Alpha);
			SetActorRotation(NewQuat);

			if (Alpha >= 1.f)
			{
				InterpStep++;
				InterpElapsedTime = 0.f;

				if (InterpStep < TransformOrder.Num())
				{
					switch (TransformOrder[InterpStep])
					{
					case ETransformType::Translate:
						StartLocation = GetActorLocation();
						break;
					case ETransformType::Scale:
						StartScale = GetActorScale3D();
						break;
					default:
						break;
					}
				}
			}
		}
		break;

	case ETransformType::Scale:
		{
			FVector NewScale = FMath::Lerp(StartScale, TargetScale, Alpha);
			SetActorScale3D(NewScale);

			if (Alpha >= 1.f)
			{
				InterpStep++;
				InterpElapsedTime = 0.f;

				if (InterpStep < TransformOrder.Num())
				{
					switch (TransformOrder[InterpStep])
					{
					case ETransformType::Translate:
						StartLocation = GetActorLocation();
						break;
					case ETransformType::Rotate:
						StartRotation = GetActorRotation();
						break;
					default:
						break;
					}
				}
			}
		}
		break;
	}
}