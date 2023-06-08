#include "JoinList.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UJoinList::JoinListSetup(FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		// TODO: initialize callbacks
	}
}

void UJoinList::OnJoinClicked(FString SessionToken)
{
	// TODO

}

bool UJoinList::Initialize() 
{
	if (!Super::Initialize()) 
	{
		return false;
	}

	if (ValidateButton)
	{
		ValidateButton->OnClicked.AddDynamic(this, &ThisClass::ValidateButtonClicked);
	}
	if (SearchButton) 
	{
		SearchButton->OnClicked.AddDynamic(this, &ThisClass::SearchButtonClicked);
	}

	return true;
}

void UJoinList::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	Sessions = {};
	//ServerList->ClearListItems();

	for (FOnlineSessionSearchResult Result : SessionResults)
	{
		FString SessionToken;
		Result.Session.SessionSettings.Get(FName("SessionToken"), SessionToken);
		Sessions.Add(SessionToken, Result);
		SetSessionEvent(SessionToken);
	}
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		ValidateButton->SetIsEnabled(true);
		ValidateButton->SetVisibility(ESlateVisibility::Hidden);
		CodePromptBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UJoinList::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UJoinList::OnDestroySession(bool bWasSuccessful)
{
}

void UJoinList::OnStartSession(bool bWasSuccessful)
{
}

void UJoinList::ValidateButtonClicked()
{
	
}

void UJoinList::SearchButtonClicked()
{
	ValidateButton->SetIsEnabled(false);
	FString PromptSessionToken = CodePromptBox->GetText().ToString();
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(100, PromptSessionToken);
	}
}

void UJoinList::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UJoinList::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}