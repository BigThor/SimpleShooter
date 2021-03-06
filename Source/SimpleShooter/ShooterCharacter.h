// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	void Shoot();
	void StartReload();
	int GetAmmoLeft() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	FString GetAmmoString() const;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;


private:
	void FinishReload();

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	bool CanSwitchWeapon() const;

	void PreviousWeapon();
	void NextWeapon();
	void ShowCurrentWeapon();
	void HideWeapon(int GunIndex);

	bool IsControllerAShooterPlayerController() const;
	bool IsIndexGunValid(int GunIndex) const;
	void Pause();

private:
	UPROPERTY(EditAnywhere)
	float RotationRate = 10.f;

	UPROPERTY(EditDefaultsOnly)
	TArray< TSubclassOf<class AGun> > GunClassArray;

	UPROPERTY(VisibleAnywhere)
	TArray<class AGun*> GunArray;
	int iCurrentGun = 0;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;
	float Health;

	FTimerHandle ReloadTimer;
	bool IsReloading;
};
