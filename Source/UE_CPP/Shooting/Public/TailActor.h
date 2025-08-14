// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TailActor.generated.h"

UCLASS()
class UE_CPP_API ATailActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATailActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> BoxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	// UPROPERTY()
	// TObjectPtr<class APawn> PlayerPawn;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AActor> TargetActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float MoveSpeed = 450.0f;
};
