// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "FactoryGame.h"
#include "FGSubsystem.h"
#include "FGSaveInterface.h"
#include "FGSchematic.h"
#include "ItemAmount.h"
#include "IncludeInBuild.h"
#include "FGSchematicManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FPurchasedSchematicDelegate, TSubclassOf< class UFGSchematic >, purchasedSchematic );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FPaidOffOnSchematicDelegate, AFGSchematicManager*, schematicManager );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnActiveSchematicChanged, TSubclassOf< UFGSchematic >, activeSchematic );
DECLARE_MULTICAST_DELEGATE_OneParam( FOnPopulateSchematicListDelegate, TArray< TSubclassOf< UFGSchematic > >&);

// @todoK2 refactor FPurchasedSchematicDelegate to use this one instead
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FPurchasedSchematicInstigatorDelegate, TSubclassOf< class UFGSchematic >, purchasedSchematic, class AFGCharacterPlayer*, purchaseInstigator );

/** Holds info about a schematic and How much has been paid of on it. */
USTRUCT()
struct FACTORYGAME_API FSchematicCost
{
	GENERATED_BODY()

	UPROPERTY( SaveGame )
	TSubclassOf< UFGSchematic > Schematic;

	/** Amount paid off */
	UPROPERTY( SaveGame, EditDefaultsOnly )
	TArray< FItemAmount > ItemCost;
};

USTRUCT( BlueprintType )
struct FACTORYGAME_API FSchematicSubCategoryData
{
	GENERATED_BODY()

	FSchematicSubCategoryData( TSubclassOf< class UFGSchematicCategory > schematicSubCategory ) :
	SchematicSubCategory( schematicSubCategory )
	{}
	FSchematicSubCategoryData() :
		SchematicSubCategory( nullptr )
	{}

	UPROPERTY( Transient, BlueprintReadOnly )
	TSubclassOf< class UFGSchematicCategory > SchematicSubCategory;

	UPROPERTY( Transient, BlueprintReadOnly )
	TArray< TSubclassOf< UFGSchematic > > Schematics;
	
};

USTRUCT( BlueprintType )
struct FACTORYGAME_API FSchematicCategoryData
{
	GENERATED_BODY()

	FSchematicCategoryData( TSubclassOf<UFGSchematicCategory> schematicCategory ) :
		SchematicCategory( schematicCategory )
	{}
	FSchematicCategoryData() :
		SchematicCategory( nullptr )
	{}

	UPROPERTY( Transient, BlueprintReadOnly )
	TSubclassOf< class UFGSchematicCategory > SchematicCategory;

	UPROPERTY( Transient, BlueprintReadOnly )
	TArray<FSchematicSubCategoryData> SchematicSubCategoryData;
	
};

/**
 * Keeps track of everything regarding schematics
 */
UCLASS( Blueprintable, abstract, HideCategories=("Actor Tick",Rendering,Replication,Input,Actor) )
class FACTORYGAME_API AFGSchematicManager : public AFGSubsystem, public IFGSaveInterface
{
	GENERATED_BODY()
public:
	/** Called when we players are granted a schematic. */
	UPROPERTY( BlueprintAssignable, Category = "Events|Schematics", DisplayName = "OnPurchasedSchematic" )
	FPurchasedSchematicDelegate PurchasedSchematicDelegate;

	/** Called when a payment on a schematic occurs */
	UPROPERTY( BlueprintAssignable, Category = "Events|Schematics", DisplayName = "OnPaidOffOnSchematic" )
	FPaidOffOnSchematicDelegate PaidOffOnSchematicDelegate;

	/** Called when a player is granted a schematic and which player instigated the purchase */
	UPROPERTY( BlueprintAssignable, Category = "Events|Schematics", DisplayName = "OnPurchasedSchematic" )
	FPurchasedSchematicInstigatorDelegate PurchasedSchematicInstigatorDelegate;

	/** Called right after PopulateSchematicsList populated mAllSchematics with the vanilla content */
	FOnPopulateSchematicListDelegate PopulateSchematicListDelegate;
public:
	/** Get the schematic manager, this should always return something unless you call it really early. */
	static AFGSchematicManager* Get( UWorld* world );

	/** Get the schematic manager from a world context, this should always return something unless you call it really early. */
	UFUNCTION( BlueprintPure, Category = "Schematic", DisplayName = "GetSchematicManager", Meta = ( DefaultToSelf = "worldContext" ) )
	static AFGSchematicManager* Get( UObject* worldContext );

