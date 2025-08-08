// Fill out your copyright notice in the Description page of Project Settings.


#include "CoadingPawn.h"

// Sets default values
ACoadingPawn::ACoadingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACoadingPawn::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
	UE_LOG(LogTemp, Warning, TEXT("Number : %d"), Number);
	// 결과: "9,999" (en-US 기준)
	FText LocalizedNumber = FText::AsNumber(Number);
	UE_LOG(LogTemp, Warning, TEXT("#,##0 : %s"), *LocalizedNumber.ToString());
	UE_LOG(LogTemp, Warning, TEXT("pi : %f"), Pi);
	UE_LOG(LogTemp, Warning, TEXT("longValue : %ld"), LongValue);
	UE_LOG(LogTemp, Warning, TEXT("doubleValue : %f"), DoubleValue);
	UE_LOG(LogTemp, Warning, TEXT("isGood : %d"), IsGood);
	UE_LOG(LogTemp, Warning, TEXT("myName : %s"), *MyName);
	UE_LOG(LogTemp, Warning, TEXT("myName : %s"), TEXT("배주백"));
}




// Called every frame
void ACoadingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACoadingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int32 ACoadingPawn::NewMyAdd(int32 a, int32 b)
{
	return a + b;
}

