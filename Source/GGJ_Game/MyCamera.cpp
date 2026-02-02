#include "MyCamera.h"

AMyCamera::AMyCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyCamera::BeginPlay()
{
	Super::BeginPlay();

	target_location = GetActorLocation();
}

void AMyCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// move camera
	if (moving) {
		FVector curr_loc = GetActorLocation();
		bool reached_target = curr_loc.Equals(target_location);

		// Move
		if (!reached_target) {
			FVector new_loc = FMath::VInterpConstantTo(curr_loc, target_location, DeltaTime, move_offset.Length() / move_time);
			SetActorLocation(new_loc);
		}
		else {
			moving = false;
		}
	}
}

void AMyCamera::Move(FVector offset, float move_speed)
{
	FVector curr_loc = GetActorLocation();

	// -------------------------------------------------------------------
	// REVIEW THIS SHIT WTF !!!!!!!!
	//target_location = target_location + offset;
	target_location = curr_loc + offset;
	// WEIRD CAMERA BUG WHILE SWIZZLING BETWEEN TWO ROOM TRANSITIONS !!!!
	// -------------------------------------------------------------------

	move_time = move_speed;

	if (!moving) {
		moving = true;
		//GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::White, TEXT("Test"));
		move_offset = offset;
	}
	else {
		move_offset = move_offset + offset;
	}
}

void AMyCamera::MoveTo(FVector location)
{
	moving = false;
	target_location = location;
	SetActorLocation(location);
}

