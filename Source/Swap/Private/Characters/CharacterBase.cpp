#include "Characters/CharacterBase.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Characters/CharacterAttributeSet.h"
#include "Characters/SwapMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/InputActionBindInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Swap.h"

static const FName FootstepTraceTag(TEXT("FootstepTrace"));
static const float GroundOffset = 10.0f;

ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USwapMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	FootstepSFXComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepSFX"));
	FootstepSFXComponent->SetupAttachment(GetRootComponent());
	FootstepSFXComponent->bAutoActivate = false;

	LandSFXComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LandedSFX"));
	LandSFXComponent->SetupAttachment(GetRootComponent());
	LandSFXComponent->bAutoActivate = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributes"));

	AliveTag = FGameplayTag::RequestGameplayTag("Character.State.Alive");
	DeadTag = FGameplayTag::RequestGameplayTag("Character.State.Dead");
	DyingTag = FGameplayTag::RequestGameplayTag("Character.State.Dying");

	AbilitySystemComponent->RegisterGameplayTagEvent(DyingTag).AddUObject(this, &ACharacterBase::HandleKill);
	AbilitySystemComponent->RegisterGameplayTagEvent(DeadTag).AddUObject(this, &ACharacterBase::HandleDead);
}

void ACharacterBase::HandleKill(const FGameplayTag CallbackTag, int NewCount)
{
	if (NewCount > 0 && AbilitySystemComponent->HasMatchingGameplayTag(AliveTag)) // Can kill only if alive
	{
		AbilitySystemComponent->SetTagMapCount(AliveTag, 0);
		OnKilled();
	}
}


void ACharacterBase::HandleDead(const FGameplayTag CallbackTag, int NewCount)
{
	if (NewCount > 0 && AbilitySystemComponent->HasMatchingGameplayTag(DyingTag)) // Can dead only if was dying
	{
		AbilitySystemComponent->SetTagMapCount(DyingTag, 0);
		AbilitySystemComponent->SetTagMapCount(AliveTag, 0);
		OnDead();
	}
}

bool ACharacterBase::IsAlive() const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(AliveTag);
}

bool ACharacterBase::IsDying() const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(DyingTag);
}

void ACharacterBase::OnFootstep()
{
	if (UWorld* World = GetWorld())
	{
		FHitResult GroundHit;
		const FVector Start = GetActorLocation();
		const FVector End = Start + FVector::DownVector * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + GroundOffset);
		const FCollisionQueryParams QueryParams(FootstepTraceTag, false, this);
		if (World->LineTraceSingleByChannel(GroundHit, Start, End, ECC_Visibility, QueryParams))
		{
			PrepareSFXForSurface(FootstepSFXComponent, GroundHit);
			FootstepSFXComponent->Play();
		}
	}
}

void ACharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	PrepareSFXForSurface(LandSFXComponent, Hit);
	LandSFXComponent->Play();
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ACharacterBase::GetMoveSpeed() const
{
	return CharacterAttributes->GetMoveSpeed();
}

float ACharacterBase::GetMaxHealth() const
{
	return CharacterAttributes->GetMaxHealth();
}

float ACharacterBase::GetHealth() const
{
	return CharacterAttributes->GetMaxHealth() - CharacterAttributes->GetTakenDamage();
}

void ACharacterBase::HandleMoveSpeedAttributeChange(const FOnAttributeChangeData& Data)
{
	if (Data.GEModData != nullptr && Data.GEModData->EffectSpec.CapturedSourceTags.GetAggregatedTags() != nullptr)
	{
		OnMoveSpeedChanged(Data.NewValue - Data.OldValue, *Data.GEModData->EffectSpec.CapturedSourceTags.GetAggregatedTags());
	}
	else
	{
		OnMoveSpeedChanged(Data.NewValue - Data.OldValue, FGameplayTagContainer());
	}
}

void ACharacterBase::HandleHealthChanged(float Delta, const FGameplayTagContainer& SourceTags)
{
	OnHealthChanged(Delta, SourceTags);
}

void ACharacterBase::HandleDamage(float Delta, const FGameplayTagContainer& SourceTags)
{
	OnDamageTaken(Delta, SourceTags);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);	
		ApplyDefaultAbilitiesAndEffects();
		
		AbilitySystemComponent->SetTagMapCount(DyingTag, 0);
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
		AbilitySystemComponent->SetTagMapCount(AliveTag, 1);
	}
}

void ACharacterBase::UnPossessed()
{
	Super::UnPossessed();
	if (AbilitySystemComponent != nullptr)
	{
		RemoveDefaultAbilitiesAndEffects();
	}
}

void ACharacterBase::ApplyDefaultAbilitiesAndEffects()
{
	for (const TSubclassOf<UGameplayAbility>& AbilityTemplate : PassiveAbilities)
	{
		UGameplayAbility* Ability = AbilityTemplate.GetDefaultObject();
		if (Ability)
		{
			IInputActionBindInterface* InputActionBindInterface = Cast<IInputActionBindInterface>(Ability);
			int InputID = InputActionBindInterface != nullptr ? InputActionBindInterface->GetActionBind() : INDEX_NONE;
			// We are the source of this ability
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 0, InputID, this));
		}
	}

	for (const TSubclassOf<UGameplayEffect>& Effect : PassiveEffects)
	{
		if (Effect)
		{
			// Set us as the source of this effect
			FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
			Context.AddSourceObject(this);

			FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(Effect, UGameplayEffect::INVALID_LEVEL, Context);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data);
		}
	}
}

void ACharacterBase::RemoveDefaultAbilitiesAndEffects()
{
	// We need to first record what abilities we are removing.
	// Can't do it one go as removing will invalidate the array
	TArray<FGameplayAbilitySpecHandle> PassiveSpecs;

	// Need to run through all abilities we can activate. As they could potentially
	// be a duplicate ability not made by use which we probaly don't want to touch
	const TArray<FGameplayAbilitySpec>& AbilitySpecs = AbilitySystemComponent->GetActivatableAbilities();
	for (const FGameplayAbilitySpec& Spec : AbilitySpecs)
	{
		if (Spec.SourceObject == this && PassiveAbilities.Contains(Spec.Ability->GetClass()))
		{
			PassiveSpecs.Add(Spec.Handle);
		}
	}

	// We can now safely remove all our passive abilities
	for (const FGameplayAbilitySpecHandle& Spec : PassiveSpecs)
	{
		AbilitySystemComponent->ClearAbility(Spec);
	}

	// Remove effects sourced by us (there might be effects made somewhere else)
	FGameplayEffectQuery Query;
	Query.EffectSource = this;

	AbilitySystemComponent->RemoveActiveEffects(Query);
}

void ACharacterBase::ActivateAbilitiesWithTags(const FGameplayTagContainer& InTags)
{
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(InTags);
	}
}

void ACharacterBase::CancelAbilitiesWithTags(const FGameplayTagContainer& InTags)
{
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->CancelAbilities(&InTags);
	}
}

void ACharacterBase::Reset()
{
	Super::Reset();
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
		RemoveDefaultAbilitiesAndEffects();
		ApplyDefaultAbilitiesAndEffects();
		
		AbilitySystemComponent->SetTagMapCount(DyingTag, 0);
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
		AbilitySystemComponent->SetTagMapCount(AliveTag, 1);
	}
}
