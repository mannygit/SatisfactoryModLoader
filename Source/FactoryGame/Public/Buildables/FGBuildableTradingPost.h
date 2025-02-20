// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "FactoryGame.h"
#include "Buildables/FGBuildableFactory.h"
#include "Buildables/FGBuildableGenerator.h"
#include "FGActorRepresentationInterface.h"
#include "FGBuildableTradingPost.generated.h"

UINTERFACE( Blueprintable )
class FACTORYGAME_API UFGTradingPostIntegratedBuildable : public UInterface
{
	GENERATED_BODY()
};

class FACTORYGAME_API IFGTradingPostIntegratedBuildable
{
	GENERATED_BODY()
public:
	/** Called when the buildable visibility changed */
	UFUNCTION( BlueprintNativeEvent, Category = "Trading Post" )
	void UpdateBuildableVisibility( bool isVisible, bool isTutorialComplete );
}; 

/**
 * The trading post, it has N inputs and sells inputed items for money.
 */
UCLASS( Abstract )
class FACTORYGAME_API AFGBuildableTradingPost : public AFGBuildableFactory, public IFGActorRepresentationInterface
{
	GENERATED_BODY()
public:
	AFGBuildableTradingPost();

	// Begin AActor interface
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	virtual void BeginPlay() override;
	// End AActor interface

	//~ Begin AFGBuildable interface
	virtual bool CanBeSampled_Implementation() const{ return false; }
	//~ End AFGBuildable interface

	virtual void PostLoadGame_Implementation( int32 saveVersion, int32 gameVersion ) override;

	//~ Begin IFGDismantleInterface
	virtual void Dismantle_Implementation() override;
	virtual void GetDismantleRefund_Implementation( TArray< FInventoryStack >& out_refund, bool noBuildCostEnabled ) const override;
	virtual void StartIsLookedAtForDismantle_Implementation( class AFGCharacterPlayer* byCharacter ) override;
	virtual void StopIsLookedAtForDismantle_Implementation( class AFGCharacterPlayer* byCharacter ) override;
	virtual void GetChildDismantleActors_Implementation( TArray< AActor* >& out_ChildDismantleActors ) const override;
	//~ End IFGDismantleInferface

	// Begin IFGActorRepresentationInterface
	UFUNCTION()
	virtual bool AddAsRepresentation() override;
	UFUNCTION()
	virtual bool UpdateRepresentation() override;
	UFUNCTION()
	virtual bool RemoveAsRepresentation() override;
	UFUNCTION()
	virtual bool IsActorStatic() override;
	UFUNCTION()
	virtual FVector GetRealActorLocation() override;
	UFUNCTION()
	virtual FRotator GetRealActorRotation() override;
	UFUNCTION()
	virtual class UTexture2D* GetActorRepresentationTexture() override;
	UFUNCTION()
	virtual FText GetActorRepresentationText() override;
	UFUNCTION()
	virtual void SetActorRepresentationText( const FText& newText ) override;
	UFUNCTION()
	virtual FLinearColor GetActorRepresentationColor() override;
	UFUNCTION()
	virtual void SetActorRepresentationColor( FLinearColor newColor ) override;
	UFUNCTION()
	virtual ERepresentationType GetActorRepresentationType() override;
	UFUNCTION()
	virtual bool GetActorShouldShowInCompass() override;
	UFUNCTION()
	virtual bool GetActorShouldShowOnMap() override;
	UFUNCTION()
	virtual EFogOfWarRevealType GetActorFogOfWarRevealType() override;
	UFUNCTION()
	virtual float GetActorFogOfWarRevealRadius() override;
	UFUNCTION()
	virtual ECompassViewDistance GetActorCompassViewDistance() override;
	UFUNCTION()
	virtual void SetActorCompassViewDistance( ECompassViewDistance compassViewDistance ) override;
	// End IFGActorRepresentationInterface

	/** Upgrading the trading post to specified level */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "TradingPost" )
	void OnTradingPostUpgraded( int32 level, bool suppressBuildEffects = false );

	/** Handles the generators visibility depending on tutorial step */
	UFUNCTION( BlueprintCallable, Category = "Trading Post" )
	void UpdateGeneratorVisibility();

	/** Handles the storage visibility depending on tutorial step */
	UFUNCTION( BlueprintCallable, Category = "Trading Post" )
	void UpdateStorageVisibility();

	/** Updates the visibility of the terminal and work bench, depending on the current upgrade level of the hub */
	UFUNCTION( BlueprintCallable, Category = "Trading Post" )
	void UpdateTerminalAndWorkbenchVisibility();

	/** Returns level of trading post upgrade */
	UFUNCTION( BlueprintPure, Category = "Trading Post" )
	int32 GetTradingPostLevel() const;

	/** Make sure to call the build effect on all visible buildings */
	virtual void PlayBuildEffects( AActor* inInstigator ) override;
	virtual void ExecutePlayBuildEffects() override;

	void PlayBuildEffectsOnAllClients( AActor* instigator = nullptr );

	/** Checks if all child buildings are created so that we can use them */
	UFUNCTION( BlueprintPure, Category = "Trading Post" )
	bool AreChildBuildingsLoaded();

	/** Called by hologram when finishing constructing child buildings to make sure they're setup */
	void ValidateSubBuildings();

	TArray<AActor*> GetAllActiveSubBuildings() const;

	/** Fetches the color to use for this actors representation */
	UFUNCTION( BlueprintImplementableEvent, Category = "Representation" )
	FLinearColor GetDefaultRepresentationColor();

