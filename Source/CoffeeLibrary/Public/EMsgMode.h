// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMSG_ID : uint8
{
	E_SET_STATE_DOF,
	E_FOCUS_CARDHAND,
	E_UPDATE_CARD_POSITIONS
};

UENUM(BlueprintType)
enum class EMSG_MODE : uint8
{
	DONT_REQUIRE_LISTENER,
	REQUIRE_LISTENER
};

UENUM(BlueprintType)
enum class EMSG_LISTENER_TYPE : uint8
{
	ADD_LISTENER,
	REMOVE_LISTENER
};