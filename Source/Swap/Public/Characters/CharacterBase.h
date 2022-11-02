#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FootstepInterface.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

#include "CharacterBase.generated.h"

class UAudioComponent;
class UAbilitySystemComponent;
class UGameplayEffect;
class UGameplayAbility;
struct FOnAttributeChangeData;

UCLASS(Abstract)
class SWAP_API ACharacterBase : public ACharacter, public IFootstepInterface, public IAbilitySystemInterface
{
	friend class UCharacterAttributeSet;
	
	GENERATED_BODY()
public:
	ACharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// IFootstepInterface
	virtual void OnFootstep() override;
	
	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// APawn
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	// ACharacter
	virtual void Landed(const FHitResult& Hit) override;

	// AActor
	virtual void Reset() override;

	UFUNCTION(BlueprintCallable, Category=Abilities)
	void ActivateAbilitiesWithTags(const FGameplayTagContainer& InTags);

	UFUNCTION(BlueprintCallable, Category=Abilities)
    void CancelAbilitiesWithTags(const FGameplayTagContainer& InTags);


	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed()const;

	UFUNCTION(BlueprintCallable)
	float GetHealth()const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth()const;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsDying() const;
protected:
	virtual void BeginPlay() override;
	
	void HandleMoveSpeedAttributeChange(const FOnAttributeChangeData& Data);
	void HandleHealthChanged(float Delta, const FGameplayTagContainer& SourceTags);
	void HandleDamage(float Delta, const FGameplayTagContainer& SourceTags);
	void HandleDead(const FGameplayTag CallbackTag, int NewCount);
	void HandleKill(const FGameplayTag CallbackTag, int NewCount);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float Delta, const FGameplayTagContainer& SourceTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDead();

	UFUNCTION(BlueprintImplementableEvent)
	void OnKilled();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float Delta, const FGameplayTagContainer& SourceTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTaken(float Delta, const FGameplayTagContainer& SourceTags);
	
	UFUNCTION(BlueprintImplementableEvent)
    void PrepareSFXForSurface(UAudioComponent* AudioComponent, const FHitResult& HitResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UAudioComponent* FootstepSFXComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UAudioComponent* LandSFXComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Abilities)
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilities;

	UPROPERTY()
	UCharacterAttributeSet* CharacterAttributes;

	void ApplyDefaultAbilitiesAndEffects();
	void RemoveDefaultAbilitiesAndEffects();

	FGameplayTag AliveTag;
	FGameplayTag DeadTag;
	FGameplayTag DyingTag;
};
