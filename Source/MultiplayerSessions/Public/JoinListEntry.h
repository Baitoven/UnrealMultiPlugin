#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "JoinListEntry.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API UJoinListEntry : public UUserObjectListEntry 
{
	GENERATED_BODY()
public:
	FString SessionToken;

	UJoinListEntry(FString Token);
};