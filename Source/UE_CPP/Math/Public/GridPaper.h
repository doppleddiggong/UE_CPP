// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridPaper.generated.h"

UCLASS()
class UE_CPP_API AGridPaper : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridPaper();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category="Grid")
	FIntVector2 PointLocation;

	UFUNCTION(CallInEditor, Category="Grid")
	void SetPointLocation();

	UFUNCTION(BlueprintCallable, Category="Grid")
	void UpdatePointLocation( const FIntVector2& Location );

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Grid")
	FIntVector2 GetPointLocation();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* PointComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* ArrowComponent;
};
