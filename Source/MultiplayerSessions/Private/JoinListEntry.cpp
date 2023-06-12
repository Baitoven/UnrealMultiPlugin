#include "JoinListEntry.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsSubsystem.h"

bool UJoinListEntry::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	if (JoinButton) 
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	return true;
}

void UJoinListEntry::JoinButtonClicked()
{
	// Implemented in Blueprints
}
