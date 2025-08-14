// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "ShootingEnemy.h"

#include "ShootingPawn.h"
#include "Components/BoxComponent.h"

// Sets default values
AShootingEnemy::AShootingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent( FVector(50,50,50) );
	SetRootComponent(BoxComp);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Engine/VREditor/BasicMeshes/SM_Cube_02.SM_Cube_02"));
	if ( TempMesh.Succeeded())
		MeshComp->SetStaticMesh( TempMesh.Object );

	ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial(TEXT("/Game/StarterContent/Materials/M_Wood_Pine.M_Wood_Pine"));
	if ( TempMaterial.Succeeded() )
		MeshComp->SetMaterial (0, TempMaterial.Object );
}

void AShootingEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AShootingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->UpdateMove(DeltaTime);
}

void AShootingEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	auto* Player = Cast<AShootingPawn>(OtherActor);
	if ( IsValid(Player))
	{
		Player->Destroy();
		this->Destroy();
	}
}

void AShootingEnemy::UpdateMove(const float DeltaTime)
{
	// P = P0 + vt;
	FVector P0 = GetActorLocation();
	FVector Direction = FVector(0, 0, -1.0f);
	Direction.Normalize();
	FVector Velocity = Direction * MoveSpeed;

	this->SetActorLocation( P0 + Velocity * DeltaTime );
}


