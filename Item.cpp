// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Curves/CurveVector.h"

// Sets default values
AItem::AItem() :

	ItemName(FString("Default")),
	ItemCount(0),
	ItemRarity(EItemRarity::EIR_Common),
	ItemState(EItemState::EIS_Pickup),

	// Item interp variables
	ZCurveTime(0.7f),
	ItemInterpStartLocation(FVector(0.f)),
	CameraTargetLocation(FVector(0.f)),
	bInterping(false),

	//ItemInterpX(0.f),
	//ItemInterpY(0.f)
	InterpIntialYawOffset(0.f),
	ItemType(EItemType::EIT_MAX),
	InterpLocIndex(0),
	MaterialIndex(0),
	bCanChangeCustomDepth(true),

	//Dynamic Material Parameters
	GlowAmount(150.f),
	FresnelExponent(3.0f),
	FresnelReflectFraction(4.0f),
	PulseCurveTime(5.f),

	SlotIndex(0),
	bCharacterInventoryFull(false)

{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ItemMesh);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Hide PickupWidget
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	//Set active stars based on rarity
	SetActiveStars();

	// Setup overlap for area sphere
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	//set item properties based on item state
	SetItemProperties(ItemState);
	
	/* Set custom depth to disabled*/
	InitializeCustomDepth();

	StartPulseTimer();
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			ShooterCharacter->IncrementOverlappedItemCount(1);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			ShooterCharacter->IncrementOverlappedItemCount(-1);
			ShooterCharacter->UnhighlightInventorySlot();
		}
	}
}

/** cycles through array and sets to false*/
void AItem::SetActiveStars()
{
	// the zero element isn't used
	for (int32 i = 0; i <=5; i++)
	{
		ActiveStars.Add(false);

	}
	/** This switch case then sets the star rarity for item*/
	switch (ItemRarity)
	{
		case EItemRarity::EIR_Damaged:

		ActiveStars[1] = true;
		break;

		case EItemRarity::EIR_Common:
		ActiveStars[1] = true;
		ActiveStars[2] = true;

		break;

		case EItemRarity::EIR_Uncommon:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;

		break;

		case EItemRarity::EIR_Rare:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		ActiveStars[4] = true;

		break;

		case EItemRarity::EIR_Legendary:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		ActiveStars[4] = true;
		ActiveStars[5] = true;

		break;

	}
}

void AItem::SetItemProperties(EItemState State)
{
	switch(State)
	{
		case EItemState::EIS_Pickup:
			// Set mesh properties
			ItemMesh->SetSimulatePhysics(false);
			ItemMesh->SetEnableGravity(false),
			ItemMesh->SetVisibility(true);
			ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			// Area sphere properties
			AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);		

			//Set collision box properties
			CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
			CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			break;

		case EItemState::EIS_Equipped :

			PickupWidget->SetVisibility(false);
			// Set mesh properties
			ItemMesh->SetSimulatePhysics(false);
			ItemMesh->SetEnableGravity(false);
			ItemMesh->SetVisibility(true);
			ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			// Area sphere properties
			AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);	

			//Set collision box properties
			CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);


			break;	

		case EItemState::EIS_Falling : 

		// Set mesh properties
			ItemMesh->SetVisibility(true);
			ItemMesh->SetSimulatePhysics(true);
			ItemMesh->SetEnableGravity(true);
			ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

			// Area sphere properties
			AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);	

			//Set collision box properties
			CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);


			break;	

		case EItemState::EIS_EquipInterping	:

			PickupWidget->SetVisibility(false);

			// Set Item mesh properties
			ItemMesh->SetSimulatePhysics(false);
			ItemMesh->SetEnableGravity(false);
			ItemMesh->SetVisibility(true);
			ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			// Area sphere properties
			AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);	

			//Set collision box properties
			CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			break;

		case EItemState::EIS_PickedUp :

			PickupWidget->SetVisibility(false);

			// Set Item mesh properties
			ItemMesh->SetSimulatePhysics(false);
			ItemMesh->SetEnableGravity(false);
			ItemMesh->SetVisibility(false);
			ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			// Area sphere properties
			AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set collision box properties
			CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			break;
	}
}

void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperties(State);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Handle item interping when in the equipinterping state
	ItemInterp(DeltaTime);

	// Get curve values from PulseCurve and set dynamic material parameters
	UpdatePulse();
}

