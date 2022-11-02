// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SWAP_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCharacterAttributeSet();

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, Category=Health)
	FGameplayAttributeData TakenDamage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, TakenDamage);

	UPROPERTY(BlueprintReadOnly, Category=Health)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category=Meta)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Damage);
};
