#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "JoinListEntry.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API UJoinListEntry : public UUserWidget, public IUserObjectListEntry 
{
	GENERATED_BODY()
public:
	void SetSession(FOnlineSessionSearchResult vSession);

protected:

	virtual bool Initialize() override;
	virtual void PostInitProperties() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionTokenText;

	UFUNCTION()
	void JoinButtonClicked();

	UFUNCTION()
	FText SetSessionText();

	FString SessionToken;
	FOnlineSessionSearchResult Session;

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};