	/** Get the tech tier of a not loaded schematic asset */
	static int32 GetTechTier( const FAssetData& schematicAsset );

	/** Get if a asset should be included in build of a not loaded schematic asset */
	static EIncludeInBuilds GetIncludedInBuild( const FAssetData& schematicAsset );
public:
	AFGSchematicManager();

	//~ Begin AActor interface
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type endPlayReason ) override;
	virtual void Tick( float dt ) override;
	//~ End AActor interface

	// Begin IFGSaveInterface
	virtual void PreSaveGame_Implementation( int32 saveVersion, int32 gameVersion ) override;
	virtual void PostSaveGame_Implementation( int32 saveVersion, int32 gameVersion ) override;
	virtual void PreLoadGame_Implementation( int32 saveVersion, int32 gameVersion ) override;
	virtual void PostLoadGame_Implementation( int32 saveVersion, int32 gameVersion ) override;
	virtual void GatherDependencies_Implementation( TArray< UObject* >& out_dependentObjects ) override;
	virtual bool NeedTransform_Implementation() override;
	virtual bool ShouldSave_Implementation() const override;
	// End IFSaveInterface

	/** Returns the available schematics in the game that have meet their dependencies. */
	UFUNCTION( BlueprintCallable, BlueprintPure = false, Category = "Schematic" )
	void GetAvailableSchematics( TArray< TSubclassOf< UFGSchematic > >& out_schematics ) const;

	/** Returns the available schematics in the game of the given types that have meet their dependencies. */
	UFUNCTION( BlueprintCallable, BlueprintPure = false, Category = "Schematic" )
	void GetAvailableSchematicsOfTypes( TArray<ESchematicType> types, TArray< TSubclassOf< UFGSchematic > >& out_schematics ) const;

	/** Returns the schematics the players have purchased of the given types. */
	UFUNCTION( BlueprintCallable, BlueprintPure = false, Category = "Schematic" )
	void GetPurchasedSchematicsOfTypes( TArray<ESchematicType> types, TArray< TSubclassOf< UFGSchematic > >& out_schematics ) const;

	/** Returns all schematics the players have purchased. */
	UFUNCTION( BlueprintCallable, BlueprintPure = false, Category = "Schematic" )
	void GetAllPurchasedSchematics( TArray< TSubclassOf< UFGSchematic > >& out_schematics ) const;

	/** Returns every possible schematic that is in the game. */
	UFUNCTION( BlueprintCallable, BlueprintPure = false, Category = "Schematic" )
	void GetAllSchematics(TArray< TSubclassOf< UFGSchematic > >& out_schematics ) const;

	/** Returns all schematics of a type. */
	UFUNCTION( BlueprintCallable, BlueprintPure = false, Category = "Schematic" )
	void GetAllSchematicsOfType( ESchematicType type, TArray< TSubclassOf< UFGSchematic > >& out_schematics ) const;

	/** Returns all schematics of a type that have any of their dependencies met. */
	UFUNCTION( BlueprintCallable, BlueprintPure = false, Category = "Schematic" )
	void GetAllSchematicsOfTypeFilteredOnDependency( ESchematicType type, TArray< TSubclassOf< UFGSchematic > >& out_schematics ) const;

	/** Returns all schematics of a type that is not in hte hidden state. */
	UFUNCTION( BlueprintCallable, BlueprintPure = false, Category = "Schematic" )
	void GetAllVisibleSchematicsOfType( ESchematicType type, TArray< TSubclassOf< UFGSchematic > >& out_schematics ) const;

	/** returns true if the passed schematic has been purchased,
	 * @param owningPlayerController is only needed if checking for a player specific schematic so defaults to nullptr.
	 * @ Note To check for a player-specific schematic, ensure the function is called with the correct player controller.
	 * If this function is called from a widget, use GetOwningPlayer() to obtain the local player controller, as it will refer to the player who has the widget opened.
	 * */
	UFUNCTION( BlueprintCallable, Category = "Schematic" )
	bool IsSchematicPurchased( TSubclassOf< UFGSchematic > schematicClass, APlayerController* owningPlayerController = nullptr ) const;

	/** Give the player access to a schematic. accessInstigator can be nullptr. */
	UFUNCTION( BlueprintCallable, Category = "Schematic" )
	void GiveAccessToSchematic( TSubclassOf< UFGSchematic > schematicClass, class AFGCharacterPlayer* accessInstigator, bool blockTelemetry = false );

	/** adds a schematic to available schematics */
	UFUNCTION( BlueprintCallable, Category = "Schematic" )
	void AddAvailableSchematic( TSubclassOf< UFGSchematic > schematicClassToAdd );
	
	/** Gives you the base cost, after random, for a schematic */
	UFUNCTION( BlueprintPure, DisplayName = "GetCostFor_Deprecated", Category = "Schematic", meta = ( DeprecatedFunction, DeprecationMessage = "Get the cost from the Schematic directly" ) )
	TArray< FItemAmount > GetCostFor( TSubclassOf< UFGSchematic > schematic );

	/** Returns remaining cost for a certain schematic */
	UFUNCTION( BlueprintPure, Category = "Schematic" )
	TArray< FItemAmount > GetRemainingCostFor( TSubclassOf< UFGSchematic > schematic );

	/** Returns how much the players has paid off on a schematic */
	UFUNCTION( BlueprintPure, Category = "Schematic" )
	TArray< FItemAmount > GetPaidOffCostFor( TSubclassOf< UFGSchematic > schematic );

	/** Returns how much the players has paid off on a schematic */
	UFUNCTION( BlueprintPure, Category = "Schematic" )
	bool IsSchematicPaidOff( TSubclassOf< UFGSchematic > schematic );

	/** Adds paid off amount on a schematic */
	UFUNCTION( BlueprintCallable, Category = "Schematic" )
	bool PayOffOnSchematic( TSubclassOf< UFGSchematic > schematic, UPARAM( ref ) TArray< FItemAmount >& amount );

	/** Sets the new active schematic to sell stuff towards */
	UFUNCTION( BlueprintCallable, Category = "Schematic" )
	bool SetActiveSchematic( TSubclassOf< class UFGSchematic > newActiveSchematic );

	/** Can we set the passed schematic as the active one? */
	UFUNCTION( BlueprintPure, Category = "Schematic" )
	bool CanSetAsActiveSchematic( TSubclassOf< UFGSchematic > inSchematic );

	/** Get active Schematic. */
	UFUNCTION( BlueprintPure, Category = "Schematic" )
	FORCEINLINE TSubclassOf< UFGSchematic > GetActiveSchematic() { return mActiveSchematic; }

	/** Player initiated launch of the ship */
	UFUNCTION( BlueprintCallable, Category = "Ship" )
	void LaunchShip();

	/** Returns true if the ship is at the trading post */
	UFUNCTION( BlueprintPure, Category = "Ship" )
	bool IsShipAtTradingPost();

	/** Returns the time until the ship is back */
	UFUNCTION( BlueprintPure, Category = "Ship" )
	float GetTimeUntilShipReturn();

	UFUNCTION( BlueprintCallable, Category = "Schematic" )
	int32 GetHighestAvailableTechTier();

	/** Get the default tech tier from the default schematics manager */
	static int32 GetDefaultMaxAllowedTechTier();

	UFUNCTION(BlueprintPure, Category = "Schematic")
	int32 GetMaxAllowedTechTier() const;

	/** Resets schematics to their defaults. Used for resetting specific progressions for testing. */
	void ResetSchematicsOfType( ESchematicType type );

	/** Used to fix up old saves that are missing the built with recipe. */
	TSubclassOf< class UFGRecipe > FixupSave_FindBuiltByRecipe( AActor* forActor );

	/** Debug stuff */
	void Debug_DumpStateToLog() const;
	TArray< TSubclassOf< class UFGRecipe > > Debug_GetAllRecipes() const;

	void Cheat_GiveAllSchematics();

	/** Checks if it's valid to give access to the given schematic */
	bool CanGiveAccessToSchematic( TSubclassOf< UFGSchematic > schematic ) const;

	/** Returns a sorted struct of all schematic data for a certain schematic type */
	UFUNCTION( BlueprintCallable, Category = "Organization" )
	void GetVisibleSchematicCategoryData( ESchematicType schematicType, TArray< FSchematicCategoryData >& out_schematicCategoryData );

	/** Unlocks all schematics of the given type */
	void UnlockAllSchematicsOfType( ESchematicType schematicType, bool requireDependency = false );
	/** Unlocks all schematics up to (not including) the given tier */ 
	void UnlockSchematicsUpToTier( int32 tier );
	
