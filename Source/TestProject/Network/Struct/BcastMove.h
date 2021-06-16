#pragma once

#include "../../Util/TPDefine.h"
#include "../../Util/TPUtil.h"
#include "InputMove.h"
#include "../../Util/Enums.h"
#include "BcastMove.generated.h"

USTRUCT(BlueprintType)
struct FBcastMove
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FString userId;

	UPROPERTY(BlueprintReadWrite)
	EOpMove operation;

	UPROPERTY(BlueprintReadWrite)
	FInputMove inputMove;

	FBcastMove() {}
	FBcastMove(const FString& _userId, const EOpMove& _operation, const FInputMove& _inputMove)
	{
		userId = _userId;
		operation = _operation;
		inputMove = _inputMove;
	}
	FBcastMove(const TB_BcastMove& bcastMove)
	{
		auto mlUserId = bcastMove.UserId()->c_str();

		wchar_t wUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, mlUserId);

		userId = FString(wUserId);
		operation = static_cast<EOpMove>(bcastMove.Operation());
		inputMove = FInputMove(*bcastMove.InputMove());
	}

	flatbuffers::Offset<TB_BcastMove> Serialize(flatbuffers::FlatBufferBuilder& _fbb)
	{
		char hUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().WCharToMultiByte(hUserId, SIZE_USER_USER_ID, *userId);

		auto offsetUserId = _fbb.CreateString(hUserId);
		auto offsetOperation = GetOffsetOperation();
		auto offsetInputMove = inputMove.Serialize(_fbb);

		TB_BcastMoveBuilder builder(_fbb);
		builder.add_UserId(offsetUserId);
		builder.add_Operation(offsetOperation);
		builder.add_InputMove(offsetInputMove);
		return builder.Finish();
	}

	OpMove GetOffsetOperation()
	{
		return EnumValuesOpMove()[static_cast<uint8>(operation)];
	}
};