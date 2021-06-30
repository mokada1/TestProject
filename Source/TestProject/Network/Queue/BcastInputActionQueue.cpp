#include "BcastInputActionQueue.h"

bool UBcastInputActionQueue::Dequeue(FBcastInputAction& value)
{
    return queue.Dequeue(value);
}
bool UBcastInputActionQueue::Enqueue(const FBcastInputAction& value)
{
    return queue.Enqueue(value);
}
void UBcastInputActionQueue::Empty()
{
    queue.Empty();
}
bool UBcastInputActionQueue::IsEmpty()
{
    return queue.IsEmpty();
}
bool UBcastInputActionQueue::Peek(FBcastInputAction& value)
{
    return queue.Peek(value);
}