#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "SwapTypes.h"
#include "GuardControllerInterface.h"
#include "GuardControllerBase.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig;
class UAISenseConfig_Sight;

USTRUCT(BlueprintType)
struct SWAP_API FGuardInterest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	int Priority;

	UPROPERTY(BlueprintReadWrite)
	float Range;

	bool IsSame(const FGuardInterest& Other)const
	{
		return Other.Priority == Priority && FVector::Distance(Location, Other.Location) <= Range;
	}

	FGuardInterest()
        : Location(FVector::ZeroVector), Priority(0), Range(300.0f)
	{ }

	FGuardInterest(const FVector& Location, int Priority, float Range = 300.0f)
        : Location(Location), Priority(Priority), Range(Range)
	{ }
};

UCLASS(Abstract)
class SWAP_API AGuardControllerBase : public AAIController, public IGuardControllerInterface
{
	GENERATED_BODY()
public:
	AGuardControllerBase();

	// AActor
	virtual void Tick(float DeltaSeconds) override;
	virtual void Reset() override;

	// IGenericTeamAgentInterface
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// IGuardControllerInterface
	virtual void SetAlertLevel(EGuardAlertLevel AlertLevel) override;
	virtual EGuardAlertLevel GetAlertLevel() const override;
	virtual void PushInterest(const FVector& Location, int Priority, float Range) override;
	virtual void PopInterest() override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	UBehaviorTree* BT;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=AI)
	float AlertTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=AI)
	float DetectTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=AI)
	float LostTargetDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float ImmediateDetectionRangeNormalized;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float ImmediateDetectionRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float SightVerticalLimit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float MainViewCone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float MidRangeViewCone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float MidRangeDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float CloseViewCone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float CloseViewDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float PeripheralDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sight)
	float PeripheralDetectionSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Hearing)
	float NoiseThreshold;
private:
	UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Other, FAIStimulus Stimulus);

	void UpdateCurrentInterest();
	void ClearCurrentInterest();
	void UpdateCurrentTarget();
	void AttackTarget(AActor* Target);
	void UpdateSeeingTarget(float DeltaTime);
	bool UpdateSeeingTarget(AActor* Target, float ConeAngle, float MaxDistance, float AlertTime, float DetectTime);
	bool IsInViewCone(const AActor* Actor, float ConeAngle, float Distance)const;
	void UpdateSight(AActor* Causer, FAIStimulus Stimulus);
	void UpdateHearing(AActor* Causer, FAIStimulus Stimulus);	
	//void UpdateTeam(AActor* Causer, FAIStimulus Stimulus);
	
	EGuardAlertLevel CurrentAlertLevel;
	FGuardInterest CurrentInterest;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
	UAISenseConfig* HearingConfig;

	UPROPERTY()
	UAISenseConfig* TeamConfig;

	UPROPERTY()
	AActor* SeeingTarget;

	UPROPERTY()
	AActor* CurrentTarget;

	float TimeSeeTarget;
	FVector LastKnownLocation;
};
