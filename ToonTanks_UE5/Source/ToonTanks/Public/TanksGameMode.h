// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "GameFramework/GameModeBase.h"
#include "TanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATanksGameMode();

	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();
	
private:
	ATank* Tank;

	class ATanksPlayerController* TanksPlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();
};
