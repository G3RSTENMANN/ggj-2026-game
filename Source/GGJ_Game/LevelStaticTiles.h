#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "LevelStaticTiles.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeMaskDelegate, int, mask);

UCLASS()
class GGJ_GAME_API ALevelStaticTiles : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelStaticTiles();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// delegate
	UPROPERTY(BlueprintAssignable)
	FChangeMaskDelegate change_mask_delegate;

	// state variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int mask_state = 1;

	// member vars

	// functions
	void SetMask(int idx);
};
