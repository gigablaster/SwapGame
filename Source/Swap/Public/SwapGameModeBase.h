#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "GameFramework/GameModeBase.h"
#include "SwapGameModeBase.generated.h"

UCLASS(Abstract)
class SWAP_API ASwapGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASwapGameModeBase();
};
