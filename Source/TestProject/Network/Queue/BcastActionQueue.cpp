#include "BcastActionQueue.h"

bool UBcastActionQueue::Dequeue(FBcastAction& value)
{
    return queue.Dequeue(value);
}
bool UBcastActionQueue::Enqueue(const FBcastAction& value)
{
    return queue.Enqueue(value);
}
void UBcastActionQueue::Empty()
{
    queue.Empty();
}
bool UBcastActionQueue::IsEmpty()
{
    return queue.IsEmpty();
}
bool UBcastActionQueue::Peek(FBcastAction& value)
{
    return queue.Peek(value);
}