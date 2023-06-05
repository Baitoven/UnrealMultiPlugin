#include "JoinListEntry.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsSubsystem.h"

void UJoinListEntry::SetSession(FOnlineSessionSearchResult vSession)
{
	vSession.Session.SessionSettings.Get(FName("SessionToken"), SessionToken);
	Session = vSession;
}

bool UJoinListEntry::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	if (SessionTokenText)
	{
		SessionTokenText->TextDelegate.BindUFunction(this, "SetSessionText");
	}
	if (JoinButton) 
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	return true;
}

void UJoinListEntry::PostInitProperties()
{
	Super::PostInitProperties();
	SessionToken = "Prout"; // TODO: Set Session Token right here
	SetSessionText();
}

void UJoinListEntry::JoinButtonClicked()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->JoinSession(Session);
	}
}

FText UJoinListEntry::SetSessionText()
{
	return FText::FromString(SessionToken);
}
