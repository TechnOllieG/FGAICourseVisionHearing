#include "VisionTargetComponent.h"

#include "AITurretGame/AITurretGameInstance.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"

UVisionTargetComponent::UVisionTargetComponent()
{
	
}

void UVisionTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	// Add this to list of vision targets in game instance
	GameInstance = Cast<UAITurretGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->VisionTargetComponents.Add(this);
	
	UShapeComponent* UShapeComponent = GetOwner()->FindComponentByClass<class UShapeComponent>();
	CollisionExtentsCollider = UShapeComponent->Bounds.BoxExtent;
}

void UVisionTargetComponent::BeginDestroy()
{
	Super::BeginDestroy();
	if(GameInstance != nullptr && GameInstance->VisionTargetComponents.Contains(this))
		GameInstance->VisionTargetComponents.Remove(this);
}

