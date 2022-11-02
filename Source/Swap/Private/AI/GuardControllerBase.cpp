#include "AI/GuardControllerBase.h"

#include "DrawDebugHelpers.h"
#include "AI/GuardInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Team.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Swap.h"

static const FName CurrentInterestLocationKey = "CurrentInterestLocation";
static const FName CurrentTargetKey = "CurrentTarget";
static const FName AlertLevelKey = "AlertLevel";

static const FName FootstepTag = "Footstep";

static const int PriorityPeripheralVision = 20;
static const int PriorityFootstep = 20;
static const int PriorityReport = 50;
static const int PrioritySaw = 100;
static const int PrioritySee = 9001;

AGuardControllerBase::AGuardControllerBase()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSenseConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingSenseConfig"));
	TeamConfig = CreateDefaultSubobject<UAISenseConfig_Team>(TEXT("TeamSenseConfig"));
	
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->ConfigureSense(*TeamConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AGuardControllerBase::OnTargetPerceptionUpdated);

	CurrentAlertLevel = EGuardAlertLevel::None;
	AlertTime = 1.5f;
	DetectTime = 1.5f;
	SightVerticalLimit = 400.0f;
	ImmediateDetectionRangeNormalized = 0.2f;

	MainViewCone = 15.0f;
	MidRangeViewCone = 45.0f;
	MidRangeDistance = 1500.0f;
	CloseViewCone = 60.0f;
	CloseViewDistance = 1000.0f;
	PeripheralDistance = 500.0f;
	LostTargetDistance = 400.0f;
	ImmediateDetectionRange = 200.0f;
	PeripheralDetectionSpeed = 50.0f;

	NoiseThreshold = 0.1f;
}

ETeamAttitude::Type AGuardControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* MyPawn = Cast<APawn>(&Other);
	if (MyPawn != nullptr)
	{
		const AController* Controller = MyPawn->GetController();
		if (Cast<APlayerController>(Controller) != nullptr)
		{
			return ETeamAttitude::Hostile;
		}
		if (Cast<AAIController>(Controller) != nullptr)
		{
			return ETeamAttitude::Friendly;
		}
	}

	return ETeamAttitude::Neutral;
}


void AGuardControllerBase::PushInterest(const FVector& Location, int Priority, float Range)
{
	// Update current interest if it's same or more important
	const FGuardInterest Interest = FGuardInterest(Location, Priority, Range);
	if (Interest.IsSame(CurrentInterest) || Interest.Priority >= CurrentInterest.Priority)
	{
		CurrentInterest = Interest;
		UpdateCurrentInterest();
	}
}

void AGuardControllerBase::PopInterest()
{
	ClearCurrentInterest();
}

void AGuardControllerBase::UpdateCurrentInterest()
{
	GetBlackboardComponent()->SetValueAsVector(CurrentInterestLocationKey, CurrentInterest.Location);
}

void AGuardControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentTarget == nullptr && SeeingTarget != nullptr)
	{
		UpdateSeeingTarget(DeltaSeconds);	
	}
	else if (CurrentTarget != nullptr)
	{
		UpdateCurrentTarget();	
	}
}

void AGuardControllerBase::UpdateCurrentTarget()
{
	if (SeeingTarget != nullptr)
	{
		LastKnownLocation = SeeingTarget->GetActorLocation();
	}
	if (CurrentTarget != nullptr)
	{
		const float Distance = FVector::Distance(CurrentTarget->GetActorLocation(), LastKnownLocation);
		if (Distance > LostTargetDistance)
		{
			UE_LOG(LogSwap, Verbose, TEXT("%s: lost %s"), *GetPawn()->GetName(), *CurrentTarget->GetName());
			PushInterest(CurrentTarget->GetActorLocation(), PrioritySaw, 300.0f);
			SetAlertLevel(EGuardAlertLevel::Investigate);
			Blackboard->ClearValue(CurrentTargetKey);
			PerceptionComponent->ForgetActor(CurrentTarget);
			CurrentTarget = nullptr;
		}
	}
}

