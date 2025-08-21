// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "ShootingEnemy.h"
#include "Features/UObjectPoolManager.h"
#include "Shared/FComponentHelper.h"
#include "ShootingPawn.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#define MESH_STATIC_MESH		TEXT("/Engine/VREditor/BasicMeshes/SM_Cube_02.SM_Cube_02")
#define MESH_MATERIAL			TEXT("/Game/StarterContent/Materials/M_Wood_Pine.M_Wood_Pine")

#define PLAYER_CHANNEL	ECollisionChannel::ECC_GameTraceChannel1
#define ENEMY_CHANNEL	ECollisionChannel::ECC_GameTraceChannel2
#define BULLET_CHANNEL	ECollisionChannel::ECC_GameTraceChannel3

// Sets default values
AShootingEnemy::AShootingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent( FVector(50,50,50) );
	SetRootComponent(BoxComp);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	MeshComp->SetStaticMesh( FComponentHelper::LoadAsset<UStaticMesh>(MESH_STATIC_MESH));
	MeshComp->SetMaterial(0, FComponentHelper::LoadAsset<UMaterial>(MESH_MATERIAL));
	
	MeshComp->SetGenerateOverlapEvents(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("Enemy"));
}

void AShootingEnemy::BeginPlay()
{
	Super::BeginPlay();

	MoveDirection = FVector(0, 0, -1.0f);
	int32 rand = FMath::RandRange(0,9 );
	if( rand < 3 )
	{
		auto Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if ( Player != nullptr )
		{
			MoveDirection = Player->GetActorLocation() - this->GetActorLocation();
			MoveDirection.Normalize();
		}
	}
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AShootingEnemy::OnBoxCompBeginOverlap);
}

void AShootingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->UpdateMove(DeltaTime);
}

void AShootingEnemy::OnBoxCompBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	auto* Player = Cast<AShootingPawn>(OtherActor);

	if ( IsValid(Player))
	{
		Player->Destroy();
		ReturnToPool();
	}
}

void AShootingEnemy::UpdateMove(const float DeltaTime)
{
	// P = P0 + vt;
	FVector P0 = GetActorLocation();
	// FVector Direction = FVector(0, 0, -1.0f);
	// Direction.Normalize();
	FVector Velocity = MoveDirection * MoveSpeed;

	this->SetActorLocation( P0 + Velocity * DeltaTime );
}

void AShootingEnemy::ReturnToPool()
{
	UObjectPoolManager* PoolManager = GetGameInstance()->GetSubsystem<UObjectPoolManager>();
	if (PoolManager != nullptr )
		PoolManager->ReturnActorToPool(this);
}