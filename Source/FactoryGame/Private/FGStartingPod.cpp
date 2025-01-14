// This file has been automatically generated by the Unreal Header Implementation tool

#include "FGStartingPod.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

AFGStartingPod::AFGStartingPod() : Super() {
	this->mCachedPlayer = nullptr;
	this->mDropPodMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DropPod Mesh"));
	this->mActorRepresentationTexture = nullptr;
	this->mMapText = INVTEXT("");
	this->PrimaryActorTick.TickGroup = ETickingGroup::TG_PrePhysics;
	this->PrimaryActorTick.EndTickGroup = ETickingGroup::TG_PrePhysics;
	this->PrimaryActorTick.bTickEvenWhenPaused = false;
	this->PrimaryActorTick.bCanEverTick = true;
	this->PrimaryActorTick.bStartWithTickEnabled = true;
	this->PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	this->PrimaryActorTick.TickInterval = 0.0;
	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->mDropPodMesh->SetupAttachment(RootComponent);
}
void AFGStartingPod::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFGStartingPod, mCachedPlayer);
	DOREPLIFETIME(AFGStartingPod, mMapText);
}
FVector AFGStartingPod::GetRefundSpawnLocationAndArea_Implementation(const FVector& aimHitLocation, float& out_radius) const{ return FVector(); }
bool AFGStartingPod::CanDismantle_Implementation() const{ return bool(); }
void AFGStartingPod::GetDismantleRefund_Implementation(TArray< FInventoryStack >& out_refund, bool noBuildCostEnabled) const{ }
void AFGStartingPod::PreUpgrade_Implementation(){ }
void AFGStartingPod::Upgrade_Implementation(AActor* newActor){ }
void AFGStartingPod::Dismantle_Implementation(){ }
void AFGStartingPod::StartIsLookedAtForDismantle_Implementation( AFGCharacterPlayer* byCharacter){ }
void AFGStartingPod::StopIsLookedAtForDismantle_Implementation( AFGCharacterPlayer* byCharacter){ }
void AFGStartingPod::GetChildDismantleActors_Implementation(TArray< AActor* >& out_ChildDismantleActors) const{ }
bool AFGStartingPod::AddAsRepresentation(){ return bool(); }
bool AFGStartingPod::UpdateRepresentation(){ return bool(); }
bool AFGStartingPod::RemoveAsRepresentation(){ return bool(); }
bool AFGStartingPod::IsActorStatic(){ return bool(); }
FVector AFGStartingPod::GetRealActorLocation(){ return FVector(); }
FRotator AFGStartingPod::GetRealActorRotation(){ return FRotator(); }
UTexture2D* AFGStartingPod::GetActorRepresentationTexture(){ return nullptr; }
FText AFGStartingPod::GetActorRepresentationText(){ return FText(); }
void AFGStartingPod::SetActorRepresentationText(const FText& newText){ }
FLinearColor AFGStartingPod::GetActorRepresentationColor(){ return FLinearColor(); }
void AFGStartingPod::SetActorRepresentationColor(FLinearColor newColor){ }
ERepresentationType AFGStartingPod::GetActorRepresentationType(){ return ERepresentationType(); }
bool AFGStartingPod::GetActorShouldShowInCompass(){ return bool(); }
bool AFGStartingPod::GetActorShouldShowOnMap(){ return bool(); }
EFogOfWarRevealType AFGStartingPod::GetActorFogOfWarRevealType(){ return EFogOfWarRevealType(); }
float AFGStartingPod::GetActorFogOfWarRevealRadius(){ return float(); }
ECompassViewDistance AFGStartingPod::GetActorCompassViewDistance(){ return ECompassViewDistance(); }
void AFGStartingPod::SetActorCompassViewDistance(ECompassViewDistance compassViewDistance){ }
void AFGStartingPod::BeginPlay(){ }