void AGuardControllerBase::SetAlertLevel(EGuardAlertLevel AlertLevel)
{
	if (CurrentAlertLevel != AlertLevel)
	{
		UE_LOG(LogSwap, Verbose, TEXT("%s: switch alert level from %d to %d"), *GetPawn()->GetName(), CurrentAlertLevel, AlertLevel);
		CurrentAlertLevel = AlertLevel;
		if (IGuardInterface* GuardInterface = Cast<IGuardInterface>(GetPawn()))
		{
			GuardInterface->AlertLevelChanged(AlertLevel);
		}
		GetBlackboardComponent()->SetValueAsEnum(AlertLevelKey, static_cast<uint8>(AlertLevel));	
	}	
}

EGuardAlertLevel AGuardControllerBase::GetAlertLevel() const
{
	return CurrentAlertLevel;
}

void AGuardControllerBase::OnTargetPerceptionUpdated(AActor* Other, FAIStimulus Stimulus)
{
	if (Stimulus.Type == SightConfig->GetSenseID())
	{
		UpdateSight(Other, Stimulus);
	}
	else if (Stimulus.Type == HearingConfig->GetSenseID())
	{
		UpdateHearing(Other, Stimulus);
	}
	else if (Stimulus.Type == TeamConfig->GetSenseID())
	{
		//UpdateTeam(Other, Stimulus);
	}
}

bool AGuardControllerBase::UpdateSeeingTarget(AActor* Target, float ConeAngle, float MaxDistance, float InAlertTime, float InDetectTime)
{
	check(Target);
	if (IsInViewCone(Target, ConeAngle, MaxDistance))
	{
		const float Distance = FVector::Distance(GetPawn()->GetActorLocation(), Target->GetActorLocation());
		const float ImmediateDetection = MaxDistance * ImmediateDetectionRangeNormalized;
		if (Distance <= ImmediateDetection && CurrentAlertLevel < EGuardAlertLevel::Attack)
		{
			AttackTarget(Target);			
			return true;
		}
		if (TimeSeeTarget >= InAlertTime && CurrentAlertLevel < EGuardAlertLevel::Alert)
		{
			UE_LOG(LogSwap, Verbose, TEXT("%s detects %s"), *GetPawn()->GetName(), *Target->GetName());
			PushInterest(Target->GetActorLocation(), PrioritySaw, 200.0f);
			SetAlertLevel(EGuardAlertLevel::Alert);
			TimeSeeTarget = 0.0f;
			return true;
		}
		if (TimeSeeTarget >= InDetectTime && CurrentAlertLevel < EGuardAlertLevel::Attack)
		{
			AttackTarget(Target);
			return true;
		}	
	}

	return false;
}

void AGuardControllerBase::UpdateSeeingTarget(float DeltaTime)
{
	TimeSeeTarget += DeltaTime;
	const float Distance = FVector::Distance(GetPawn()->GetActorLocation(), SeeingTarget->GetActorLocation());
	const bool IsVisibleVertically = FMath::Abs(SeeingTarget->GetActorLocation().Z - GetPawn()->GetActorLocation().Z) <= SightVerticalLimit;
	
	if (IsVisibleVertically && Distance <= ImmediateDetectionRange && CurrentAlertLevel < EGuardAlertLevel::Attack)
	{
		AttackTarget(SeeingTarget);
		return;
	}
	if (IsVisibleVertically)
	{
		if (!UpdateSeeingTarget(SeeingTarget, CloseViewCone, CloseViewDistance, AlertTime, DetectTime))
		{
			if (!UpdateSeeingTarget(SeeingTarget, MidRangeViewCone, MidRangeDistance, AlertTime, DetectTime))
			{
				if (!UpdateSeeingTarget(SeeingTarget, MainViewCone, SightConfig->SightRadius, 0, AlertTime))
				{
					if (Distance <= PeripheralDistance && SeeingTarget->GetVelocity().Size() >= PeripheralDetectionSpeed)
					{
						if (CurrentAlertLevel < EGuardAlertLevel::Alert)
						{
							SetAlertLevel(EGuardAlertLevel::Alert);
						}
						PushInterest(SeeingTarget->GetActorLocation(), PriorityPeripheralVision, 400.0f);
					}
				}	
			}
		}	
	}
}

