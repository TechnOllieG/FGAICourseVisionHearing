#include "SoundSensingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogSoundSensingComponent);

USoundSensingComponent::USoundSensingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USoundSensingComponent::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UAITurretGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->SoundSensingComponents.Add(this);
}

void USoundSensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(DebugHearingRange)
		UKismetSystemLibrary::DrawDebugSphere(this, GetOwner()->GetActorLocation(), MaxRangeRadius, 12, FLinearColor::Red);
}

void USoundSensingComponent::MakeSound(const FSoundSensingData& SoundSensingData)
{
	float NormalizedDistanceToSound = UKismetMathLibrary::NormalizeToRange((SoundSensingData.TargetActor->GetActorLocation() - GetOwner()->GetActorLocation()).SizeSquared(), 0.f, FMath::Square(MaxRangeRadius));
	
	float RelativeSoundLoudness = (1 - NormalizedDistanceToSound) * SoundSensingData.RelativeSoundLoudness;

	if(DebugRelativeLoudness)
		UE_LOG(LogSoundSensingComponent, Log, TEXT("Sound was picked up by %s with a percieved loudness of %f"), *GetOwner()->GetName(), RelativeSoundLoudness);

	if(RelativeSoundLoudness >= LoudnessThreshold)
	{
		FSoundSensingData Data = FSoundSensingData(GetOwner(), RelativeSoundLoudness);
		OnHearSound(Data);
	}
}
