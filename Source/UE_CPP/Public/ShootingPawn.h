// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "ShootingPawn.generated.h"

UCLASS()
class UE_CPP_API AShootingPawn : public APawn
{
	GENERATED_BODY()

public:
	AShootingPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateMove(const float DeltaTime);
	void OnInputMove(const FInputActionValue& Value);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	float Horizontal;
	float Vertical;
};
