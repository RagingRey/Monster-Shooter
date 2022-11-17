// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishDoor.h"

#include "BaseCharacter.h"
#include "MonsterShooter_GameMode.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFinishDoor::AFinishDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
}

// Called when the game starts or when spawned
void AFinishDoor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFinishDoor::OnHit);
}

// Called every frame
void AFinishDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishDoor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);

	if(Character)
	{
		AMonsterShooter_GameMode* GameMode = Cast<AMonsterShooter_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
			GameMode->RestartGameplay(true);
	}
}