// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "FactoryGame.h"
#include "CoreMinimal.h"
#include "Unlocks/FGUnlock.h"
#include "FGUnlockCustomizer.generated.h"

/**
 * Unlocks the in game customizer for the build gun
 */
UCLASS( Blueprintable, EditInlineNew, abstract, DefaultToInstanced )
class FACTORYGAME_API UFGUnlockCustomizer : public UFGUnlock
{
	GENERATED_BODY()

public:
	// Begin FGUnlock interface
	virtual void Apply( class AFGUnlockSubsystem* unlockSubsystem ) override;
	// End FGUnlock interface
	
};
