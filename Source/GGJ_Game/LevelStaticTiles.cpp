#include "LevelStaticTiles.h"

ALevelStaticTiles::ALevelStaticTiles()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelStaticTiles::BeginPlay()
{
	Super::BeginPlay();

	SetMask(mask_state);
}

void ALevelStaticTiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelStaticTiles::SetMask(int idx)
{
	mask_state = idx;
	change_mask_delegate.Broadcast(idx);
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::White, TEXT("Baller"));
}

