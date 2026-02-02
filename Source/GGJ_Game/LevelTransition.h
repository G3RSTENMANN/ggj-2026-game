#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "MyCamera.h"

#include "LevelTransition.generated.h"

UCLASS()
class GGJ_GAME_API ALevelTransition : public AActor
{
	GENERATED_BODY()
	
public:	
	// standard stuff
	ALevelTransition();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* capsule_comp;

	// member vars
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float move_time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector camera_direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AMyCamera* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int transition_to_room;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool is_vertical = false;

	// functions
	void MoveCamera();
};
