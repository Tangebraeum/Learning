// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_BaseActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseActor, All,All);

// Sets default values
AGS_BaseActor::AGS_BaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGS_BaseActor::BeginPlay()
{
	Super::BeginPlay();

	// FString	Name = "John";
	//
	// UE_LOG(LogBaseActor, Warning, TEXT("Hello Ureal! %s Again :)"), *Name);
	//
	// GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,Name);

	FTransform ActorTransform = GetActorTransform(); // Get actors Transform
	FVector Location = ActorTransform.GetLocation(); // Get actors location
	FRotator Rotation = ActorTransform.Rotator(); // Get actors Rotation
	FVector Scale = ActorTransform.GetScale3D(); // Get actors Scale

	InitialLocation = GetActorLocation();

	SetColor(GeometryData.MaterialColor);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGS_BaseActor::OnTimerFired, GeometryData.TimerRate, true);
}

void AGS_BaseActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	
}

// Called every frame
void AGS_BaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement(DeltaTime);
	// z - z0 + amplitude * sin(freq * t) sinusoidal movement law

}

void AGS_BaseActor::HandleMovement(const float &DeltaTime)
{
	switch (GeometryData.MovementType)
	{
		case EMovementType::Sin:
		{
			FVector CurrentLocation = GetActorLocation();
			CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * GetWorld()->GetTimeSeconds());
			SetActorLocation(CurrentLocation);
			break;
		}
		case EMovementType::Static: break;
		default: break;
	}
}

void AGS_BaseActor::SetColor(const FLinearColor& Color)
{
	UMaterialInstanceDynamic* DynMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
    	if (DynMaterial)
    	{
    		DynMaterial->SetVectorParameterValue("Color",Color); //FLinearColor - 32bit, FColor - 8bit
    	}
}

void AGS_BaseActor::OnTimerFired()
{
	if(++TimerCount <= MaxTimerCount)
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		SetColor(NewColor);
		OnColorChanged.Broadcast(NewColor, GetName());
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		OnTimerFinished.Broadcast(this);
	}
}
