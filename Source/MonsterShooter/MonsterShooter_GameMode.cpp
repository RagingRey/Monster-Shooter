// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterShooter_GameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


void AMonsterShooter_GameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(CountDown_Handle, this, &AMonsterShooter_GameMode::CountDown, 1.0f, true);

	if(MainHUD_Class)
	{
		MainHUD = CreateWidget(GetWorld(), MainHUD_Class);
		MainHUD->AddToViewport();
	}
}

void AMonsterShooter_GameMode::RestartGameplay(bool Won)
{
	if(Won)
	{
		ResetLevel();
	}
	else
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonsterShooter_GameMode::ResetLevel, 3.0f);
	}
}

void AMonsterShooter_GameMode::ResetLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(GetWorld()->GetName()));
}

void AMonsterShooter_GameMode::CountDown()
{
	TimerCount --;

	if (TimerCount == 0)
		GetWorldTimerManager().ClearTimer(CountDown_Handle);
}
