// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SWAP_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPlayerAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category=Player)
	FGameplayAttributeData NoiseLevel;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, NoiseLevel)
};
