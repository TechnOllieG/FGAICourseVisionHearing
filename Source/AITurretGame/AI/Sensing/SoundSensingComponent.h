#pragma once
#include "Components/ActorComponent.h"
#include "AITurretGame/AITurretGameInstance.h"
#include "SoundSensingComponent.generated.h"

class USoundTargetComponent;
class UAITurretGameInstance;

DECLARE_LOG_CATEGORY_EXTERN(LogSoundSensingComponent, Log, All);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API USoundSensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoundSensingComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleSound(const FSoundSensingData& SoundSensingData);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHearSound(const FSoundSensingData& SoundData);

	/** Log each sound that is played with the loudness as this sound sensing component percieves it */
	UPROPERTY(EditAnywhere)
	bool DebugRelativeLoudness;

	/** Display a debug sphere to indicate the max hearing range of this sound sensing component */
	UPROPERTY(EditAnywhere)
	bool DebugHearingRange;

	/** If sound loudness is equal or higher than this value the sound will be "heard" by the AI. Note that the loudness of the sound is based on the distance of the sound from the AI */
	UPROPERTY(EditAnywhere)
	float LoudnessThreshold;

	/** If a sound is outside this range, it will not be heard regardless of how loud it is */
	UPROPERTY(EditAnywhere)
	float MaxRangeRadius;

private:
	UAITurretGameInstance* GameInstance;
};