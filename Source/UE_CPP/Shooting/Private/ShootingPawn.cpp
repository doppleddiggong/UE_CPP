// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "ShootingPawn.h"
#include "UObjectPoolManager.h"
#include "UGameEventManager.h"
#include "ShootingBullet.h"
#include "ShootingEnemy.h"
#include "FComponentHelper.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

#define MESH_STATIC_MESH		TEXT("/Engine/VREditor/BasicMeshes/SM_Cube_02.SM_Cube_02")
#define MESH_MATERIAL			TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse")

#define EXPLOSION_VFX			TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion")
#define EXPLOSION_SOUND			TEXT("/Game/Shooting/Sounds/Explosion.Explosion")

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

	MeshComp->SetStaticMesh( FComponentHelper::LoadAsset<UStaticMesh>(MESH_STATIC_MESH));
	MeshComp->SetMaterial(0, FComponentHelper::LoadAsset<UMaterial>(MESH_MATERIAL));
	MeshComp->SetGenerateOverlapEvents(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("Player"));

	ExplosionVFX = FComponentHelper::LoadAsset<UParticleSystem>(EXPLOSION_VFX);
	ExplosionSound = FComponentHelper::LoadAsset<USoundBase>(EXPLOSION_SOUND);
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

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AShootingPawn::OnBoxCompBeginOverlap );
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
	AActor* Actor = PoolManager->GetPoolItemLocationRotator(this, BulletToSpawn,
		FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation() );
	
	if (AShootingBullet* ShootingBullet = Cast<AShootingBullet>(Actor))
	{
		ShootingBullet->SetOwner(this);
		ShootingBullet->SetInstigator( GetInstigator() );

		UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AShootingBullet Cast Fail") );
	}
}

void AShootingPawn::OnBoxCompBeginOverlap(
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
		this->Destroy();
	}
}