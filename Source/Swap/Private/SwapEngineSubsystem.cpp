// Copyright (c) 2020-2021 Vladimir Kuskov
#include "SwapEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void USwapEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
