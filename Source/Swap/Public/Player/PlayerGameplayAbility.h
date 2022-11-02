// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/InputActionBindInterface.h"
#include "SwapTypes.h"
#include "PlayerGameplayAbility.generated.h"

UCLASS(Abstract)
class SWAP_API UPlayerGameplayAbility : public UGameplayAbility, public IInputActionBindInterface
{
	GENERATED_BODY()
public:
	UPlayerGameplayAbility();

	virtual int GetActionBind()const override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlayerAction PlayerAction;
};
