#include "VisionSensingComponent.h"
#include "AITurretGame/AITurretGameInstance.h"
#include "VisionTargetComponent.h"
#include "AITurretGame/TurretGamePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "CustomVisionRoot.h"

UVisionSensingComponent::UVisionSensingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVisionSensingComponent::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UAITurretGameInstance>(UGameplayStatics::GetGameInstance(this));
	OptionalSceneComponentOverride = GetOwner()->FindComponentByClass<UCustomVisionRoot>();
}

void UVisionSensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(OnTickVisionTarget.IsBound())
	{
		for(UVisionTargetComponent* TargetComponent : SensedComponents)
		{
			FVisionSensingData Data = FVisionSensingData(TargetComponent->GetOwner(), TargetComponent);
			OnTickVisionTarget.Broadcast(Data);
		}
	}
	
	AActor* Owner = GetOwner();
	FVector Position;
	FVector Forward;

	if(OptionalSceneComponentOverride == nullptr)
	{
		Position = Owner->GetActorLocation();
		Forward = Owner->GetActorForwardVector();
	}
	else
	{
		Position = OptionalSceneComponentOverride->GetComponentLocation();
		Forward = OptionalSceneComponentOverride->GetForwardVector();
	}

	if (bDebugVisionAngle)
	{
		FVector Right = Forward.RotateAngleAxis(VisionAngle, FVector::UpVector);
		FVector Left = Forward.RotateAngleAxis(-VisionAngle, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Position, Position + Right * DistanceMinimum, FLinearColor::Red);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Position, Position + Left * DistanceMinimum, FLinearColor::Green);
	}
	
	for(UVisionTargetComponent* TargetComp : GameInstance->VisionTargetComponents)
	{
		const FVector TargetLocation = TargetComp->GetOwner()->GetActorLocation();
		const bool IsVisible = IsPointVisible(TargetLocation, Position, Forward);

		const FVector DirectionToTarget = (TargetLocation - Position).GetSafeNormal();

		// These two vectors are the left and right directions of the target actor from the perspective of this actor. Used to check if part of the actor is visible around an obstacle
		const FVector LeftDirectionOfTarget = FVector::CrossProduct(DirectionToTarget, FVector::UpVector).GetSafeNormal();
		const FVector RightDirectionOfTarget = -LeftDirectionOfTarget;

		const FVector Extents = TargetComp->CollisionExtentsCollider;
		const float RelevantExtents = Extents.X > Extents.Y ? Extents.X : Extents.Y;
		const float HeightExtents = Extents.Z;
		
		const FVector LeftPos = TargetLocation + LeftDirectionOfTarget * (RelevantExtents - KINDA_SMALL_NUMBER);
		const FVector RightPos = TargetLocation + RightDirectionOfTarget * (RelevantExtents - KINDA_SMALL_NUMBER);
		const FVector TopPos = TargetLocation + FVector::UpVector * (HeightExtents - KINDA_SMALL_NUMBER);
		
		const bool IsObstructed = IsTargetObstructed(TargetLocation, Position) &&
			IsTargetObstructed(LeftPos, Position) &&
			IsTargetObstructed(RightPos, Position) &&
			IsTargetObstructed(TopPos, Position);
		
		if(!SensedComponents.Contains(TargetComp))
		{
			if(IsVisible && !IsObstructed)
			{
				SensedComponents.Add(TargetComp);
				
				if(!OnFindVisionTarget.IsBound())
					return;
				
				FVisionSensingData Data = FVisionSensingData(TargetComp->GetOwner(), TargetComp);
				OnFindVisionTarget.Broadcast(Data);
			}
		}
		else if(!IsVisible || IsObstructed)
		{
			SensedComponents.Remove(TargetComp);

			if(!OnLooseVisionTarget.IsBound())
				return;
			
			FVisionSensingData Data = FVisionSensingData(TargetComp->GetOwner(), TargetComp);
			OnLooseVisionTarget.Broadcast(Data);
		}
	}
}

bool UVisionSensingComponent::IsPointVisible(const FVector& PointToTest, const FVector& Origin, const FVector& Forward) const
{
	const float DistanceMinimumSq = FMath::Square(DistanceMinimum);

	if (FVector::DistSquared(Origin, PointToTest) > DistanceMinimumSq)
		return false;

	const FVector DirectionToTarget = (PointToTest - Origin).GetSafeNormal();

	const float DotEdgesOfVisionCone = FMath::Cos(FMath::DegreesToRadians(VisionAngle));
	const float Dot = FVector::DotProduct(Forward, DirectionToTarget);
	
	return Dot > DotEdgesOfVisionCone;
}

bool UVisionSensingComponent::IsTargetObstructed(const FVector& VisionTarget, const FVector& Origin) const
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Origin, VisionTarget, ObstructionTraceChannel.GetValue());
	
	if(Cast<ATurretGamePlayer>(Hit.GetActor()) != nullptr)
		return false;
	
	return true;
}

bool UVisionSensingComponent::IsTargetSeen(UVisionTargetComponent* TargetComponent)
{
	if(SensedComponents.Contains(TargetComponent))
		return true;

	return false;
}

