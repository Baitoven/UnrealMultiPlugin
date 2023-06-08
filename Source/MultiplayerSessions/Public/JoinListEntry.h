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
protected:

	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void JoinButtonClicked();
};