// Copyright (c) 2025 Doppleddiggong. All rights reserved.

#include "UCoffeeCommonUtil.h"

bool UCoffeeCommonUtil::InBounds(const int32 Index, const int32 Count)
{
	return (Index >= 0) && (Index < Count);
}