private:
	/** Populate list with all schematics */
	void PopulateSchematicsLists();

	/** Populate list with the default available schematics. */
	void PopulateAvailableSchematicsList();

	UFUNCTION()
	void OnRep_ActiveSchematic();
	UFUNCTION()
	void OnRep_PurchasedSchematic( TArray< TSubclassOf< UFGSchematic > > lastPurchasedSchematics );
	UFUNCTION()
	void OnRep_PaidOffOnSchematic();

	/** Returns the highest tech tier available in lists */
	int32 FindHighestAvailableTechTier();

	/** Find or remove a given schematic in the pay off schematic list. */
	FSchematicCost* FindSchematicPayOff( TSubclassOf< class UFGSchematic > schematic );
	void AddSchematicPayOff( TSubclassOf< class UFGSchematic > schematic, const TArray< FItemAmount >& amount );
	void RemoveSchematicPayOff( TSubclassOf< class UFGSchematic > schematic );

	/** Telemetry helper. */
	void SubmitUnlockSchematicTelemetry( TSubclassOf< UFGSchematic > schematicClass );

	UFUNCTION()
	void SubmitMilestoneTelemetry( TSubclassOf< UFGSchematic > activeSchematic );
	
	/** Tries to find if there are any new schematics of the given type that can now be unlocked
	  * Used by game modes when we want to unlock all new alt recipes or shop schematics when their dependency schematics are unlocked
	  * @param userSettingStrID we check if the setting connected to this id is true before trying to unlock new schematics of the given type
	  * @param timerHandle the timer handle to check if we are already have a pending request. Will be assigned a handle if we start a new timer
	  * @param schematicType the schematics type we try to unlock more schematics of
	 */ 
	void TryUnlockNewSchematicsOfType( const FString& userSettingStrID, FTimerHandle& timerHandle, ESchematicType schematicType );
	/** Called by TryUnlockNewSchematicsOfType when time is  */
	void Internal_TryUnlockNewSchematicsOfType( ESchematicType schematicType );


