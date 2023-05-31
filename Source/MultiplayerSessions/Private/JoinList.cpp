#include "JoinList.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UJoinList::JoinListSetup(UObject ListEntryWidget, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	ListEntry = MakeShareable<UObject>(&ListEntryWidget);
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
	// TODO: Add button events
	// TEST
	if (ServerList) 
	{
		PopulatePanel();
	}

	return true;
}

void UJoinList::PopulatePanel() 
{
	ServerList->AddItem(ListEntry);
}

void UJoinList::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : SessionResults)
	{
		FString SessionCodeValue;
		Result.Session.SessionSettings.Get(FName("SessionToken"), SessionCodeValue);
		if (SessionCodeValue == SessionToken)
		{
			MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
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
	ValidateButton->SetIsEnabled(false);
	FString PromptSessionToken = CodePromptBox->GetText().ToString();
	if (MultiplayerSessionsSubsystem)
	{
		SessionToken = PromptSessionToken;
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