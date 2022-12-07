// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Engine/DataTable.h"
#include "WeaponType.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaterialIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClipBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneToHide;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadShotDamage;
};

/**
 * 
 */
UCLASS()
class SHOOTER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:

	void StopFalling();

	virtual void OnConstruction(const FTransform& Transform) override;

	void FinishMovingSlide();

private:

	virtual void BeginPlay() override;

	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	/** Ammo count for this weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	/* Maximum ammo that our weapon can hold*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

	/** The type of weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	/* The type of ammo for this weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	/* FName for the reload montage section*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;

	/* Ture when moving the clip while reloading*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bMovingClip;

	/* Name of the clip bone*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ClipBoneName;

	/* data table for weapon properties*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponDataTable;

	int32 PreviousMaterialIndex;

	/* Textures for the weapon crosshairs*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsBottom;

	/* The speed at which automatic fire happens*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	float AutoFireRate;

	/* Particle system spawned at the BarrelSocket*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	/* Sound played whehn the weapon is fired*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	USoundCue* FireSound;

	/* Name of the bone to hide on the weapon mesh*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	FName BoneToHide;

	/* Amount that the slide is pushed back during the pistol fire*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pistol", meta = (AllowPrivateAccess = "true"))
	float SlideDisplacement;

	/* Curve for the slide displacement*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SlideDisplacementCurve;

	/* Timer handle for updating slideDisplacement*/
	FTimerHandle SlideTimer;

	/* Time for displacing the slide during the pistol fire*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol", meta = (AllowPrivateAccess = "true"))
	float SlideDisplacementTime;

	/* True when moving the pistol slide*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pistol", meta = (AllowPrivateAccess = "true"))
	bool bMovingSlide;

	/* Max distance for the slide on the pistol*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol", meta = (AllowPrivateAccess = "true"))
	float MaxSlideDisplacement;

	/* Max rotation for pistol recoil*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol", meta = (AllowPrivateAccess = "true"))
	float MaxRecoilRotation;

	/* The amount that the pistol will rotate while firing*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pistol", meta = (AllowPrivateAccess = "true"))
	float RecoilRotation;

	/* True for auto gun fire*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bAutomatic;

	void UpdateSlideDisplacement();

	/* Amount of damage caused by bullet*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float Damage;

	/* Amount of damage when bullet hits the head*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		float HeadShotDamage;

public:

	// Adds impulse to the weapon	
	void ThrowWeapon();

	// public getter for the Ammo() function
	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity;  }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection;  }

	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }

	FORCEINLINE void SetClipBoneName(FName name) { ClipBoneName = name; }

	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move;  }

	FORCEINLINE void SetReloadMontageSection(FName name) { ReloadMontageSection = name;  }

	FORCEINLINE float GetAutoFireRate() const { return AutoFireRate;}

	FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }

	FORCEINLINE USoundCue* GetFireSound() const { return FireSound; }

	FORCEINLINE bool GetAutomatic() const { return bAutomatic; }

	FORCEINLINE float GetDamage() const { return Damage; }

	FORCEINLINE float GetHeadShotDamage() const { return HeadShotDamage; }

	void StartSlideTimer();

	/** Called from character class when firing weapon*/
	void DecrementAmmo();

	void ReloadAmmo(int32 Amount);

	bool ClipIsFull();

	
	
};
