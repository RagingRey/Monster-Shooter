// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(this->RootComponent);
	BoxCollision->SetBoxExtent(FVector(32.0f, 60.0f, 112.0f));
	BoxCollision->SetRelativeLocation(FVector(41.0f, 0, 0));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("AI Sight");
	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");

	SightConfig->SightRadius = 1250.0f;
	SightConfig->LoseSightRadius = 1280.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(0.1f);

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &ABaseEnemy::OnSensed);

	CurrentVelocity = FVector::ZeroVector;
	MovementSpeed = 375.0f;

	DistanceSquared = BIG_NUMBER;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnHit);

	BaseLocation = this->GetActorLocation();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!CurrentVelocity.IsZero())
	{
		NewLocation = this->GetActorLocation() + CurrentVelocity * DeltaTime;

		if(BackToBaseLocation)
		{
			if((NewLocation - BaseLocation).SizeSquared2D() < DistanceSquared)
			{
				DistanceSquared = (NewLocation - BaseLocation).SizeSquared2D();
			}
			else
			{
				CurrentVelocity = FVector::Zero();
				DistanceSquared = BIG_NUMBER;
				BackToBaseLocation = false;

				SetNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}

		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::SetNewRotation(FVector TargetPosition, FVector CurrentPosition)
{
	FVector NewDirection = TargetPosition - CurrentPosition;
	NewDirection.Z = 0.0f;

	EnemyRotation = NewDirection.Rotation();

	SetActorRotation(EnemyRotation);
}

void ABaseEnemy::OnSensed(const TArray<AActor*>& UpdatedActor)
{
	for(int i {0}; i < UpdatedActor.Num(); i++)
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerComp->GetActorsPerception(UpdatedActor[i], Info);

		if(Info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			FVector Dir = UpdatedActor[i]->GetActorLocation() - this->GetActorLocation();
			Dir.Z = 0;

			CurrentVelocity = Dir.GetSafeNormal() * MovementSpeed;

			SetNewRotation(UpdatedActor[i]->GetActorLocation(), this->GetActorLocation());
		}
		else
		{
			FVector Dir = BaseLocation - this->GetActorLocation();
			Dir.Z = 0;

			if(Dir.SizeSquared2D() > 1.0f)
			{
				CurrentVelocity = Dir.GetSafeNormal() * MovementSpeed;
				BackToBaseLocation = true;

				SetNewRotation(BaseLocation, this->GetActorLocation());
			}

		}
	}
}

void ABaseEnemy::DealDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if(Health <= 0.0f)
	{
		Destroy();
	}
}

void ABaseEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);

	if(Character)
	{
		Character->DealDamage(DamageValue);
	}
}
