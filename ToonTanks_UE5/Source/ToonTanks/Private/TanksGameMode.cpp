// Fill out your copyright notice in the Description page of Project Settings.


#include "TanksGameMode.h"
#include "Tank.h"
#include "TanksPlayerController.h"
#include "Turret.h"
#include "Kismet/GameplayStatics.h"

ATanksGameMode::ATanksGameMode()
{
	DefaultPawnClass = ATank::StaticClass();
}

void ATanksGameMode::ActorDied(AActor* DeadActor)
{
	if(DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if(TanksPlayerController)
		{
			TanksPlayerController->SetPlayerEnabled(false);
		}
	}
	else if (ATurret* DestroyedTurret = Cast<ATurret>(DeadActor))
	{
		DestroyedTurret->HandleDestruction();
	}
}

void ATanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}

void ATanksGameMode::HandleGameStart()
{
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	TanksPlayerController = Cast<ATanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));

	StartGame();
	
	if(TanksPlayerController)
	{
		TanksPlayerController->SetPlayerEnabled(false);

		FTimerHandle PlayerEnableTimerhandle;
		FTimerDelegate PlayerEnabledTimerDelegate = FTimerDelegate::CreateUObject(TanksPlayerController,&ATanksPlayerController::SetPlayerEnabled,true);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerhandle,PlayerEnabledTimerDelegate, StartDelay,false);
	}
}
