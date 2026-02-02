#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"

#include "MyCamera.generated.h"

UCLASS()
class GGJ_GAME_API AMyCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	// standard stuff
	AMyCamera();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// config and controlling
	float move_time;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool moving;

	FVector target_location;

	FVector move_offset;

	void Move(FVector offset, float move_speed);

	void MoveTo(FVector location);
};
