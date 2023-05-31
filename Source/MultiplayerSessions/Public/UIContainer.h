#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UIContainer.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API UUIContainer : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent)
	void DisplayMenuUI();

	UFUNCTION(BlueprintNativeEvent)
	void DisplayJoinListUI();

protected:

	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:

	void MenuTearDown();
};
