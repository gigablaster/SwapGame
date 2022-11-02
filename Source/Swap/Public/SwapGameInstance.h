// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SwapGameInstance.generated.h"

UCLASS(Abstract)
class SWAP_API USwapGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	USwapGameInstance();
};