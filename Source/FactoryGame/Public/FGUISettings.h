// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "FactoryGame.h"
#include "Engine/DeveloperSettings.h"
#include "FGUISettings.generated.h"

UCLASS( config = Game, defaultconfig, meta = ( DisplayName = "Satisfactory UI Settings" ) )
class FACTORYGAME_API UFGUISettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	static const UFGUISettings* Get() { return GetDefault<UFGUISettings>(); };
public:
	/** Widget used to show that the user is setting up a server */
	UPROPERTY( EditAnywhere, config, Category = UI )
	TSoftClassPtr<UUserWidget> mSettingUpServerWidget;

	/** Widget used to show the user when joining a session */
	UPROPERTY( EditAnywhere, config, Category = UI )
	TSoftClassPtr<UUserWidget> mJoinSessionWidget;

	/** Widget used to show the user when linking accounts */
	UPROPERTY( EditAnywhere, config, Category = UI )
	TSubclassOf<UUserWidget> mConnectAccountsWidget;

	/** Widget to show the user when unlinking account */
	UPROPERTY( EditAnywhere, config, Category = UI )
	TSubclassOf<UUserWidget> mResetAccountLinkingWidget;

	/** Widget used to show the user when enabling the debug overlay */
	UPROPERTY( EditAnywhere, config, Category = UI )
	TSubclassOf< class UFGDebugOverlayWidget > mDebugOverlayWidgetClass;

	/** Emote wheel widget to select a widget */
	UPROPERTY( EditAnywhere, config, Category = UI )
	TSubclassOf< class UFGInteractWidget > mEmoteMenuWidget;

	/** Widget used to customize and spawn a map marker in the world */
	UPROPERTY( EditAnywhere, config, Category = UI )
	TSubclassOf< class UFGInteractWidget > mAddMapMarkerWidget;

	/** Widget used to hold audio messages that the player receives. This is populated with data from UFGMessage assets */
	UPROPERTY( EditAnywhere, config, Category = Narrative )
	TSoftClassPtr< class UFGAudioMessage > mDefaultAudioMessageWidget;

	
};