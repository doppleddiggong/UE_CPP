// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManagerActor.generated.h"

UCLASS()
class UE_CPP_API AEnemyManagerActor : public AActor
{
	GENERATED_BODY()

public:
	AEnemyManagerActor();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void MakeEnemy();

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShootingEnemy> EnemyFactory;

	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* SpawnPoint;

private:
	FTimerHandle MakeEnemyTimerHandle;
};
