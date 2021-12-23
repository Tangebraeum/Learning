// Shoot Them Up Game, All Rights Reserved


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STU_EquipFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent,All,All);

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();
	
	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter*	Character = Cast<ACharacter>(GetOwner());
	if(!Character || !GetWorld()) return;

	for(auto WeaponClass : WeaponClasses)
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
		if(!Weapon) continue;

		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);
		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if(!Weapon || !SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent,AttachmentRules,SocketName);
}


void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	ACharacter*	Character = Cast<ACharacter>(GetOwner());
	if(!Character) return;

	if(CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon,Character->GetMesh(), WeaponArmorySocketName);
	}
	
	CurrentWeapon = Weapons[WeaponIndex];
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire()
{
	if(!CurrentWeapon) return;
	CurrentWeapon->StartFire();
}
void USTUWeaponComponent::StopFire()
{
	if(!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
	CurrentWeaponIndex = (CurrentWeaponIndex+1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character) return;
	Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
	if(!EquipAnimMontage) return;
	
	const auto NotifyEvents = EquipAnimMontage->Notifies;
	for(auto NotifyEvent: NotifyEvents)
	{
		auto EquipFinishedNotify = Cast<USTU_EquipFinishedAnimNotify>(NotifyEvent.Notify);
		if(EquipFinishedNotify)
		{
			EquipFinishedNotify->OnNotify.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
			break;
		}
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character) return;

	if(Character->GetMesh() == MeshComponent)	UE_LOG(LogWeaponComponent, Display, TEXT("EquipFinished"));
}