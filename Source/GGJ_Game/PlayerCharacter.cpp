#include "PlayerCharacter.h"

#include "LevelStaticTiles.h"
#include "LevelTransition.h"
#include "RespawnPoint.h"
#include "DeathZone.h"
#include "Collectible.h"

#include "Kismet/GameplayStatics.h"

// GAME LIFECYCLE
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// component setup
	//spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("spring_arm"));
	//spring_arm->SetupAttachment(RootComponent);

	//camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	//camera->SetupAttachment(spring_arm, USpringArmComponent::SocketName);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// add input mapping context
	if (APlayerController* player_controller = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(player_controller->GetLocalPlayer())) {
			subsystem->AddMappingContext(input_mapping_ctx, 0);
		}
	}

	// bind functions
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OverlapBegin);

	// get camera
	AActor* camera_actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMyCamera::StaticClass());
	camera = Cast<AMyCamera>(camera_actor);

	// handle locations
	FVector curr_loc = GetActorLocation();
	FVector new_loc = FVector(curr_loc.X, 0.0f, curr_loc.Z);
	SetActorLocation(new_loc);
	respawn_location = new_loc;
	camera_on_respawn_location = camera->GetActorLocation();
	room_on_respawn_location = 1;

	// setup death
	on_override_anim_end_delegate.BindUObject(this, &APlayerCharacter::OnOverrideAnimEnd);

}

void APlayerCharacter::Tick(float delta_time)
{
	Super::Tick(delta_time);

	if (is_dead) {
		SetActorLocation(death_location);
	}
}


// CHARACTER MOVEMENT
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* player_input_component)
{
	Super::SetupPlayerInputComponent(player_input_component);

	if (UEnhancedInputComponent* enhanced_input_comp = Cast<UEnhancedInputComponent>(player_input_component)) {
		// Set up input action bindings
		enhanced_input_comp->BindAction(move_action, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		enhanced_input_comp->BindAction(jump_action, ETriggerEvent::Started, this, &APlayerCharacter::JumpStarted);
		enhanced_input_comp->BindAction(jump_action, ETriggerEvent::Completed, this, &APlayerCharacter::JumpEnded);
		enhanced_input_comp->BindAction(jump_action, ETriggerEvent::Canceled, this, &APlayerCharacter::JumpEnded);

		enhanced_input_comp->BindAction(change_mask_action, ETriggerEvent::Started, this, &APlayerCharacter::ChangeMask);
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::White, TEXT("Baller"));
	float move_action_val = value.Get<float>(); // = 1 if 'D' is pressed, = -1 if 'A' is pressed

	if (can_move) {
		FVector dir = FVector(1.0f, 0.0f, 0.0f);
		AddMovementInput(dir, move_action_val); // quasi: dir * move_action_val
		UpdateDirection(move_action_val);
	}
}

void APlayerCharacter::JumpStarted(const FInputActionValue& value)
{
	if (can_move) {
		Jump();
	}
}

void APlayerCharacter::JumpEnded(const FInputActionValue& value)
{
	StopJumping();
}

void APlayerCharacter::ChangeMask(const FInputActionValue& value)
{
	if (is_dead) return;

	// Change mask
	AActor* level_tiles_actor = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelStaticTiles::StaticClass());
	ALevelStaticTiles* level_tiles = Cast<ALevelStaticTiles>(level_tiles_actor);
	if (level_tiles && level_tiles->mask_state == 1) {
		current_mask_state = 2;
		level_tiles->SetMask(2);
	}
	else if (level_tiles && level_tiles->mask_state == 2) {
		current_mask_state = 1;
		level_tiles->SetMask(1);
	}

	// Raycast for suffocation
	FCollisionShape interaction_sphere = FCollisionShape::MakeSphere(20.0f);
	FHitResult hit_result;
	FVector start = GetActorLocation() + FVector(0.0f, 1.0f, 0.0f);
	FVector end = GetActorLocation() + FVector(0.0f, -1.0f, 0.0f);
	bool has_hit = GetWorld()->SweepSingleByChannel(
		hit_result, 
		start,
		end, 
		FQuat::Identity, 
		ECC_GameTraceChannel1, 
		interaction_sphere
	);
	if (has_hit) {
		AActor* hit_actor = hit_result.GetActor();
		if (hit_actor->ActorHasTag("DeathZone")) {
			ADeathZone* death_zone = Cast<ADeathZone>(hit_actor);
			if (death_zone && death_zone->GetDeathBySuffocation(current_mask_state)) {
				//GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::White, TEXT("Oh God please work"));
				Die();
			}
		}
	}
}

