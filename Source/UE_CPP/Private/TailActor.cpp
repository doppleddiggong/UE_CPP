// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "TailActor.h"
#include "CoffeeLibrary.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ATailActor::ATailActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(37.5f,37.5f,37.5f));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetWorldScale3D(FVector(0.75f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Engine/VREditor/BasicMeshes/SM_Cube_02.SM_Cube_02"));
	if ( TempMesh.Succeeded())
		MeshComp->SetStaticMesh( TempMesh.Object );

	ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse"));
	if ( TempMaterial.Succeeded() )
		MeshComp->SetMaterial (0, TempMaterial.Object );
}

void ATailActor::BeginPlay()
{
	Super::BeginPlay();

	if ( TargetActor == nullptr )
	{
		TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s : %d"),
		*this->GetActorLabel(),
		UCoffeeCommonUtil::GetFirstNumberInActorLabel(this));
}

void ATailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( TargetActor == nullptr )
		return;
	
	auto Direction = (TargetActor->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
	AddActorWorldOffset(Direction * MoveSpeed * DeltaTime);
}