void AItem::StartItemCurve(AShooterCharacter* Char, bool bForcePlaySound)
{
	// store a handle to the character
	Character = Char;

	/* Get Array index and InterpLocations with the lowest item count*/
	InterpLocIndex = Character->GetInterpLocationIndex();

	// Add one to the item count for this interp location struct
	Character->IncrementInterpLocItemCount(InterpLocIndex, 1);

	PlayPickupSound(bForcePlaySound);

	//Store intial location of the item
	ItemInterpStartLocation = GetActorLocation();
	bInterping = true;
	SetItemState(EItemState::EIS_EquipInterping);
	GetWorldTimerManager().ClearTimer(PulseTimer);

	GetWorldTimerManager().SetTimer(ItemInterpTimer, this, &AItem::FinishInterping, ZCurveTime);

	/* Get initial yaw of the camera*/
	const double CameraRotationYaw {Character->GetFollowCamera()->GetComponentRotation().Yaw };
	/** Get initial yaw of the item*/
	const double ItemRotationYaw {GetActorRotation().Yaw};
	/** Initial yaw offset between camera and itam*/
	InterpIntialYawOffset = ItemRotationYaw - CameraRotationYaw;

	bCanChangeCustomDepth = false;
}

void AItem::FinishInterping()
{	
	bInterping = false;
	if (Character)
	{
		//Subtract one from the item count for the interp location struct
		Character->IncrementInterpLocItemCount(InterpLocIndex, -1);
		Character->GetPickupItem(this);

		Character->UnhighlightInventorySlot();
	}
	//Set scale back to normal
	SetActorScale3D(FVector(1.f));

	DisableGlowMaterial(); //Disables the glow
	bCanChangeCustomDepth = true;
	DisableCustomDepth(); //Disables the outline of the weapon
}

void AItem::ItemInterp(float DeltaTime)
{
	if (!bInterping) return;

	if(Character && ItemZCurve)
	{
		// Elapsed time since we started ItemInterpTimer
		const float ElapsedTime = GetWorldTimerManager(). GetTimerElapsed(ItemInterpTimer);
		//Get curve value corresponding to elapsedtime
		const float CurveValue = ItemZCurve->GetFloatValue(ElapsedTime);

		// Get the items intial location when the curve starts
		FVector ItemLocation = ItemInterpStartLocation;
		//Get location in front of camera
		const FVector CameraInterpLocation{GetInterpLocation()};

		// Vector from item to camera interp location. X and Y are zeroed out
		const FVector ItemToCamera{ FVector(0.f, 0.f, (CameraInterpLocation - ItemLocation).Z)};
		//Scale factor to multiply with CurveValue
		const float DeltaZ = ItemToCamera.Size();

		const FVector CurrentLocation{GetActorLocation()};

		//Interpolated X value
		const float InterpXValue = FMath::FInterpTo(CurrentLocation.X, CameraInterpLocation.X, DeltaTime, 30.0);
		//Interpolated Y value
		const float InterpYValue = FMath::FInterpTo(CurrentLocation.Y, CameraInterpLocation.Y, DeltaTime, 30.0);

		// Set X and Y of ItemLocation to Interped values
		ItemLocation.X = InterpXValue;
		ItemLocation.Y = InterpYValue;

		// Adding curve value to the Z component of the Initial location (Scaled by DeltaZ)
		ItemLocation.Z += CurveValue * DeltaZ;
		SetActorLocation(ItemLocation, true, nullptr, ETeleportType::TeleportPhysics);

		// Camera rotation this frame
		const FRotator CameraRotation {Character->GetFollowCamera()->GetComponentRotation()};
		//camera rotation plus intial yaw offset
		const FRotator ItemRotation {0.f, CameraRotation.Yaw + InterpIntialYawOffset, 0.f};
		SetActorRotation(ItemRotation, ETeleportType::ResetPhysics);

		if (ItemScaleCurve)
		{
			const float ScaleCurveValue = ItemScaleCurve->GetFloatValue(ElapsedTime);
			// Scales dows the mesh (In FinishInterping it is sized up again)
			SetActorScale3D(FVector(ScaleCurveValue, ScaleCurveValue, ScaleCurveValue));
		}
		

	}
}

FVector AItem::GetInterpLocation()
{
	if (Character == nullptr) return FVector(0.f);

	switch (ItemType)
	{
	case EItemType::EIT_Ammo : 

		return Character->GetInterpLocation(InterpLocIndex).SceneComponent->GetComponentLocation();

		break;

	case EItemType::EIT_Weapon :

		return Character->GetInterpLocation(0).SceneComponent->GetComponentLocation();

		break;

	}
	return FVector();
}

void AItem::PlayPickupSound(bool bForcePlaySound)
{
	if (Character)
	{
		if (bForcePlaySound)
		{
			UGameplayStatics::PlaySound2D(this, PickupSound);
		}
		else if (Character->ShouldPlayPickupSound())
		{
			Character->StartPickupSoundTimer();
			{
				if (PickupSound)
				{
					UGameplayStatics::PlaySound2D(this, PickupSound);
				}
			}
		}
	}
}


