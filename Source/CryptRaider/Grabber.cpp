// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();

	if (physicsHandle && physicsHandle->GetGrabbedComponent() != nullptr) {
		FVector targetLocation = GetComponentLocation() + GetForwardVector().GetSafeNormal() * holdDistance;
		physicsHandle->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab() {

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) return;
 
	FHitResult hitResult;
	bool hasHit = GetGrabbableInReach(hitResult);
 
	if (hasHit) {
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		hitComponent->WakeAllRigidBodies();
		hitResult.GetActor()->Tags.Add("Grabbed");
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			hitResult.GetComponent(),
			NAME_None,
			hitResult.ImpactPoint,
			GetComponentRotation()
		);
 
		DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 10, 10, FColor::Red, false, 3);
	}
}


void UGrabber::Release() {
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();

	if (physicsHandle && physicsHandle->GetGrabbedComponent() != nullptr) {
		physicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		AActor* grabbedActor = physicsHandle->GetGrabbedComponent()->GetOwner();
		grabbedActor->Tags.Remove("Grabbed");
		physicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const {
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

bool UGrabber::GetGrabbableInReach(FHitResult& outHitResult) const {
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * maxGrabDistance;
 
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 3);
 
	FCollisionShape Sphere = FCollisionShape::MakeSphere(grabRadius);
 
	return GetWorld()->SweepSingleByChannel(
		outHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);
}