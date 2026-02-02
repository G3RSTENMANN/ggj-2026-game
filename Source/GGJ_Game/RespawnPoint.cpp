#include "RespawnPoint.h"

ARespawnPoint::ARespawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	// create components
	capsule_comp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsule_comp"));
	SetRootComponent(capsule_comp);
	//capsule_comp->SetupAttachment(RootComponent);

	// Add Tag
	Tags.Add("RespawnPoint");
}

void ARespawnPoint::BeginPlay()
{
	Super::BeginPlay();

	loc = GetActorLocation();
}

void ARespawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

