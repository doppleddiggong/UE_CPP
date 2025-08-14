// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/Function.h"
#include "EMsgMode.h"
#include "UMessenger.generated.h"

template<typename T>
void AddListener(EMSG_ID MsgID, TFunction<void(T)> Listener);

template<typename T>
void Broadcast(EMSG_ID MsgID, T Arg);

template<typename T, typename U>
void AddListener(EMSG_ID MsgID, TFunction<void(T, U)> Listener);

template<typename T, typename U>
void Broadcast(EMSG_ID MsgID, T Arg1, U Arg2);

UCLASS()
class COFFEELIBRARY_API UMessenger : public UObject
{
	GENERATED_BODY()

public:
	UMessenger();
	
	static UMessenger* Get();

	void RemoveAllListeners();

	void AddListener(EMSG_ID MsgID, TFunction<void()> Listener);
	void RemoveListener(EMSG_ID MsgID, TFunction<void()> Listener);
	void Broadcast(EMSG_ID MsgID, EMSG_MODE Mode = EMSG_MODE::DONT_REQUIRE_LISTENER);

private:
	TMap<EMSG_ID, TArray<TFunction<void()>>> EventMap;

	static UMessenger* Instance;
};