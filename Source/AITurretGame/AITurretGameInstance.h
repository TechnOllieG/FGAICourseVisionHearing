#pragma once
#include "Engine/GameInstance.h"
#include "AITurretGameInstance.generated.h"

class UVisionTargetComponent;
class USoundTargetComponent;
class USoundSensingComponent;

USTRUCT(BlueprintType)
struct FSoundSensingData
{
	GENERATED_BODY()

	FSoundSensingData() {}
	FSoundSensingData(AActor* Actor, float Loudness)
	{
		TargetActor = Actor;
		RelativeSoundLoudness = Loudness;
	}

	UPROPERTY(BlueprintReadOnly)
	AActor* TargetActor;

	UPROPERTY(BlueprintReadOnly)
	float RelativeSoundLoudness;
};

UCLASS()
class AITURRETGAME_API UAITurretGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	TSet<UVisionTargetComponent*> VisionTargetComponents;
	TSet<USoundSensingComponent*> SoundSensingComponents;
};