#pragma once
#include "GameFramework/Pawn.h"
#include "TurretGamePlayer.generated.h"

class UCapsuleComponent;
class UCameraComponent;

UCLASS(Abstract)
class ATurretGamePlayer : public APawn
{
	GENERATED_BODY()

public:
	ATurretGamePlayer();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AccelerationSpeed = 2000.f;

	UPROPERTY(EditDefaultsOnly)
	float Friction = 4.5f;

	UPROPERTY(EditDefaultsOnly)
	float JumpImpulse = 500.f;

	UPROPERTY(BlueprintReadOnly)
	bool IsCrouching = false;

	UPROPERTY(EditDefaultsOnly)
	float CapsuleHalfHeightCrouch;

	UPROPERTY(EditDefaultsOnly)
	float CrouchSpeed;

	UPROPERTY(EditDefaultsOnly)
	float AccelerationSpeedWhileCrouched = 1500.f;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The maximum amount of iterations movement will take to slide against wall"))
	int MovementIterativeDepth = 4;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The trace channel to be used for the linetrace to check if player is grounded"))
	TEnumAsByte<ECollisionChannel> GroundTraceChannel;

	UPROPERTY(EditDefaultsOnly)
	float Gravity = 981.f;

	UFUNCTION(BlueprintCallable)
	bool IsGrounded();

private:
	void HandleMoveForwardInput(float Value);
	void HandleMoveRightInput(float Value);
	void HandleLookUpInput(float Value);
	void HandleLookRightInput(float Value);
	void HandlePressJumpInput();
	void HandleReleaseJumpInput();
	void HandlePressCrouchInput();
	void HandleReleaseCrouchInput();
	
	void HandleCrouch(float DeltaTime);

	FVector Velocity = FVector::ZeroVector;
	float ForwardInput = 0.f;
	bool JumpInput = false;
	float RightInput = 0.f;
	float DefaultCapsuleHalfHeight = 0.f;
	float BaseAccelerationSpeed = 0.f;
};
