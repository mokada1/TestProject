#pragma once

#include "UObject/Object.h"
#include "Containers/Queue.h"
#include "../../Network/Struct/BcastMove.h"
#include "BcastMoveQueue.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TESTPROJECT_API UBcastMoveQueue : public UObject
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool Dequeue(FBcastMove& value);

    UFUNCTION(BlueprintCallable)
    bool Enqueue(const FBcastMove& value);

    UFUNCTION(BlueprintCallable)
    void Empty();

    UFUNCTION(BlueprintPure)
    bool IsEmpty();

    UFUNCTION(BlueprintPure)
    bool Peek(FBcastMove& value);

private:
	TQueue<FBcastMove> queue;
};