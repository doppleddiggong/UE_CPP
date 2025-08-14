// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "UObjectPoolManager.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UObjectPoolManager* UObjectPoolManager::Instance = nullptr;

UObjectPoolManager::UObjectPoolManager()
{
#if WITH_EDITOR
	FEditorDelegates::EndPIE.AddLambda([](const bool bIsSimulating)
	{
		Instance = nullptr;
	});
#endif
}

UObjectPoolManager* UObjectPoolManager::Get()
{
	if ( !IsValid(Instance))
	{
		Instance = NewObject<UObjectPoolManager>();
		Instance->AddToRoot();
	}
	return Instance;
}

AActor* UObjectPoolManager::GetPoolItem(UWorld* World, const TSubclassOf<AActor> InClass )
{
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("GetPoolItem failed: World is invalid."));
		return nullptr;
	}

	if (!InClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GetPoolItem failed: InClass is null."));
		return nullptr;
	}
	
	TArray<AActor*>& Pool = PoolMap.FindOrAdd(InClass);

	for (AActor* Actor : Pool)
	{
		if ( Actor != nullptr && Actor->IsActorTickEnabled() == false)
		{
			Actor->SetActorHiddenInGame(false);
			Actor->SetActorEnableCollision(true);
			Actor->SetActorTickEnabled(true);
			return Actor;
		}
	}

	AActor* NewActor = World->SpawnActor<AActor>(InClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if ( NewActor != nullptr )
	{
		Pool.Add(NewActor);
		return NewActor;
	}

	return nullptr;
}

void UObjectPoolManager::ReturnActorToPool(AActor* Actor)
{
	if ( Actor == nullptr )
		return;
        
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
}