void APlayerCharacter::UpdateDirection(float move_dir)
{
	// Rotate the player actor | Rotator = Pitch - Yaw - Roll
	FRotator curr_rot = Controller->GetControlRotation();
	if (move_dir < 0.0f) {
		// facing left
		if (curr_rot.Yaw != 180.0f) {
			Controller->SetControlRotation(FRotator(curr_rot.Pitch, 180.0f, curr_rot.Roll));
		}
	}
	else if (move_dir > 0.0f) {
		// facing right
		if (curr_rot.Yaw != 0.0f) {
			Controller->SetControlRotation(FRotator(curr_rot.Pitch, 0.0f, curr_rot.Roll));
		}
	}
}

void APlayerCharacter::IncreaseScore()
{
	score++;
}

void APlayerCharacter::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("LevelTransition")) {
		ALevelTransition* level_transition = Cast<ALevelTransition>(OtherActor);
		if (level_transition && level_transition->transition_to_room != current_room) {
			current_room = level_transition->transition_to_room;
			level_transition->MoveCamera();
			//GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::White, TEXT("Test"));
		}
	}
	if (OtherActor && OtherActor->ActorHasTag("RespawnPoint")) {
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::White, TEXT("Baller"));
		ARespawnPoint* respawn_point = Cast<ARespawnPoint>(OtherActor);
		if (respawn_point) {
			respawn_location = respawn_point->loc;
			camera_on_respawn_location = camera->target_location;
			room_on_respawn_location = current_room;
		}
	}
	if (OtherActor && OtherActor->ActorHasTag("DeathZone")) {
		ADeathZone* death_zone = Cast<ADeathZone>(OtherActor);
		//GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::White, TEXT("Baller"));
		if (death_zone && (!death_zone->is_inside_of_block || death_zone->GetDeathBySuffocation(current_mask_state))) {
			Die();
		}
	}
	if (OtherActor && OtherActor->ActorHasTag("Collectible")) {
		ACollectible* collectible = Cast<ACollectible>(OtherActor);
		if (collectible) {
			UGameplayStatics::PlaySound2D(GetWorld(), bling_sound);
			collectible->Destroy();
			IncreaseScore();
		}
	}
}

void APlayerCharacter::Die()
{
	// freeze player
	can_move = false;
	death_location = GetActorLocation();
	is_dead = true;
	//CustomTimeDilation = 0.01f;

	// death animation
	switch (current_mask_state) {
	case 1:
		GetAnimInstance()->PlayAnimationOverride(death_seq_red, FName("DefaultSlot"), 1.0f, 0.0f, on_override_anim_end_delegate);
		break;
	case 2:
		GetAnimInstance()->PlayAnimationOverride(death_seq_blue, FName("DefaultSlot"), 1.0f, 0.0f, on_override_anim_end_delegate);
		break;
	default:
		break;
	}

	// start timer
	GetWorldTimerManager().SetTimer(death_timer, this, &APlayerCharacter::OnDeathTimerTimeout, 1.0f, false, 1.8f);
}

void APlayerCharacter::OnDeathTimerTimeout()
{
	death_location = respawn_location;
}

void APlayerCharacter::OnOverrideAnimEnd(bool completed)
{
	ResetPlayer();
}

void APlayerCharacter::ResetPlayer()
{
	// reset player & camera
	is_dead = false;
	can_move = true;
	current_room = room_on_respawn_location;
	camera->MoveTo(camera_on_respawn_location); // ITS IMPORTANT THAT CAMERA RESETS BEFORE PLAYER
	SetActorLocation(respawn_location);
	//CustomTimeDilation = 1.0f;
}
