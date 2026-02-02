#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"

#include "DeathZone.generated.h"

UCLASS()
class GGJ_GAME_API ADeathZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// standard stuff
	ADeathZone();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* capsule_comp;

	// state vars
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool is_inside_of_block = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int death_during_mask_state = 0;

	// member functions
	bool GetDeathBySuffocation(int mask_state) const; // lol
};