void AItem::PlayEquipSound(bool bForcePlaySound)
{
	if (Character)
	{
		if (bForcePlaySound)
		{
			if (EquipSound)
			{
				UGameplayStatics::PlaySound2D(this, EquipSound);
			}
		}
		else if (Character->ShouldPlayEquipSound())
			{
				Character->StartEquipSoundTimer();
				{
					if (EquipSound)
					{
						UGameplayStatics::PlaySound2D(this, EquipSound);
					}
				}
			}
		
	}
}


void AItem::EnableCustomDepth()
{
	if (bCanChangeCustomDepth)
	{
		ItemMesh->SetRenderCustomDepth(true); //built in function to set the custom depth in the editor (this case, turns on)
	}
	
}

void AItem::DisableCustomDepth()
{
	if (bCanChangeCustomDepth)
	{
		ItemMesh->SetRenderCustomDepth(false);//built in function to set the custom depth in the editor (this case, turns off)
	}
	
}

void AItem::InitializeCustomDepth()
{
	DisableCustomDepth();
}

//using the construction node from C++ instead of the editor
void AItem::OnConstruction(const FTransform& Transform)
{
	/* Load data in the ItemRarityDataTable*/
	FString RarityTablePath(TEXT("DataTable'/Game/_Game/DataTable/ItemRarityDataTable.ItemRarityDataTable'")); //Path to the itemraritydatatable
	UDataTable* RarityTableObject = Cast <UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RarityTablePath));
	if (RarityTableObject)
	{
		FItemRarityTable* RarityRow = nullptr;

		switch (ItemRarity)
		{
		case EItemRarity::EIR_Damaged :

			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("Damaged"), TEXT(""));

			break;

		case EItemRarity::EIR_Common:

			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("Common"), TEXT(""));

			break;

		case EItemRarity::EIR_Uncommon:

			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("Uncommon"), TEXT(""));

			break;

		case EItemRarity::EIR_Rare : 

			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("Rare"), TEXT(""));

			break;

		case EItemRarity::EIR_Legendary :

			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("Legendary"), TEXT(""));

			break;
		}

		if (RarityRow)
		{
			GlowColor = RarityRow->GlowColor;
			LightColor = RarityRow->LightColor;
			DarkColor = RarityRow->DarkColor;
			NumberOfStars = RarityRow->NumberOfStars;
			IconBackground = RarityRow->IconBackground;
			if (GetItemMesh())
			{
				GetItemMesh()->SetCustomDepthStencilValue(RarityRow->CustomDepthStencil);
			}
		}
	}
	if (MaterialInstance)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInstance, this);
		DynamicMaterialInstance->SetVectorParameterValue(TEXT("FresnelColor"), GlowColor);
		ItemMesh->SetMaterial(MaterialIndex, DynamicMaterialInstance);
		EnableGlowMaterial(); //Activating the glow material; happens before the game launches
	}
}

//Setting the parameter in the in constructon in editor to 0
void AItem::EnableGlowMaterial()
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("GlowBlendAlpha"), 0); // Sets the parameter I made in the editor called GlowBlendAlpha to 0 (off)
	}
}

//Setting the parameter in the in constructon in editor to 1
void AItem::DisableGlowMaterial()
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("GlowBlendAlpha"), 1);// Sets the parameter I made in the editor called GlowBlendAlpha to 1 (on)
	}
	
}


void AItem::ResetPulseTimer()
{
	StartPulseTimer();
}

void AItem::StartPulseTimer()
{

	if (ItemState == EItemState::EIS_Pickup)
	{
		GetWorldTimerManager().SetTimer(PulseTimer, this, &AItem::ResetPulseTimer, PulseCurveTime);
	}
}


void AItem::UpdatePulse()
{
	float ElapsedTime{};
	FVector CurveValue{};

	switch (ItemState)
	{
	case EItemState::EIS_Pickup :

		if (PulseCurve)
		{
			ElapsedTime = GetWorldTimerManager().GetTimerElapsed(PulseTimer);
			CurveValue = PulseCurve->GetVectorValue(ElapsedTime);
		}

		break;
	
	case EItemState::EIS_EquipInterping : 
			
		if (InterpPulseCurve)
		{
			ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpTimer);
			CurveValue = InterpPulseCurve->GetVectorValue(ElapsedTime);
		}

		break;
	}
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("GlowAmount"), CurveValue.X * GlowAmount);
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("FresnelExponent"), CurveValue.Y * FresnelExponent);
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("FresnelReflectFraction"), CurveValue.Z * FresnelReflectFraction);
	}

}