protected:
	virtual void OnBuildEffectFinished() override;
	virtual void TogglePendingDismantleMaterial( bool enabled ) override;

	/** HAXX: For those that has sampled subbuildings, we tag them with a actor tag so that we know that they are not samplable */
	UFUNCTION()
	void OnRep_HAXX_SubbuildingReplicated();
	
	/** Adjust all child actor components that spawning a AFGPlayerStartTradingPost to the ground */
	void AdjustPlayerSpawnsToGround();

	/** Returns the cached schematic manager */
	class AFGSchematicManager* GetSchematicManager();

private:
	UFUNCTION()
	void OnRep_NeedPlayingBuildEffect();

	/** Try to add event sub buildings like calendar and mini game */
	UFUNCTION()
	void TryAddEventSubBuildings();

public:
	/** Class of generators to create with the trading post */
	UPROPERTY( EditDefaultsOnly, Category = "Trading Post" )
	TSubclassOf< class UFGRecipe > mDefaultGeneratorRecipe;

	/** References to the created generators */
	UPROPERTY( ReplicatedUsing=OnRep_HAXX_SubbuildingReplicated, SaveGame )
	TArray< AFGBuildableGenerator* > mGenerators;

	//@todo Why are these recipes and not only TSubclassOf<AFGBuildable>? This seems like a lot of setup just for the extra buildables? Hologram can be created for buildables as well and not just recipes!
	/** Class of storage to create with the trading post */
	UPROPERTY( EditDefaultsOnly, Category = "Trading Post" )
	TSubclassOf< class UFGRecipe > mDefaultStorageRecipe;

	/** Class of hub terminal to create with the trading post */
	UPROPERTY( EditDefaultsOnly, Category = "Trading Post" )
	TSubclassOf< class UFGRecipe > mDefaultHubTerminalRecipe;

	/** Class of work bench to create with the trading post */
	UPROPERTY( EditDefaultsOnly, Category = "Trading Post" )
	TSubclassOf< class UFGRecipe > mDefaultWorkBenchRecipe;

	/** References to the created storage */
	UPROPERTY( ReplicatedUsing=OnRep_HAXX_SubbuildingReplicated, SaveGame )
	class AFGBuildable* mStorage;

	/** References to the created Hub Terminal */
	UPROPERTY( ReplicatedUsing=OnRep_HAXX_SubbuildingReplicated, SaveGame )
	class AFGBuildableHubTerminal* mHubTerminal;

	/** References to the created work bench */
	UPROPERTY( ReplicatedUsing=OnRep_HAXX_SubbuildingReplicated, SaveGame )
	class AFGBuildable* mWorkBench;

	/** References to the created calendar */
	UPROPERTY( Replicated )
	class AFGBuildableCalendar* mCalendar;

	/** References to the created mini game */
	UPROPERTY( Replicated )
	class AFGBuildable* mMiniGame;

	/** Arrays containing ints for what level  we should activate/show the generator */
	UPROPERTY( EditDefaultsOnly, Category = "Trading Post", Meta = (NoAutoJson = true) )
	TArray< int32 > mGeneratorVisibilityLevels;

	/** Size of the storage box */
	UPROPERTY( EditDefaultsOnly, Category = "Trading Post" )
	int32 mStorageInventorySize;

	/** At what trading post level should the storage be visible */
	UPROPERTY( EditDefaultsOnly, Category = "Trading Post" )
	int32 mStorageVisibilityLevel;

protected:
	/** Our input inventory where items are stored before put on ship */
	UPROPERTY( SaveGame, Replicated )
	class UFGInventoryComponent* mInputInventory;	

	/** How far above the ground the spawn-points should be adjusted */
	UPROPERTY( EditDefaultsOnly, Category="Spawning" )
	float mSpawningGroundZOffset;

	/** How much up and down we should search for the ground around all child actors that's spawning AFGPlayerStartTradingPost */
	UPROPERTY( EditDefaultsOnly, Category="Spawning" )
	float mGroundSearchZDistance;

	/** A cached schematic manager */
	UPROPERTY()
	class AFGSchematicManager* mSchematicManager;

	/** The starting resources in the tradingpost storage */
	UPROPERTY( EditDefaultsOnly, Category = "Inventory" )
	TArray< FItemAmount > mDefaultResources;

	/** Component used to determine generators location */
	UPROPERTY( EditAnywhere )
	USceneComponent* mGenerator1Location;

	/** Component used to determine generators location */
	UPROPERTY( EditAnywhere )
	USceneComponent* mGenerator2Location;

	/** Component used to determine storage location */
	UPROPERTY( EditAnywhere )
	USceneComponent* mStorageLocation;

	/** Component used to determine Hub terminal location */
	UPROPERTY( EditAnywhere )
	USceneComponent* mHubTerminalLocation;

	/** Component used to determine work bench terminal location */
	UPROPERTY( EditAnywhere )
	USceneComponent* mWorkBenchLocation;

	/** Component used to determine calendar location */
	UPROPERTY( EditAnywhere )
	USceneComponent* mCalendarLocation;

	/** Component used to determine mini game location */
	UPROPERTY( EditAnywhere )
	USceneComponent* mMiniGameLocation;

	/** Bool to sync playing of build and upgrade effects */
	UPROPERTY( ReplicatedUsing = OnRep_NeedPlayingBuildEffect )
	bool mNeedPlayingBuildEffectNotification;

private:
	UPROPERTY( EditDefaultsOnly, Category = "Representation" )
	class UTexture2D* mActorRepresentationTexture;

	UPROPERTY( EditDefaultsOnly, Category = "Representation" )
	FText mRepresentationText;
};
