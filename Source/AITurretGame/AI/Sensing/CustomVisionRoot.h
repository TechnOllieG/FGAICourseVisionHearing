#pragma once
#include "Components/SceneComponent.h"
#include "CustomVisionRoot.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API UCustomVisionRoot : public USceneComponent
{
	GENERATED_BODY()

public:
	UCustomVisionRoot();
};
