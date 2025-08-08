// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CoadingPawn.generated.h"

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	int32 Number = 123456789;

	UPROPERTY(EditInstanceOnly )
	float Pi = 3.14f;

	UPROPERTY(EditAnywhere)
	int64 LongValue = LONG_MAX;

	UPROPERTY(VisibleDefaultsOnly )
	double DoubleValue = DBL_MAX;

	UPROPERTY(VisibleInstanceOnly )
	bool IsGood = false;

	UPROPERTY(VisibleAnywhere )
	FString MyName = TEXT("dopple");

	UFUNCTION(BlueprintCallable, Category="ACoadingPawn")
	int32 NewMyAdd(int32 a, int32 b);

	UFUNCTION(BlueprintCallable, Category="ACoadingPawn")
	FORCEINLINE int32 MyAdd(int32 a, int32 b)
	{
		return a + b;
	}
};
