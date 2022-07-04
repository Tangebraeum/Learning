// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GS_BaseActor.generated.h"

//for dynamic delegates we need to point param names - it would be visible in blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnColorChanged, const FLinearColor&, Color, const FString&, Name);

//only visible in c++ - no need to assign param name
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimerFinished, AActor*);

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Sin,
	Static
};

USTRUCT(BlueprintType)
struct FGeometryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    float Amplitude = 50.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    float Frequency = 2.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    EMovementType MovementType = EMovementType::Static;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design")
	FLinearColor MaterialColor = FLinearColor::Black;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design")
	float TimerRate = 3.0f;
};


UCLASS()
class GEOMETRYSANDBOX_API AGS_BaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGS_BaseActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	void SetGeometryData(FGeometryData& Data){GeometryData = Data;}

	UFUNCTION(BlueprintCallable)
	FGeometryData GetGeometryData() const {return GeometryData;}

	UPROPERTY(BlueprintAssignable) //for BP delegates
	FOnColorChanged OnColorChanged;

	
	FOnTimerFinished OnTimerFinished;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	float WeaponsNum = 4.f;
	
	UPROPERTY(EditAnywhere, Category="GeometryData")
	FGeometryData GeometryData;	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void HandleMovement(const float &DeltaTime);
private:
	FVector InitialLocation;

	void SetColor(const FLinearColor& Color);

	FTimerHandle TimerHandle;

	void OnTimerFired();

	int32 TimerCount;
	const int32 MaxTimerCount = 5;
	
};
