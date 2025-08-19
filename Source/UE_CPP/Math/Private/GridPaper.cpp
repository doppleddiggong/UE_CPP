// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "GridPaper.h"

#include "ULog.h"
#include "FComponentHelper.h"

#include "Components/ArrowComponent.h"

#define POINT_PATH TEXT("Point")
#define POINT_ARROW_PATH TEXT("Point_Arrow")

AGridPaper::AGridPaper()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGridPaper::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	PointComponent = FComponentHelper::FindComponentByNameRecursive<USceneComponent>(this, POINT_PATH);
	ArrowComponent = FComponentHelper::FindComponentByNameRecursive<UArrowComponent>(this, POINT_ARROW_PATH);
}

void AGridPaper::BeginPlay()
{
	Super::BeginPlay();
}

void AGridPaper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridPaper::SetPointLocation()
{
	this->UpdatePointLocation( PointLocation );
}

void AGridPaper::UpdatePointLocation(const FIntVector2& Location)
{
	if ( PointComponent == nullptr)
	{
		ULOG(Warning, "PointComponent == nullptr" );
		return;
	}
	
	FVector NewLocation( Location.X, Location.Y, 0.0f );
	PointComponent->SetRelativeLocation( NewLocation);
	
	FVector OriginLocation = ArrowComponent->GetComponentLocation();
	FVector TargetLocation = PointComponent->GetComponentLocation();
	FVector Direction = TargetLocation - OriginLocation;
	float Distance = Direction.Size();
	ArrowComponent->ArrowLength = Distance;

	FRotator NewRotation = Direction.Rotation();
	ArrowComponent->SetWorldRotation(NewRotation);
	ArrowComponent->MarkRenderStateDirty();
}

FIntVector2 AGridPaper::GetPointLocation()
{
	if ( PointComponent == nullptr)
	{
		ULOG(Warning, "PointComponent == nullptr" );
		return FIntVector2(0,0) ;
	}

	const auto Location = PointComponent->GetRelativeLocation();

	int32 X = FMath::RoundToInt32(Location.X);
	int32 Y = FMath::RoundToInt32(Location.Y);
	return FIntVector2(X, Y);
}