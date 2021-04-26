#include "SoundTargetComponent.h"
#include "AITurretGame/AITurretGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SoundSensingComponent.h"

USoundTargetComponent::USoundTargetComponent()
{
}

void USoundTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UAITurretGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void USoundTargetComponent::PlaySound(float Loudness)
{
	const FSoundSensingData Data = FSoundSensingData(GetOwner(), Loudness);
	TSet<USoundSensingComponent*> Components = GameInstance->SoundSensingComponents;
	
	for(USoundSensingComponent* Component : Components)
	{
		Component->MakeSound(Data);
	}
}
