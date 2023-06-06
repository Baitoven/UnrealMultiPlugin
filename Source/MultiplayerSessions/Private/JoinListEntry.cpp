#include "JoinListEntry.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsSubsystem.h"

void UJoinListEntry::SetSession(FString vSessionToken)
{
	SessionToken = vSessionToken;
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

FText UJoinListEntry::SetSessionText()
{
	return FText::FromString(SessionToken);
}

void UJoinListEntry::JoinButtonClicked()
{
	/*UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->JoinSession(Session);
	}*/
}
