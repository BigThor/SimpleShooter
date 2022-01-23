// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	// Hide bone gun
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
	
	for (int IndexGunClass = 0; IndexGunClass < GunClassArray.Num(); IndexGunClass++)
	{
		TSubclassOf<AGun> GunClass = GunClassArray[IndexGunClass];

		AGun* NewGun = GetWorld()->SpawnActor<AGun>(GunClass);
		NewGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		NewGun->SetOwner(this);
		GunArray.Add(NewGun);

		if (IndexGunClass == iCurrentGun)
		{
			ShowCurrentWeapon();
		}
		else
		{
			HideWeapon(IndexGunClass);
		}
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	// Binds used for mouse camera rotation support
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	// Different binds for controller support to make it framerate independent
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);

	PlayerInputComponent->BindAction(TEXT("NextWeapon"), IE_Pressed, this, &AShooterCharacter::NextWeapon);
	PlayerInputComponent->BindAction(TEXT("PrevWeapon"), IE_Pressed, this, &AShooterCharacter::PreviousWeapon);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}


void AShooterCharacter::LookUpRate(float AxisValue)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(AxisValue * RotationRate * DeltaTime);
}


void AShooterCharacter::LookRightRate(float AxisValue)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(AxisValue * RotationRate * DeltaTime);
}

void AShooterCharacter::Shoot()
{
	if (GunArray.Num() > 0 && iCurrentGun < GunArray.Num())
	{
		GunArray[iCurrentGun]->PullTrigger();
	}
}

void AShooterCharacter::PreviousWeapon()
{
	if (GunArray.Num() < 2)
		return;

	HideWeapon(iCurrentGun);
	iCurrentGun = (iCurrentGun + GunArray.Num() -1) % GunArray.Num();
	ShowCurrentWeapon();
}

void AShooterCharacter::NextWeapon()
{
	if (GunArray.Num() < 2)
		return;

	HideWeapon(iCurrentGun);
	iCurrentGun = (iCurrentGun + GunArray.Num() - 1) % GunArray.Num();
	ShowCurrentWeapon();
}

void AShooterCharacter::ShowCurrentWeapon()
{
	if (GunArray.Num() > 0 && iCurrentGun < GunArray.Num())
	{
		GunArray[iCurrentGun]->SetActorHiddenInGame(false);
		GunArray[iCurrentGun]->SetActorEnableCollision(true);
	}
}

void AShooterCharacter::HideWeapon(int GunIndex)
{
	if (GunArray.Num() > 0 && GunIndex < GunArray.Num())
	{
		GunArray[GunIndex]->SetActorHiddenInGame(true);
		GunArray[GunIndex]->SetActorEnableCollision(false);
	}
}



float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float DamageApplied = FMath::Min(Health, DamageToApply);
	Health -= DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Health left: %f"), Health);

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageApplied;
}



float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}