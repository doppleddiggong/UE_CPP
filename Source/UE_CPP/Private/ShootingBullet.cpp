// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "ShootingBullet.h"

#include "Components/BoxComponent.h"

AShootingBullet::AShootingBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(37.5f,37.5f,37.5f));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetWorldScale3D(FVector(0.75f));
}

void AShootingBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AShootingBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Direction = this->GetActorUpVector().GetSafeNormal();
	AddActorWorldOffset(Direction * MoveSpeed * DeltaTime);
}

