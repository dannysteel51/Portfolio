// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AmmoType.h"
#include "ShooterCharacter.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied 			UMETA(DisplayName = "Unoccupied "),
	ECS_FireTimerInProgress	UMETA(DisplayName = "FireTimerInProgress "),
	ECS_Reloading 			UMETA(DisplayName = "Reloading"),
	ECS_Equipping			UMETA(DisplayName = "Equipping"),

	ECS_Max 				UMETA(DisplayName = "DefaultMax")
	
};

USTRUCT(BlueprintType)
struct FInterpLocation
{
	GENERATED_BODY()

	/* Scene component to use for its location for interping*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;
	
	/* Number of items interping to/at this scene comp location*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemCount;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegate, int32, CurrentSlotIndex, int32, NewSlotIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHighlightIconDelegate, int32, slotIndex, bool, bStartAnimation);

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forward/backward input*/
	void MoveForward(float Value);

	/** Called for side to side input*/
	void MoveRight(float Value);

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult);

	/** 
	 * Called via input to look up/down at a given rate
	 * @param rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/** 
	 * Called via input turn at a given rate
	 * @param rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	/** Rotate controller based on mouse X movement
	 * @Param Rate The input value from the mouse
	*/
	void Turn(float Rate);

	/** Rotate controller based on mouse Y movement
	 * @Param Rate The input value from the mouse
	*/
	void LookUp(float Rate);

	/** Set bAiming to true or false with press button*/
	void AimingButtonPressed();
	void AimingButtonReleased();

	void CameraInterpZoom(float DeltaTime);

	void CalculateCrosshairSpread(float DeltaTime);

	void StartCrosshairBulletFire();

	UFUNCTION()
	void FinishCrosshairBulletFire();

	void FireButtonPressed();

	void FireButtonReleased();

	void StartFireTimer();

	UFUNCTION()
	void AutoFireReset();

	/** Line trace for items under the crosshairs*/
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	/** Trace for items if overlapped item count is greater than zero*/
	void TraceForItems();

	/** Spawns a default weapon and equips it*/
	class AWeapon* SpawnDefaultWeapon();

	/** Takes a weapon and attaches it to the mesh*/
	void EquipWeapon(AWeapon* WeaponToEquip, bool bSwapping = false);

	/** Detach weapon and let it fall to the ground*/
	void DropWeapon();

	void SelectButtonPressed();
	void SelectButtonReleased();

	/** Drops currently equipped weapon and equips TraceHitItem*/
	void SwapWeapon(AWeapon* WeaponToSwap);

	/** Intialize the ammo map with the ammo values*/
	void IntializeAmmoMap();

	/** FireWeapon functions*/
	void PlayFireSound();
	void SendBullet();
	void PlayGunfireMontage();

	/** Bound to the R key and the gamepad face button left*/
	void ReloadButtonPressed();

	/** Handle reloading of the weapon*/
	void ReloadWeapon();

	/*Checks to see if we have ammo of the equipped weapon ammo type*/
	bool CarryingAmmo();

	/*Called from the Animation Blueprint with Grab Clip notify*/
	UFUNCTION(BlueprintCallable)
	void GrabClip();

	/* Called from the animation Blueprint */
	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	void CrouchButtonPressed();

	virtual void Jump() override;

	/* Interps capsule half height when crouching/standing*/
	void InterpCapsuleHalfHeight(float DeltaTime);

	void Aim();

	void StopAiming();

	void PickupAmmo(class AAmmo* Ammo);

	void InitializeInterpLocations();

	void FKeyPressed();
	void OneKeyPressed();
	void TwoKeyPressed();
	void ThreeKeyPressed();
	void FourKeyPressed();
	void FiveKeyPressed();

	void ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex);

	int32 GetEmptyInventorySlot();

	void HighlightInventorySlot();

	UFUNCTION(BlueprintCallable)
	EPhysicalSurface GetSurfaceType();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** Camera boom positioning the camera behind the character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Camera that follows the character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rate in degrees per second. Other scaling may affect final turn*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/** base look up/down rate, in deg/sec. Other scaling may affect final turn rate*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	/** Turn rate while not aiming*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;

	/** Look up rate while not aiming*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;

	/** Turn rate while aiming*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;

	/** look up rate while aiming*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate;

	/** Scale factor for mouse look sensitivity. Turn rate when not aiming*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = 1.0, UIMin = 0.0, UIMax = 1.0))
	float MouseHipTurnRate;
	/** Scale factor for mouse look sensitivity. Look up rate when not aiming*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = 1.0, UIMin = 0.0, UIMax = 1.0))
	float MouseHipLookUpRate;

	/** Scale factor for mouse look sensitivity. Turn rate when  aiming*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = 1.0, UIMin = 0.0, UIMax = 1.0))
	float MouseAimingTurnRate;
	/** Scale factor for mouse look sensitivity. Look up rate when  aiming*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = 1.0, UIMin = 0.0, UIMax = 1.0))
	float MouseAimingLookUpRate;

	/** Called when the fire button is pressed*/
	void FireWeapon();

	/** Montage for firing the weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;

	/** particles spawn upon bullet impact*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	/** Smoke trail for bullets*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;

	/** True when aiming*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	/** Deault cmera field of view value*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CameraDefaultFOV;

	/** Field of view value when zoomed in*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CameraZoomFOV;

	/** CurrentField of view this frame*/
	float CameraCurrentFOV;

	/** interp speed for zooming when aiming*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed;

	/** Set baseTurnRate and BaseLookUpRate based on aiming*/
	void SetLookRates();

	/** Determines the spread of the crosshairs*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;

	/** Velocity component for crosshairs spread*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;

	/** Aim component for crosshairs spread*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;

	/** Shooting component for crosshairs spread*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;

	float ShootTimeDuration;

	bool bFiringBullet;

	FTimerHandle CrosshairShootTimer;

	/** Left mouse button or left console trigger pressed*/
	bool bFireButtonPressed;

	/** True when we can fire, false when waiting for the timer*/
	bool bShouldFire;

	/** Set a timer between gunshots*/
	FTimerHandle AutoFireTimer;

	/** True is we should trace every frame for items*/
	bool bShouldTraceForItems;

	/** Number of overlapped AItems*/
	int8 OverlappedItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	class AItem* TraceHitItemLastFrame;

	/** Currently equipped weapon*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	AWeapon* EquippedWeapon;

	/** Set this in blueprints for the defaults weapon class*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	/** The item currently hit by our trace in TraceForItems (could be null)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	AItem* TraceHitItem;

	/** Distance outward from the camera for the interp destination*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	float CameraInterpDistance;

	/** Distance upward from the camera for the interp destination*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	float CameraInterpElevation;

	/** Map to keep track of ammo of the different ammo types*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	TMap<EAmmoType, int32> AmmoMap;

	/* Starting amount of 9mm ammo*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = true))
	int32 Starting9mmAmmo;

	/* Starting amount of AR ammo*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = true))
	int32 StartingARAmmo;

	/** Check to make sure our weapon has ammo*/
	bool WeaponHasAmmo();

	//* Combat state can only fire or reload if unoccupied
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	ECombatState CombatState;

	/** Montage for reload animaiton*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	UAnimMontage* EquipMontage;

	/* Transform of the clip when we first grab the clip during reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FTransform ClipTransform;

	/* Scene component to attach to the character hand during reloading*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	USceneComponent* HandSceneComponent;

	/* True when crouching*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bCrouching;

	/* regular movement speed*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float BaseMovementSpeed;

	/* Crouch movement speed*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float CrouchMovementSpeed;


	/* Current half height of the capsule*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float CurrentCapsuleHalfHeight;

	/* Half height of the capsule when not crouching*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float StandingCapsuleHalfHeight;

	/* Half height of the capsule when crouching*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float CrouchingCapsuleHalfHeight;

	/* Ground friciton while not crouching*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float BaseGroundFriciton;

	/* Ground friction while crouching*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float CrouchingGroundFriction;

	/* Used for knowing when the aiming button is pressed*/
	bool bAimingButtonPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USceneComponent* WeaponInterpComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp6;

	/* Array of interp location structs*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FInterpLocation> InterpLocations;

	FTimerHandle PickupSoundTimer;
	FTimerHandle EquipSoundTimer;

	bool bShouldPlayPickupSound;
	bool bShouldPlayEquipSound;

	void ResetPickupSoundTimer();
	void ResetEquipSoundTimer();

	/* Time to wait before we can play another pickup sound*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	float PickupSoundResetTime;

	/* Time to wait before we can play another equip sound*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	float EquipSoundResetTime;

	/* An Array of AItems for our inventory*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<AItem*> Inventory;

	const int32 INVENTORY_CAPACITY{ 6 };

	/* Delegate for sending slot informaiton to InventoryBar when equipping*/
	UPROPERTY(BlueprintAssignable, Category = "Delegates", meta = (AllowPrivateAccess = true))
	FEquipItemDelegate EquipItemDelegate;

	/* Delegate for sending slot informaiton for playing the icon animation*/
	UPROPERTY(BlueprintAssignable, Category = "Delegates", meta = (AllowPrivateAccess = true))
	FHighlightIconDelegate HighlightIconDelegate;

	/* The index for the currently highlighted slot*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	int32 HighlightedSlot;

public:
	/** Returns cameraboom*/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom ; }

	/** Returns follow camera*/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera ; }

	FORCEINLINE bool GetAiming() const{ return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMulitplier() const;

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount;}

	/** Adds/subtracts to/from overlapped item count and updates bShouldTraceForItems*/
	void IncrementOverlappedItemCount(int8 Amount);

	// No longer need. AItem had GetInterpLocation()
	//FVector GetCameraInterpLocation();

	void GetPickupItem(AItem* Item);

	FORCEINLINE ECombatState GetCombatState() const { return CombatState;  }

	FORCEINLINE bool GetCrouching() const { return bCrouching; }

	FInterpLocation GetInterpLocation(int32 Index);

	/* Returns the index in Interpolations array with the lowest ItemCount*/
	int32 GetInterpLocationIndex();

	void IncrementInterpLocItemCount(int32 Index, int32 Amount);

	FORCEINLINE bool ShouldPlayPickupSound() const { return bShouldPlayPickupSound; }
	FORCEINLINE bool ShouldPlayEquipSound() const { return bShouldPlayEquipSound; }

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon;  }

	void StartPickupSoundTimer();
	void StartEquipSoundTimer();


	void UnhighlightInventorySlot();
};
