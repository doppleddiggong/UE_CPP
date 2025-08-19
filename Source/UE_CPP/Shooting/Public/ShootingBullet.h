// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingBullet.generated.h"

UCLASS()
class UE_CPP_API AShootingBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	AShootingBullet();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnBoxCompBeginOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	
	void ReturnToPool();

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> BoxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFX;

	UPROPERTY(EditAnywhere)
	class USoundBase* ExplosionSound; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float MoveSpeed = 100.0f;


private:
	FTimerHandle TimerHandle_AutoReturn;
};
