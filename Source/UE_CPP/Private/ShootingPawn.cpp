// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "ShootingPawn.h"

AShootingPawn::AShootingPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp ->SetupAttachment(RootComponent);
}

void AShootingPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AShootingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShootingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

