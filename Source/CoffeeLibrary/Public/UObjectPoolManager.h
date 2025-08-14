// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObjectPoolManager.generated.h"

UCLASS()
class COFFEELIBRARY_API UObjectPoolManager : public UObject
{
    GENERATED_BODY()

public:	
    UObjectPoolManager();

private:	
    static UObjectPoolManager* Instance;
    
    TMap<UClass*, TArray<AActor*>> PoolMap;

public:
    UFUNCTION(BlueprintCallable, Category="Pool")
    static UObjectPoolManager* Get();

    UFUNCTION(BlueprintCallable, Category="Pool")
    AActor* GetPoolItem(UWorld* World, const TSubclassOf<AActor> InClass );

    UFUNCTION(BlueprintCallable, Category="Pool")
    void ReturnActorToPool(AActor* Actor);
};