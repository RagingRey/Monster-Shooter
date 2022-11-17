// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MonsterShooter_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERSHOOTER_API AMonsterShooter_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> MainHUD_Class;
	TObjectPtr<UUserWidget> MainHUD;

	UPROPERTY(BlueprintReadOnly)
		int TimerCount = 300.0f;

	FTimerHandle CountDown_Handle;

	void RestartGameplay(bool Won);

protected:
	virtual void BeginPlay() override;

private:
	void ResetLevel();
	void CountDown();
};
