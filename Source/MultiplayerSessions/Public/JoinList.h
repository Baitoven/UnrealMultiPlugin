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

	UFUNCTION(BlueprintCallable)
	void OnJoinClicked(FString SessionToken);

	UFUNCTION(BlueprintImplementableEvent)
	void SetSessionEvent(const FString& Entry);

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
	class UButton* SearchButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* CodePromptBox;

	UFUNCTION()
	void SearchButtonClicked();

	void MenuTearDown();

	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	FString PathToLobby{ TEXT("") };
	bool bIsHosting;

	TMap<FString, FOnlineSessionSearchResult> Sessions{};
};
