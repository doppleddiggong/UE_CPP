// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "ShootingPawn.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AShootingPawn::AShootingPawn()
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

	ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse"));
	if ( TempMaterial.Succeeded() )
		MeshComp->SetMaterial (0, TempMaterial.Object );
}

void AShootingPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer() );
	if ( SubSystem != nullptr )
		SubSystem->AddMappingContext(IMC_Player,0);
}

void AShootingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->UpdateMove(DeltaTime);
}

void AShootingPawn::UpdateMove(const float DeltaTime)
{
	// P = P0 + vt;
	FVector P0 = GetActorLocation();
	FVector Direction = FVector(0, Horizontal, Vertical);
	Direction.Normalize();
	FVector Velocity = Direction * MoveSpeed;

	this->SetActorLocation( P0 + Velocity * DeltaTime );

	Horizontal = Vertical = 0;
}

void AShootingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* IC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	IC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AShootingPawn::OnInputMove);
}

void AShootingPawn::OnInputMove(const FInputActionValue& Value)
{
	auto V = Value.Get<FVector2D>();
	this->Vertical = V.X;
	this->Horizontal = V.Y;
}