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
	void SetSession(FString vSessionToken);

	UPROPERTY(BlueprintReadOnly)
	FString SessionToken { TEXT("") };

protected:

	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	
	/*UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionTokenText;*/

	UFUNCTION()
	void JoinButtonClicked();

	UFUNCTION()
	FText SetSessionText();
};