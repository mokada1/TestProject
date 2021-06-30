#pragma once

#include "UObject/Object.h"
#include "Containers/Queue.h"
#include "../../Network/Struct/BcastInputAction.h"
#include "BcastInputActionQueue.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TESTPROJECT_API UBcastInputActionQueue : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool Dequeue(FBcastInputAction& value);

    UFUNCTION(BlueprintCallable)
    bool Enqueue(const FBcastInputAction& value);

    UFUNCTION(BlueprintCallable)
    void Empty();

    UFUNCTION(BlueprintPure)
    bool IsEmpty();

    UFUNCTION(BlueprintPure)
    bool Peek(FBcastInputAction& value);

private:
    TQueue<FBcastInputAction> queue;
};