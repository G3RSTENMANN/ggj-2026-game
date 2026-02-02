#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"

#include "RespawnPoint.generated.h"

UCLASS()
class GGJ_GAME_API ARespawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// standard stuff
	ARespawnPoint();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* capsule_comp;

	// member vars
	FVector loc;

};
