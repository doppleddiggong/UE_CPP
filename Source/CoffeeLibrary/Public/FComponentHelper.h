// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

/**
 * 액터 컴포넌트 관련 유틸리티 함수들을 제공하는 헬퍼 클래스입니다.
 */
class FComponentHelper
{
public:
	/**
	 * 액터의 컴포넌트들 중에서 지정된 이름의 컴포넌트를 찾습니다.
	 * 첫 번째로 찾은 컴포넌트를 반환하며, 하위 컴포넌트는 검색하지 않습니다.
	 * * @param Owner 컴포넌트를 찾을 액터입니다.
	 * @param ComponentName 찾고자 하는 컴포넌트의 이름입니다.
	 * @return 지정된 이름의 컴포넌트를 찾으면 해당 포인터를, 없으면 nullptr을 반환합니다.
	 */
	template<typename T>
	static T* FindComponentByName(AActor* Owner, const FName& ComponentName)
	{
		if (!Owner)
		{
			return nullptr;
		}

		TArray<T*> FoundComponents;
		Owner->GetComponents(FoundComponents);

		for (T* Component : FoundComponents)
		{
			if (Component && Component->GetFName() == ComponentName)
			{
				return Component;
			}
		}
		return nullptr;
	}

	/**
	 * 액터의 컴포넌트들과 그 하위 컴포넌트들까지 재귀적으로 검색하여 지정된 이름의 컴포넌트를 찾습니다.
	 * * @param Owner 컴포넌트를 찾을 액터입니다.
	 * @param ComponentName 찾고자 하는 컴포넌트의 이름입니다.
	 * @return 지정된 이름의 컴포넌트를 찾으면 해당 포인터를, 없으면 nullptr을 반환합니다.
	 */
	template<typename T>
	static T* FindComponentByNameRecursive(AActor* Owner, const FName& ComponentName)
	{
		if (!Owner)
		{
			return nullptr;
		}

		TArray<UActorComponent*> Components;
		Owner->GetComponents(Components, true);

		for (UActorComponent* Component : Components)
		{
			if (Component && Component->GetFName() == ComponentName)
			{
				return Cast<T>(Component);
			}
		}
		return nullptr;
	}

private:
	FComponentHelper() {}
};