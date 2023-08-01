// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

#include "MyCharacter.generated.h"

UCLASS()
class TPCHARACTER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = "Camera Config")
		UCameraComponent* followCamera;

	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = "Camera Config")
		USpringArmComponent* cameraBoom;

	UPROPERTY(EditAnywhere, Category = "UI Config")
		TSubclassOf<UUserWidget>playerHUD;
	
	class UUserWidget* playerWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float health;

	UPROPERTY(EditAnywhere, Category = "UI Config")
	float healthDecreaseRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDead;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void MoveForward(float axisValue);
	virtual void MoveRight(float axisValue);
	virtual void RestartGame();

	UFUNCTION()
	virtual void BeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
