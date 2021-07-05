#pragma once

#include "UObject/Object.h"
#include "Containers/Queue.h"
#include "../../Network/Struct/BcastAction.h"
#include "BcastActionQueue.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TESTPROJECT_API UBcastActionQueue : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool Dequeue(FBcastAction& value);

    UFUNCTION(BlueprintCallable)
    bool Enqueue(const FBcastAction& value);

    UFUNCTION(BlueprintCallable)
    void Empty();

    UFUNCTION(BlueprintPure)
    bool IsEmpty();

    UFUNCTION(BlueprintPure)
    bool Peek(FBcastAction& value);

private:
    TQueue<FBcastAction> queue;
};