#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

#include "Collectible.generated.h"

UCLASS()
class GGJ_GAME_API ACollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// standard stuff
	ACollectible();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* capsule_comp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* collectible_flipbook;
};
