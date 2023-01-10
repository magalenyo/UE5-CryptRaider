// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent() {
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* actor = GetAcceptableActor();
    if (actor != nullptr) {
        if (mover != nullptr) {
            mover->SetShouldMove(true);
            UPrimitiveComponent* component = Cast<UPrimitiveComponent>(actor->GetRootComponent());
            if (component != nullptr) {
                component->SetSimulatePhysics(false);
            }
            actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

void UTriggerComponent::SetMover(UMover* newMover) {
    mover = newMover;
}

AActor* UTriggerComponent::GetAcceptableActor() const {
    TArray<AActor*> actors;
    GetOverlappingActors(actors);
    for (AActor* actor : actors) {
        if (actor->ActorHasTag(acceptableActorTag) &&
            !actor->ActorHasTag("Grabbed")) {
            return actor;
        }
    }

    return nullptr;
}

