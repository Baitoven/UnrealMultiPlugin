#pragma once

#include "CoreMinimal.h"
#include "JoinListEntry.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "JoinList.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API UJoinList : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void JoinListSetup(FString LobbyPath = FString(TEXT("/Game/Maps/Lobby")));

protected:

	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* ValidateButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SearchButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* CodePromptBox;

	UPROPERTY(meta = (BindWidget))
	UListView* ServerList;

	UFUNCTION()
	void ValidateButtonClicked();

	UFUNCTION()
	void SearchButtonClicked();

	void MenuTearDown();

	void AddEntry(FOnlineSessionSearchResult Session);

	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	FString SessionToken;
	FString PathToLobby{ TEXT("") };

	bool bIsHosting;
};
