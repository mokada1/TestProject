#include "BcastMoveQueue.h"

bool UBcastMoveQueue::Dequeue(FBcastMove& value)
{
    return queue.Dequeue(value);
}
bool UBcastMoveQueue::Enqueue(const FBcastMove& value)
{
    return queue.Enqueue(value);
}
void UBcastMoveQueue::Empty()
{
    queue.Empty();
}
bool UBcastMoveQueue::IsEmpty()
{
    return queue.IsEmpty();
}
bool UBcastMoveQueue::Peek(FBcastMove& value)
{
    return queue.Peek(value);
}