void AGuardControllerBase::UpdateHearing(AActor* Causer, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && CurrentTarget == nullptr && Stimulus.Strength >= NoiseThreshold)
	{
		PushInterest(Stimulus.StimulusLocation, PriorityFootstep, 400.0f);
		if (CurrentAlertLevel >= EGuardAlertLevel::Alert)
		{
			SetAlertLevel(EGuardAlertLevel::Investigate);
		}
		else if (CurrentAlertLevel >= EGuardAlertLevel::Normal)
		{
			SetAlertLevel(EGuardAlertLevel::Alert);
		}
	}
}

void AGuardControllerBase::UpdateSight(AActor* Causer, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// Drop it if too high or low in relation to us.
		// But ONLY if it isn't too close to us.
		if (FMath::Abs(Causer->GetActorLocation().Z - GetPawn()->GetActorLocation().Z) > SightVerticalLimit)
		{
			PerceptionComponent->ForgetActor(Causer);
			return;
		}
		SeeingTarget = Causer;
		TimeSeeTarget = 0.0f;
		UE_LOG(LogSwap, Verbose, TEXT("%s: see %s"), *GetPawn()->GetName(), *Causer->GetName());
	}
	else
	{
		UE_LOG(LogSwap, Verbose, TEXT("%s: lost sight at %s"), *GetPawn()->GetName(), *Causer->GetName());
		LastKnownLocation = Causer->GetActorLocation();
		SeeingTarget = nullptr;
	}
}

bool AGuardControllerBase::IsInViewCone(const AActor* Actor, float ConeAngle, float Distance) const
{
	if (Actor == nullptr)
	{
		return false;
	}
	const float Dot = FMath::Cos(FMath::DegreesToRadians(ConeAngle / 2.0f));
	const float TargetDistance = FVector::Distance(GetPawn()->GetActorLocation(), Actor->GetActorLocation());
	const FVector ToTarget = (Actor->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal2D();
	return FVector::DotProduct(GetPawn()->GetActorForwardVector(), ToTarget) >= Dot && TargetDistance <= Distance;
}

void AGuardControllerBase::Reset()
{
	Super::Reset();
	PerceptionComponent->ForgetAll();
	if (BT != nullptr)
	{
		RunBehaviorTree(BT);
		ClearCurrentInterest();
		Blackboard->ClearValue(CurrentInterestLocationKey);
		Blackboard->ClearValue(CurrentTargetKey);
		Blackboard->ClearValue(AlertLevelKey);
		CurrentAlertLevel = EGuardAlertLevel::None;
		SetAlertLevel(EGuardAlertLevel::Normal);	
	}
}

void AGuardControllerBase::ClearCurrentInterest()
{
	GetBlackboardComponent()->ClearValue(CurrentInterestLocationKey);
	GetBlackboardComponent()->ClearValue(CurrentTargetKey);
}

void AGuardControllerBase::AttackTarget(AActor* Target)
{
	UE_LOG(LogSwap, Verbose, TEXT("%s: attack %s"), *GetPawn()->GetName(), *Target->GetName());
	CurrentTarget = Target;
	Blackboard->SetValueAsObject(CurrentTargetKey, Target);
	SetAlertLevel(EGuardAlertLevel::Attack);
}

void AGuardControllerBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentAlertLevel = EGuardAlertLevel::None;
	if (BT != nullptr)
	{
		RunBehaviorTree(BT);		
		SetAlertLevel(EGuardAlertLevel::Normal);	
	}	
}
