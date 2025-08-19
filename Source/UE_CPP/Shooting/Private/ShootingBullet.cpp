// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "ShootingBullet.h"
#include "ShootingEnemy.h"
#include "UObjectPoolManager.h"
#include "FComponentHelper.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#define EXPLOSION_VFX		TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion")
#define EXPLOSION_SOUND		TEXT("/Game/Shooting/Sounds/Explosion.Explosion")

AShootingBullet::AShootingBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(37.5f,37.5f,37.5f));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetWorldScale3D(FVector(0.75f));

	MeshComp->SetGenerateOverlapEvents(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("Bullet"));

	ExplosionVFX = FComponentHelper::LoadAsset<UParticleSystem>(EXPLOSION_VFX);
	ExplosionSound = FComponentHelper::LoadAsset<USoundBase>(EXPLOSION_SOUND);
}

void AShootingBullet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	this->ReturnToPool();
}

void AShootingBullet::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		TimerHandle_AutoReturn,
		this,
		&AShootingBullet::ReturnToPool,
		2.0f,      // 2초
		false      // 반복 없음
	);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AShootingBullet::OnBoxCompBeginOverlap );
}

void AShootingBullet::ReturnToPool()
{
	UE_LOG(LogTemp, Warning, TEXT("ReturnToPool") );

	if (GetWorldTimerManager().IsTimerActive(TimerHandle_AutoReturn))
		GetWorldTimerManager().ClearTimer(TimerHandle_AutoReturn);
	
	UObjectPoolManager* PoolManager = GetGameInstance()->GetSubsystem<UObjectPoolManager>();
	if (PoolManager != nullptr )
		PoolManager->ReturnActorToPool(this);
}

void AShootingBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Direction = this->GetActorUpVector().GetSafeNormal();
	AddActorWorldOffset(Direction * MoveSpeed * DeltaTime);
}

void AShootingBullet::OnBoxCompBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	auto* Enemy = Cast<AShootingEnemy>(OtherActor);
	if ( IsValid(Enemy))
	{
		UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), ExplosionVFX, GetActorLocation() );
		UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);

		Enemy->ReturnToPool();
		this->ReturnToPool();
	}
}