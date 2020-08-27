// Created in the Udemy Unreal Engine C++ Developer Course

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has an Open Door component, but does not have a Pressure Plate set!"), *GetOwner()->GetName());
	}

	DoorOpenSound = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (DoorOpenSound == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find an Audio Component on %s"), *GetOwner()->GetName())
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Print out the FRotation and the Yaw
	// UE_LOG(LogTemp, Warning, TEXT("Door Rotation is %s; Yaw is %f"), *GetOwner()->GetActorRotation().ToString(), GetOwner()->GetActorRotation().Yaw);

	if (TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if ((CurrentTime - DoorLastOpened) >= DoorCloseDelay)
		{
			CloseDoor();
		}
	}

}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// My solution
	//FRotator CurrentRotation = GetOwner()->GetActorRotation();
	//CurrentRotation.Yaw = FMath::Lerp(GetOwner()->GetActorRotation().Yaw, TargetYaw, 0.01f);
	//GetOwner()->SetActorRotation(CurrentRotation);

	// actual solution for absolute method
	//float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	//FRotator OpenDoor(0.f, TargetYaw, 0.f);
	////OpenDoor.Yaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.002f); // this is frame rate dependent on not good
	////OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, 30); // this will not be frame rate dependent and the last parameter is the degrees per second you want to move, but it is linear
	//OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2); // this is what we actually want
	//GetOwner()->SetActorRotation(OpenDoor);

	// Dynamic method so it will be relative to door's initial position
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	bCloseDoorSound = false;

	// play sound
	if (DoorOpenSound == nullptr) { return; }
	if (!bOpenDoorSound)
	{
		DoorOpenSound->Play();
		bOpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor()
{
	FRotator ClosedRotation = GetOwner()->GetActorRotation();
	ClosedRotation.Yaw = InitialYaw;
	GetOwner()->SetActorRotation(ClosedRotation);
	CurrentYaw = 0;

	bOpenDoorSound = false;

	// play sound
	if (DoorOpenSound == nullptr) { return; }
	if (!bCloseDoorSound)
	{
		DoorOpenSound->Play();
		bCloseDoorSound = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	if (PressurePlate == nullptr) { return TotalMass; }

	// Find all overlapping Actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	// Add up their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}