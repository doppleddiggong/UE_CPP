// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->UpdateMove(DeltaTime);
}

void ABullet::UpdateMove(float DeltaTime)
{
	auto Move = GetActorUpVector();
	Move = Move.GetSafeNormal() * MoveSpd * DeltaTime;
	
	this->SetActorLocation( GetActorLocation() + Move );
}