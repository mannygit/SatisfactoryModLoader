// This file has been automatically generated by the Unreal Header Implementation tool

#include "Buildables/FGBuildableConveyorBase.h"
#include "Components/SceneComponent.h"
#include "FGFactoryConnectionComponent.h"

bool GIsConveyorFreezingEnabled = false;

void UFGConveyorRemoteCallObject::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFGConveyorRemoteCallObject, mForceNetField_UFGConveyorRemoteCallObject);
}
void UFGConveyorRemoteCallObject::Server_OnUse_Implementation( AFGBuildableConveyorBelt* target,  AFGCharacterPlayer* byCharacter, uint32 itemRepID, float itemOffset){ }
bool UFGConveyorRemoteCallObject::Server_OnUse_Validate( AFGBuildableConveyorBelt* target,  AFGCharacterPlayer* byCharacter, uint32 itemRepID, float itemOffset){ return bool(); }
void UFGConveyorRemoteCallObject::Server_ReportInvalidStateAndRequestConveyorRepReset_Implementation( AFGBuildableConveyorBase* target){ }
bool UFGConveyorRemoteCallObject::Server_ReportInvalidStateAndRequestConveyorRepReset_Validate( AFGBuildableConveyorBase* target){ return bool(); }
FConveyorBeltItems::FConveyorBeltItems(){ }
int32 FConveyorBeltItems::GetIndexForItemByRepKey(FG_ConveyorItemRepKeyType itemID) const{ return int32(); }
int32 FConveyorBeltItems::GetIndexForItemByRepKeyNetBitLimit(uint32 itemID) const{ return int32(); }
bool FConveyorBeltItems::NetDeltaSerialize(FNetDeltaSerializeInfo& parms){ return bool(); }
void FConveyorBeltItems::ApplySpacingToItem(int32 targetIndex, int32 nbReadsDone, EConveyorSpawnStyle spacingStyle, int16 spacingParamA, int16 spacingParamB, uint32 individualSpacingParam){ }
void FConveyorBeltItems::SignalReadErrorAndRequestReset(FBitReader& reader){ }
void FConveyorBeltItems::CleanupAndSignalVersionChange(){ }
void FConveyorBeltItems::MarkArrayDirty(){ }
void FConveyorBeltItems::UpdateLastestIDFromState(){ }
float FConveyorBeltItems::ConsumeAndUpdateConveyorOffsetDebt(float dt){ return float(); }
void FConveyorBeltItems::MarkItemDirty(FConveyorBeltItem& item){ }
#if !UE_BUILD_SHIPPING
void AFGBuildableConveyorBase::DebugDrawStalled() const{ }
#endif 
#if UE_BUILD_SHIPPING
#endif 
AFGBuildableConveyorBase::AFGBuildableConveyorBase() : Super() {
	this->mSpeed = 0.0;
	this->mConnection0 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("ConveyorAny0"));
	this->mConnection1 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("ConveyorAny1"));
	this->mNextConveyor = nullptr;
	this->mConveyorChainFlags = 0;
	this->mFactoryTickFunction.TickGroup = ETickingGroup::TG_PrePhysics;
	this->mFactoryTickFunction.EndTickGroup = ETickingGroup::TG_PrePhysics;
	this->mFactoryTickFunction.bTickEvenWhenPaused = false;
	this->mFactoryTickFunction.bCanEverTick = true;
	this->mFactoryTickFunction.bStartWithTickEnabled = true;
	this->mFactoryTickFunction.bAllowTickOnDedicatedServer = true;
	this->mFactoryTickFunction.TickInterval = 0.0;
	this->NetDormancy = ENetDormancy::DORM_Awake;
	this->mConnection0->SetupAttachment(RootComponent);
	this->mConnection1->SetupAttachment(RootComponent);
}
void AFGBuildableConveyorBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFGBuildableConveyorBase, mItems);
}
void AFGBuildableConveyorBase::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker){ }
void AFGBuildableConveyorBase::BeginPlay(){ }
void AFGBuildableConveyorBase::EndPlay(const EEndPlayReason::Type endPlayReason){ }
void AFGBuildableConveyorBase::Serialize(FArchive& ar){ Super::Serialize(ar); }
void AFGBuildableConveyorBase::Tick(float dt){ }
void AFGBuildableConveyorBase::PostLoadGame_Implementation(int32 saveVersion, int32 gameVersion){ }
bool AFGBuildableConveyorBase::ShouldBeConsideredForBase_Implementation(){ return bool(); }
void AFGBuildableConveyorBase::Factory_Tick(float deltaTime){ }
uint8 AFGBuildableConveyorBase::MaxNumGrab(float dt) const{ return uint8(); }
uint8 AFGBuildableConveyorBase::EstimatedMaxNumGrab_Threadsafe(float estimatedDeltaTime) const{ return uint8(); }
void AFGBuildableConveyorBase::GainedSignificance_Implementation(){ }
void AFGBuildableConveyorBase::LostSignificance_Implementation(){ }
void AFGBuildableConveyorBase::GainedSignificance_Native(){ }
void AFGBuildableConveyorBase::LostSignificance_Native(){ }
void AFGBuildableConveyorBase::SetupForSignificance(){ }
void AFGBuildableConveyorBase::BuildStaticItemInstances(){ }
void AFGBuildableConveyorBase::DestroyStaticItemInstancesNextFrame(){ }
void AFGBuildableConveyorBase::ReportInvalidStateAndRequestConveyorRepReset(){ }
bool AFGBuildableConveyorBase::Factory_PeekOutput_Implementation(const  UFGFactoryConnectionComponent* connection, TArray< FInventoryItem >& out_items, TSubclassOf< UFGItemDescriptor > type) const{ return bool(); }
bool AFGBuildableConveyorBase::Factory_GrabOutput_Implementation( UFGFactoryConnectionComponent* connection, FInventoryItem& out_item, float& out_OffsetBeyond, TSubclassOf< UFGItemDescriptor > type){ return bool(); }
bool AFGBuildableConveyorBase::Factory_QuickPullConveyorToConveyor(FConveyorBeltItem& out_item, float availableSpace, float deltaTime){ return bool(); }
void AFGBuildableConveyorBase::GetDismantleInventoryReturns(TArray< FInventoryStack >& out_returns) const{ }
int32 AFGBuildableConveyorBase::FindItemClosestToLocation(const FVector& location) const{ return int32(); }
void AFGBuildableConveyorBase::Factory_RemoveItemAt(int32 index){ }
void AFGBuildableConveyorBase::DestroyStaticItemInstances(){ }
void AFGBuildableConveyorBase::Factory_DequeueItem(){ }
void AFGBuildableConveyorBase::Factory_EnqueueItem(const FInventoryItem& item, float initialOffset){ }
