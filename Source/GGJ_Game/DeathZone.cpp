#include "DeathZone.h"

ADeathZone::ADeathZone()
{
	PrimaryActorTick.bCanEverTick = true;

	// create components
	capsule_comp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsule_comp"));
	SetRootComponent(capsule_comp);
	//capsule_comp->SetupAttachment(RootComponent);

	// Add Tag
	Tags.Add("DeathZone");
}

void ADeathZone::BeginPlay()
{
	Super::BeginPlay();
}

void ADeathZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ADeathZone::GetDeathBySuffocation(int mask_state) const
{
	return is_inside_of_block && mask_state == death_during_mask_state;
}

