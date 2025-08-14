// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "Bullet.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->UpdateMove(DeltaTime);
}

void ABullet::UpdateMove(const float DeltaTime)
{
	auto MoveDirection = GetActorUpVector();
	MoveDirection = MoveDirection.GetSafeNormal() * MoveSpd * DeltaTime;
	
	this->SetActorLocation( GetActorLocation() + MoveDirection );
}