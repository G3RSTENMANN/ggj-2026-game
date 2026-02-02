#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

// camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// movement
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "MyCamera.h"
#include "PaperZDAnimInstance.h"
#include "Engine/TimerHandle.h"

#include "Sound/SoundBase.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class GGJ_GAME_API APlayerCharacter : public APaperZDCharacter
{
	GENERATED_BODY()
	
public:

	// standard stuff
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float delta_time) override;

	// member variables
	// ----------------
	// character movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* input_mapping_ctx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* move_action;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* jump_action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* change_mask_action;

	// player state
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool can_move = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector respawn_location;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int current_mask_state = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int score = 0;

	// room state
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int current_room = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int room_on_respawn_location;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* bling_sound;


	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AMyCamera* camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector camera_on_respawn_location;

	// Death Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperZDAnimSequence* death_seq_red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperZDAnimSequence* death_seq_blue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTimerHandle death_timer;

	FZDOnAnimationOverrideEndSignature on_override_anim_end_delegate;
	FVector death_location;
	bool is_dead;

	void OnDeathTimerTimeout();
	void OnOverrideAnimEnd(bool completed);
	void ResetPlayer();

	// methods
	// -------
	// character movement
	virtual void SetupPlayerInputComponent(UInputComponent* player_input_component) override;

	void Move(const FInputActionValue& value);
	void JumpStarted(const FInputActionValue& value);
	void JumpEnded(const FInputActionValue& value);
	void ChangeMask(const FInputActionValue& value);

	void UpdateDirection(float move_dir);

	void IncreaseScore();

	// handlers
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	void Die();
	
};
