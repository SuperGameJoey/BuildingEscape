// Created in the Udemy Unreal Engine C++ Developer Course

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor();
	float TotalMassOfActors() const;

private:
	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f; // initial value is relative to the door

	float InitialYaw = 0.f;
	float CurrentYaw = 0.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	float DoorLastOpened = 0.f;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 0.5f;

	UPROPERTY()
	UAudioComponent* DoorOpenSound = nullptr;
	
	bool bOpenDoorSound = false;
	bool bCloseDoorSound = false;
};
