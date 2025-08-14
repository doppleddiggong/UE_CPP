#pragma once

#include "CoreMinimal.h"
#include "UGameEventManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnScore, int);

UCLASS()
class COFFEELIBRARY_API UGameEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UGameEventManager* Get(UObject* WorldContext)
	{
		if (!WorldContext)
			return nullptr;
		
		if (auto* GI = WorldContext->GetWorld()->GetGameInstance())
			return GI->GetSubsystem<UGameEventManager>();
		return nullptr;
	}
	
	FOnScore OnScore;
};
