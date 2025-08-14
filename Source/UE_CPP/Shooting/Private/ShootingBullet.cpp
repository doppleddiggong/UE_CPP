// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "ShootingBullet.h"

#include "ShootingEnemy.h"
#include "UObjectPoolManager.h"

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

	// 2초 후에 반환
	GetWorldTimerManager().SetTimer(
		TimerHandle_AutoReturn,
		this,
		&AShootingBullet::ReturnToPool,
		2.0f,      // 2초
		false      // 반복 없음
	);
}

void AShootingBullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	auto* Enemy = Cast<AShootingEnemy>(OtherActor);
	if ( IsValid(Enemy))
	{
		Enemy->Destroy();
		// this->Destroy();
		ReturnToPool();
	}
}

void AShootingBullet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	this->ReturnToPool();
}

void AShootingBullet::ReturnToPool()
{
	UObjectPoolManager::Get()->ReturnActorToPool(this);
}

void AShootingBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Direction = this->GetActorUpVector().GetSafeNormal();
	AddActorWorldOffset(Direction * MoveSpeed * DeltaTime);
}

