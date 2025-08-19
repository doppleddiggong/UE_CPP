// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "USequenceActivatable.generated.h"

UINTERFACE(BlueprintType)
class COFFEELIBRARY_API USequenceActivatable : public UInterface
{
	GENERATED_BODY()
};

class COFFEELIBRARY_API ISequenceActivatable
{
	GENERATED_BODY()

public:
	/** Duration 동안 연출 수행 (구현체에서 Timeline.PlayRate = Len/Duration 등) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Sequence")
	void Activate(float Duration);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Sequence")
	void Deactivate();
};