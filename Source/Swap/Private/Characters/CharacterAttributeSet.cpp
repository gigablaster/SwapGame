// Copyright (c) 2020-2021 Vladimir Kuskov
#include "Characters/CharacterAttributeSet.h"
#include "Characters/CharacterBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UCharacterAttributeSet::UCharacterAttributeSet()
	: MoveSpeed(300.0f)	
	, TakenDamage(0.0f)
	, MaxHealth(100.0f)
{}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("Ping!"));
	Super::PostGameplayEffectExecute(Data);

	AActor* TargetActor = nullptr;
	ACharacterBase* TargetCharacter = nullptr;
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ACharacterBase>(TargetActor);
	}

	float DeltaValue = 0.0f;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	if (Data.EvaluatedData.Attribute == GetTakenDamageAttribute())
	{
		SetTakenDamage(FMath::Clamp(GetTakenDamage(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float MaxDamage = GetMaxHealth() - GetTakenDamage();
		const float RealDamage = FMath::Min(MaxDamage, GetDamage());
		SetDamage(0);
		SetTakenDamage(GetTakenDamage() + RealDamage);
		if (TargetCharacter != nullptr)
		{
			TargetCharacter->HandleHealthChanged(-RealDamage, SourceTags);
			TargetCharacter->HandleDamage(RealDamage, SourceTags);
		}
	}
}
