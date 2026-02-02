#include "Collectible.h"

ACollectible::ACollectible()
{
	PrimaryActorTick.bCanEverTick = true;

	// setup components
	capsule_comp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsule_comp"));
	SetRootComponent(capsule_comp);

	collectible_flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("collectible_flipbook"));
	collectible_flipbook->SetupAttachment(RootComponent);

	// Add Tag
	Tags.Add("Collectible");
}

void ACollectible::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("Collectible");
}

void ACollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

