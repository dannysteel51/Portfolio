// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"



UENUM(BlueprintType)
enum class EItemRarity : uint8
{	
	EIR_Damaged 	 UMETA(DisplayName= "Damaged"),
	EIR_Common 		 UMETA(DisplayName = "Common"),
	EIR_Uncommon	 UMETA(DisplayName = "Uncommon"),
	EIR_Rare 		 UMETA(DisplayName = "Rare"),
	EIR_Legendary	 UMETA(DisplayName = "Legendary"),

	EIR_MAX 		 UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{	
	EIS_Pickup 	 			UMETA(DisplayName= "Pickup"),
	EIS_EquipInterping 	 	UMETA(DisplayName= "EquipInterping"),
	EIS_PickedUp 	 		UMETA(DisplayName= "PickedUp"),
	EIS_Equipped			UMETA(DisplayName= "Equipped"),
	EIS_Falling	 			UMETA(DisplayName= "Falling"),

	EIR_MAX 		 		UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Ammo	UMETA(DisplayName= "Ammo"),
	EIT_Weapon	UMETA(DisplayName = "Weapon"),

	EIT_MAX		UMETA(DisplayName = "DefaultMax")
};
USTRUCT(BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DarkColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CustomDepthStencil;
};

UCLASS()
class SHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when overlapping area sphere*/
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

	/** Called when overlapping area end*/
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);	

	/** Sets the ActiveStars array of bools based on rarity*/
	void SetActiveStars();	


	/* Sets properties of the item's state base on State*/
	virtual void SetItemProperties(EItemState State);

	/** Called when ItemInterpTimer is finished*/
	void FinishInterping();

	/** Handles item interpolation when in the equip interping state*/
	void ItemInterp(float DeltaTime);

	/* Get interp location based on the item type*/
	FVector GetInterpLocation();

	void PlayPickupSound(bool bForcePlaySound = false);

	virtual void InitializeCustomDepth();

	//Setting the correct Material instance
	virtual void OnConstruction(const FTransform& Transform) override;

	void EnableGlowMaterial();

	void UpdatePulse();

	void ResetPulseTimer();

	void StartPulseTimer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called in AShootercharacter::GetPickupItem
	void PlayEquipSound(bool bForcePlaySound = false);

private:

	/** Skeletal mesh for the item*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* ItemMesh;

	/** Line trace collides with box to show hud widgets*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UBoxComponent* CollisionBox;

	/** Popup widget for when player looks at item*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* PickupWidget;

	/** Enables item tracing when overlap*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class USphereComponent* AreaSphere;
	
	/** The name which appears on the pickup widget*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	FString ItemName;

	/** The count which appears on the pickup widget*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	int32 ItemCount;

	/** Item rarity determines number of stars in weapon widget*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = true))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	TArray<bool> ActiveStars;

	/** State of the item*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	EItemState ItemState;

	// The Curve asset to use for the item's Z location when interping
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UCurveFloat* ItemZCurve;

	/** Starting location when interping begins*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	FVector ItemInterpStartLocation;

	/** Target interp location in front of the camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	FVector CameraTargetLocation;

	/* True when interping*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	bool bInterping;

	/** plays when we start interping*/
	FTimerHandle ItemInterpTimer;
	/** Duration of the curve and timer*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	float ZCurveTime;

	/** Point to the character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class AShooterCharacter* Character;

	/** X and Y for the Item while interping in the EquipInterpingState*/
	//float ItemInterpX;
	//float ItemInterpY;

	/** Initial yaw offset between the camera and the interping item*/
	float InterpIntialYawOffset;

	/** Curve used to scale the item when interping*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	UCurveFloat* ItemScaleCurve;

	/* Sound played when item is picked up*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class USoundCue* PickupSound;

	/* Sound played when the item is equipped*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	USoundCue* EquipSound;

	/* Enum for the type of item this item is*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	EItemType ItemType;

	/* Index of the interp location this item is interping to*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	int32 InterpLocIndex;

	/* Index for the material we'd like to change at runtime*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	int32 MaterialIndex;

	/* Dynamic instance that we can change at runtime*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	/* Material instance used with the dynamic material instance*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	UMaterialInstance* MaterialInstance;
	
	bool bCanChangeCustomDepth;

	/* Curve to drive the dynamic material parameter*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UCurveVector* PulseCurve;

	/* Curve to drive the dynamic material parameter*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	UCurveVector* InterpPulseCurve;

	FTimerHandle PulseTimer;

	/* time for the pulsetimer*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	float PulseCurveTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	float GlowAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	float FresnelExponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	float FresnelReflectFraction;

	/* The icon for this item in the inventory*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	UTexture2D* IconItem;

	/* The icon for the ammo for this item*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	UTexture2D* AmmoIcon;

	/* Slot in the inventory array*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	int32 SlotIndex;

	/* True when the character's inventory is full*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	bool bCharacterInventoryFull;

	/* Item Rarity Data Table*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = true))
	class UDataTable* ItemRarityDataTable;

	/* Color in the glow material*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = true))
	FLinearColor GlowColor;

	/* Light color in the pickup widget*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = true))
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = true))
	FLinearColor DarkColor;

	/* Number of stars in the pickup widget*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = true))
	int32 NumberOfStars;

	/* Background icon for the inventory*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = true))
	UTexture2D* IconBackground;

public:		
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget ;}

	FORCEINLINE USphereComponent* GetAreaSphere() const {return AreaSphere; }

	FORCEINLINE UBoxComponent* GetCollisionBox() const {return CollisionBox; }

	FORCEINLINE EItemState GetItemState() const {return ItemState; }

	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound;  }

	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }

	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }

	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex;  }

	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	
	FORCEINLINE void SetCharacter(AShooterCharacter* Char) { Character = Char; }

	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex = Index; }

	FORCEINLINE void SetCharacterInventoryFull(bool bFull) { bCharacterInventoryFull = bFull; }
	
	FORCEINLINE void SetPickupSound(USoundCue* Sound) {PickupSound = Sound;}

	FORCEINLINE void SetEquipSound(USoundCue* Equip) { EquipSound = Equip; }

	FORCEINLINE void SetItemName(FString Name) { ItemName = Name; }

	/* Set item icon for the inventory*/
	FORCEINLINE void SetIconItem(UTexture2D* Icon) { IconItem = Icon; }
	/* Set ammo icon for the pickup widget*/
	FORCEINLINE void SetAmmoIcon(UTexture2D* Icon) { AmmoIcon = Icon; }

	FORCEINLINE void SetMaterialInstance(UMaterialInstance* Instance) { MaterialInstance = Instance; }

	FORCEINLINE UMaterialInstance* GetMaterialInstance() const { return MaterialInstance;  }

	FORCEINLINE UMaterialInstanceDynamic* GetDynamicMaterialInstance() const { return DynamicMaterialInstance; }

	FORCEINLINE void SetDynamicMaterialInstance(UMaterialInstanceDynamic* Instance) { DynamicMaterialInstance = Instance;  }

	FORCEINLINE FLinearColor GetGlowColor() const { return GlowColor;}

	FORCEINLINE int32 GetMaterialIndex() const { return MaterialIndex;  }

	FORCEINLINE void SetMaterialIndex(int32 Index) {MaterialIndex = Index; }

	void SetItemState(EItemState State);

	/** Called from the AShooterCharacter class*/
	void StartItemCurve(AShooterCharacter* Char, bool bForcePlaySound = false);

	virtual void EnableCustomDepth();

	virtual void DisableCustomDepth();

	void DisableGlowMaterial();
};
