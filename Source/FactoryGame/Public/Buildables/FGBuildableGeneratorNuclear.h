// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "FactoryGame.h"
#include "CoreMinimal.h"
#include "Buildables/FGBuildableGeneratorFuel.h"
#include "Replication/FGReplicationDetailActor_GeneratorNuclear.h"
#include "FGBuildableGeneratorNuclear.generated.h"


//[FreiholtzK:Tue/15-12-2020] @todo look into making this into a more generic warnign system for all generators if applicable.
UENUM(BlueprintType)
enum class EGeneratorNuclearWarning : uint8
{
	GNW_None,
    GNW_WasteFull,
    GNW_MissmatchBetweenInputAndWaste
};

/**
 * A generator that runs on nuclear fuel and produces waste.
 */
UCLASS()
class FACTORYGAME_API AFGBuildableGeneratorNuclear : public AFGBuildableGeneratorFuel
{
	GENERATED_BODY()
public:
	AFGBuildableGeneratorNuclear();

	// Replication
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	virtual void BeginPlay() override;

protected:
	// Begin Factory_ interface
	virtual void Factory_Tick( float dt ) override;
	// End Factory_ interface

	// Begin FGBuildableFactory interface
	virtual void OnRep_ReplicationDetailActor() override;
	// End FGBuildableFactory interface
	
	// Begin AFGBuildableGenerator interface
	virtual bool CanStartPowerProduction_Implementation() const override;
	// End AFGBuildableGenerator interface

	// Begin AFGBuildableGeneratorFuel interface
	virtual bool CanLoadFuel() const override;
	virtual void LoadFuel() override;
	// End AFGBuildableGeneratorFuel interface

	// Begin IFGReplicationDetailActorOwnerInterface
	virtual UClass* GetReplicationDetailActorClass() const override { return AFGReplicationDetailActor_GeneratorNuclear::StaticClass(); };
	virtual void OnReplicationDetailActorRemoved() override;
	
	class AFGReplicationDetailActor_GeneratorNuclear* GetCastRepDetailsActor() const;
	// End IFGReplicationDetailActorOwnerInterface

	bool IsWasteFull() const;
	
	/** Try to produce nuclear waste and put it in the output inventory */
	void TryProduceWaste();

	/** Returns the inventory for waste in the nuclear generator */
	UFUNCTION( BlueprintPure, Category = "Nuclear" )
	FORCEINLINE class UFGInventoryComponent* GetWasteInventory() const { return mOutputInventoryHandlerData.GetActiveInventoryComponent(); }

	/** Returns the current active warning for this nuclear generator */
	UFUNCTION( BlueprintPure, Category = "Nuclear" )
	FORCEINLINE EGeneratorNuclearWarning GetCurrentGeneratorNuclearWarning() const { return mCurrentGeneratorNuclearWarning; }

	virtual void GetAllReplicationDetailDataMembers(TArray<FReplicationDetailData*>& out_repDetailData) override
	{
		Super::GetAllReplicationDetailDataMembers( out_repDetailData );
		out_repDetailData.Add( &mOutputInventoryHandlerData );
	}
private:
	/** Check if the waste inventory has space for the waste item of the fuel in the fuel inventory. */
	bool CanFitWasteOfNextFuelClass() const;

private:
	friend class AFGReplicationDetailActor_GeneratorNuclear;
	
	/** Spent fuel rods goes here. */
	UPROPERTY( SaveGame )
	class UFGInventoryComponent* mOutputInventory;

	UPROPERTY()
	FReplicationDetailData mOutputInventoryHandlerData;

	/** Waste left to produce from the current fuel rod*/
	UPROPERTY( SaveGame )
	int32 mWasteLeftFromCurrentFuel;

	/** Current active warning on this nuclear generator */
	UPROPERTY( Replicated )
	EGeneratorNuclearWarning mCurrentGeneratorNuclearWarning;
};
