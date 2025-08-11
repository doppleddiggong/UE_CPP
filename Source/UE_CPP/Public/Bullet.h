// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class UE_CPP_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="이동 속도"))
	float MoveSpd = 1.0f;


private:
	void UpdateMove(float DeltaTime);
};