protected:	
	/** All schematic assets that have been sucked up in the PopulateSchematicsList function. Contains cheats and all sort of schematic. */
	UPROPERTY()
	TArray< TSubclassOf< UFGSchematic > > mAllSchematics;

	/** All schematics that are available to the player */
	UPROPERTY( SaveGame, Replicated )
	TArray< TSubclassOf< UFGSchematic > > mAvailableSchematics;

	/** Once schematic is purchased it ends up here */
	UPROPERTY( EditDefaultsOnly, SaveGame, ReplicatedUsing = OnRep_PurchasedSchematic, Category = "Schematic" )
	TArray< TSubclassOf< UFGSchematic > > mPurchasedSchematics;

	/* This keeps track of what players have paid off on different schematics */
	UPROPERTY( SaveGame, ReplicatedUsing = OnRep_PaidOffOnSchematic )
	TArray< FSchematicCost > mPaidOffSchematic;
	
	/** The active schematic the resources is being sold towards. */
	UPROPERTY( SaveGame, ReplicatedUsing = OnRep_ActiveSchematic )
	TSubclassOf< UFGSchematic > mActiveSchematic;

	/** Called when we the schematic has been changed . */
	UPROPERTY( BlueprintAssignable, Category = "Schematics" )
	FOnActiveSchematicChanged mOnActiveSchematicChanged;
	
	/* Time stamp for when the ship is gonna land back at the Trading Post. */
	UPROPERTY( Replicated )
	float mShipLandTimeStamp;

	/** Used to save the ship land timestamp */
	UPROPERTY( SaveGame )
	float mShipLandTimeStampSave;
	
	FTimerHandle mTryUnlockInstantAltRecipeHandle;
	FTimerHandle mTryUnlockNewResourceSinkSchematicsHandle;
	
	UPROPERTY( EditDefaultsOnly, Category = "Schematic" )
	bool mHasTechTierLimit;

	UPROPERTY( EditDefaultsOnly, Category = "Schematic", meta = ( EditCondition = "mHasTechTierLimit" , ClampMin = "0", ClampMax = "99999", UIMin = "0", UIMax = "99999" ) )
	int32 mMaxAllowedTechTier = 6;

	/** Internal bool to keep track of the state of the ship  */
	bool mIsShipAtTradingPost; 

	/** Message sent when trading post ship has returned */
	UPROPERTY( EditDefaultsOnly, Category = "Message" )
	TSubclassOf< class UFGMessageBase > mShipReturnedMessage;
};
