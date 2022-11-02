// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "SwapEngineSubsystem.generated.h"

UCLASS()
class SWAP_API USwapEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
