// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUWeaponSwapService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUWeaponSwapService : public UBTService
{
	GENERATED_BODY()

public:
	USTUWeaponSwapService();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Probability = 0.5f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
