// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "UMessenger.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UMessenger* UMessenger::Instance = nullptr;

UMessenger::UMessenger()
{
#if WITH_EDITOR
	FEditorDelegates::EndPIE.AddLambda([](const bool bIsSimulating)
	{
		Instance = nullptr;
	});
#endif
}

UMessenger* UMessenger::Get()
{
	if ( !IsValid(Instance))
	{
		Instance = NewObject<UMessenger>();
		Instance->AddToRoot();
	}
	return Instance;
}

void UMessenger::RemoveAllListeners()
{
	EventMap.Empty();
}

void UMessenger::AddListener(EMSG_ID MsgID, TFunction<void()> Listener)
{
	if (!Listener)
		return;

	TArray<TFunction<void()>>& List = EventMap.FindOrAdd(MsgID);
	List.Add(Listener);
}

void UMessenger::RemoveListener(EMSG_ID MsgID, TFunction<void()> Listener)
{
	if (!EventMap.Contains(MsgID)) return;

	TArray<TFunction<void()>>& List = EventMap[MsgID];
	// C++에서는 정확한 비교가 어렵기 때문에 간단한 구조로 처리
	List.Empty(); 
	if (List.Num() == 0)
	{
		EventMap.Remove(MsgID);
	}
}

void UMessenger::Broadcast(EMSG_ID MsgID, EMSG_MODE Mode)
{
	if (!EventMap.Contains(MsgID))
	{
		if (Mode == EMSG_MODE::REQUIRE_LISTENER)
		{
			UE_LOG(LogTemp, Error, TEXT("Broadcasting message %d but no listener found."), (uint8)MsgID);
		}
		return;
	}

	TArray<TFunction<void()>>& List = EventMap[MsgID];
	for (auto& Callback : List)
	{
		Callback();
	}
}