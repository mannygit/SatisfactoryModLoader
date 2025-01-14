// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "FactoryGame.h"
#include "CoreMinimal.h"
#include "Unlocks/FGUnlockRecipe.h"
#include "Unlocks/FGUnlock.h"
#include "FGUnlockBlueprints.generated.h"

/**
 * Unlocks the blueprints feature. Derived from unlock recipe since we also want to unlock the blueprint designer recipe with this unlock  
 */
UCLASS( Blueprintable, EditInlineNew, abstract, DefaultToInstanced )
class FACTORYGAME_API UFGUnlockBlueprints : public UFGUnlockRecipe
{
	GENERATED_BODY()

public:
	// Begin FGUnlock interface
	virtual void Apply( class AFGUnlockSubsystem* unlockSubsystem ) override;
	// End FGUnlock interface
	
};
