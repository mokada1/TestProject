#pragma once

#include "../../TP_generated.h"
#include "InputMove.generated.h"

USTRUCT(BlueprintType)
struct FInputMove
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FVector forwardVector;

	UPROPERTY(BlueprintReadWrite)
	FVector rightVector;

	UPROPERTY(BlueprintReadWrite)
	float moveForward;

	UPROPERTY(BlueprintReadWrite)
	float moveRight;

	FInputMove() {}
	FInputMove(const TB_InputMove& inputMove)
	{
		forwardVector = FVector(inputMove.ForwardVector()->x(),
			inputMove.ForwardVector()->y(),
			inputMove.ForwardVector()->z()
		);
		rightVector = FVector(inputMove.RightVector()->x(),
			inputMove.RightVector()->y(),
			inputMove.RightVector()->z()
		);
		moveForward = inputMove.MoveForward();
		moveRight = inputMove.MoveRight();
	}

	flatbuffers::Offset<TB_InputMove> Serialize(flatbuffers::FlatBufferBuilder& _fbb)
	{
		ST_Vec3 stForwardVector(forwardVector.X, forwardVector.Y, forwardVector.Z);
		ST_Vec3 stRightVector(rightVector.X, rightVector.Y, rightVector.Z);

		TB_InputMoveBuilder builder(_fbb);
		builder.add_ForwardVector(&stForwardVector);
		builder.add_RightVector(&stRightVector);
		builder.add_MoveForward(moveForward);
		builder.add_MoveRight(moveRight);
		return builder.Finish();
	}
};