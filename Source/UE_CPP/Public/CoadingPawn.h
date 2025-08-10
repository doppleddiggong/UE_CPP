// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "CoadingPawn.generated.h"

struct FInputActionValue;

UCLASS()
class UE_CPP_API ACoadingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACoadingPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	UPROPERTY()
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="이동 속도"))
	float MoveSpd = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* HorizontalAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* VerticalAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta=(ToolTip="발사 위치"))
	USceneComponent* FirePos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta=(ToolTip="생설될 총알 객체"))
	TSubclassOf<AActor> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta=(ToolTip="발사 효과음"))
	class USoundBase* FireSound;

private:
	void SetupInputMapping() const;
	void OnHorizontalAction(const FInputActionValue& Value);
	void OnVerticalAction(const FInputActionValue& Value);
	void OnFireAction(const FInputActionValue& Value);

	void UpdateMove(float DeltaTime);

	float HorizontalValue;
	float VerticalValue;
};
