#include "LevelTransition.h"

#include "Kismet/GameplayStatics.h"

ALevelTransition::ALevelTransition()
{
	PrimaryActorTick.bCanEverTick = true;

	// Add Tags
	Tags.Add("LevelTransition");

	// setup components
	capsule_comp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsule_comp"));
	SetRootComponent(capsule_comp);
	//capsule_comp->SetupAttachment(RootComponent);
}

void ALevelTransition::BeginPlay()
{
	Super::BeginPlay();

	AActor* camera_actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMyCamera::StaticClass());
	camera = Cast<AMyCamera>(camera_actor);
}

void ALevelTransition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelTransition::MoveCamera()
{
	camera->Move(camera_direction, move_time);
}


