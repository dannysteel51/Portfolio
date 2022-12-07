// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BulletHitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTER_API AEnemy : public ACharacter, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

	void Die();

	void PlayHitMontage(FName Section, float PlayRate = 1.0f);

	void ResetHitReactTimer();

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector Location);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

	void UpdateHitNumbers();

private:

	/* Particles to spawn when hit by bullets*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ImpactParticles;

	/* Sound to play when hit by bullets*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Combat", meta = (AllowPrivateAccess = "true"))
	class USoundCue* ImpactSound;

	/* Current Health of the enemy*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Combat", meta = (AllowPrivateAccess = "true"))
	float Health;
	/* Max health of the enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	/* Name of the head bone*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Combat", meta = (AllowPrivateAccess = "true"))
	FString HeadBone;

	/* time to display health bar once shot*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Combat", meta = (AllowPrivateAccess = "true"));
	float HealthBarDisplayTime;

	FTimerHandle HealthBarTimer;

	/* Montage containing hit and death animations*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Combat", meta = (AllowPrivateAccess = "true"));
	UAnimMontage* HitMontage;

	FTimerHandle HitReactTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Combat", meta = (AllowPrivateAccess = "true"));
	float HitReactTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Combat", meta = (AllowPrivateAccess = "true"));
	float HitReactTimeMax;

	bool bCanHitReact;

	/* Map to store HitNumber widgets and their hit locations*/
	UPROPERTY(VisibleAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"));
	TMap<UUserWidget*, FVector> HitNumbers;

	/* Time Before a hit number is removed from the screen*/
	UPROPERTY(EditAnywhere, category = "Combat", meta = (AllowPrivateAccess = "true"));
	float HitNumberDestroyTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult) override;

	FORCEINLINE FString GetHeadBone() const { return HeadBone; }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 Damage, FVector HitLocation);

};
