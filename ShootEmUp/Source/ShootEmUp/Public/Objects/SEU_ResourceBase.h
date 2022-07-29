// Pupa&Lupa sftwr. All rights reserved!

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SEU_ResourceBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnResourceValueChangedSignature, float);

USTRUCT(BlueprintType)
struct FResourceParams
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "Resource parameters", 
		meta=(DisplayName="Auto update enabled"))
	bool bUpdateEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category = "Resource parameters", 
		meta = (EditCondition="bUpdateEnabled", ClampMin="1.0", ClampMax="100.0", ToolTip="Maximum value on which update will stop"))
	float UpdateMaxThreshold = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Resource parameters",
		meta = (EditCondition = "bUpdateEnabled", ClampMin = "1.0", ClampMax = "100.0",
			ToolTip = "Minimum value on which update will stop"))
	float UpdateMinThreshold = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Resource parameters", 
		meta = (EditCondition="bUpdateEnabled", ClampMin="1.0", ClampMax="100.0", ToolTip="Initial delay before start updating the value"))
	float UpdateInitialDelay = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Resource parameters",
		meta = (EditCondition = "bUpdateEnabled", ClampMin = "1.0", ClampMax = "100.0", ToolTip="Frequency of value updates"))
	float UpdateFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Resource parameters",
		meta = (EditCondition = "bUpdateEnabled", ClampMin = "1.0", ClampMax = "100.0"))
	float UpdateValue = 1.f;
};

UCLASS()
class SHOOTEMUP_API USEU_ResourceBase : public UObject
{
	GENERATED_BODY()
	
	public:

		FOnResourceValueChangedSignature OnResourceValueChanged;
		
		/** Set or Return whole structure*/
		void SetResourceParams(const FResourceParams& NewResourceParams);
		FResourceParams GetResourceParams() const { return ResourceParams;}
		virtual void HandleDamage(float& Damage);

	protected:

		FResourceParams ResourceParams;
		FTimerHandle UpdateValueTimerHandle;

		void UpdateResourceValue(const float DeltaValue);
		void UpdateTimerState(const bool bIsActive);
};
