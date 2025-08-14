// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "ShootingPawn.h"
#include "UObjectPoolManager.h"
#include "UGameEventManager.h"
#include "ShootingBullet.h"
#include "ShootingEnemy.h"

#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

AShootingPawn::AShootingPawn()
{ 
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent( FVector(50,50,50) );
	SetRootComponent(BoxComp);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePointComp"));
	FirePoint->SetupAttachment(MeshComp);

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

	this->ShotCount = 0;
	this-> Score = 0;
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer() );
	if ( SubSystem != nullptr )
		SubSystem->AddMappingContext(IMC_Player,0);


	UGameEventManager::Get(this)->OnScore.AddLambda([&](int InScore) {
		this->Score += InScore;
		UE_LOG(LogTemp, Log, TEXT("Game Score: %d"), this->Score);
	});
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

	if ( bAutoFire )
	{
		FireRate -= DeltaTime;
		if ( FireRate < 0)
		{
			Fire();
		}
	}
}

void AShootingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* IC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	IC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AShootingPawn::OnInputMove);
	IC->BindAction(IA_Fire, ETriggerEvent::Started, this, &AShootingPawn::OnInputFirePress);
	IC->BindAction(IA_Fire, ETriggerEvent::Completed, this, &AShootingPawn::OnInputFireRelease);
}

void AShootingPawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	auto* Enemy = Cast<AShootingEnemy>(OtherActor);
	if ( IsValid(Enemy))
	{
		Enemy->Destroy();
		this->Destroy();
	}	
}

void AShootingPawn::OnInputMove(const FInputActionValue& Value)
{
	auto V = Value.Get<FVector2D>();
	this->Vertical = V.X;
	this->Horizontal = V.Y;
}

void AShootingPawn::OnInputFirePress(const FInputActionValue& Value)
{
	bAutoFire = true;

	Fire();
}

void AShootingPawn::OnInputFireRelease(const FInputActionValue& Value)
{
	bAutoFire = false;
}

void AShootingPawn::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire") );
	FireRate = FireDelay;
	ShotCount++;

	UObjectPoolManager* PoolManager = GetGameInstance()->GetSubsystem<UObjectPoolManager>();
	if (!PoolManager)
	{
		UE_LOG(LogTemp, Error, TEXT("AShootingPawn::Fire() failed: UObjectPoolManager subsystem not found."));
		return;
	}

	// 풀에서 아이템을 가져옵니다.
	TSubclassOf<AActor> BulletToSpawn = BulletClass[ShotCount % BulletClass.Num()];
	AActor* Actor = PoolManager->GetPoolItem(this, BulletToSpawn);
	
	if (AShootingBullet* ShootingBullet = Cast<AShootingBullet>(Actor))
	{
		ShootingBullet->SetOwner(this);
		ShootingBullet->SetInstigator( GetInstigator() );
		ShootingBullet->SetActorLocationAndRotation(FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation());

		UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
	}
}
