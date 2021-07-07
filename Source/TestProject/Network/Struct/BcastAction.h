#pragma once

#include "../../Util/TPDefine.h"
#include "../../Util/TPUtil.h"
#include "../../Util/Enums.h"
#include "../../TP_generated.h"
#include "InputAction.h"
#include "BcastAction.generated.h"

USTRUCT(BlueprintType)
struct FBcastAction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString userId;

	UPROPERTY(BlueprintReadWrite)
	EOpAction operation;

	UPROPERTY(BlueprintReadWrite)
	FInputAction inputAction;

	FBcastAction()
	{
		userId = nullptr;
		operation = EOpAction::None;
		inputAction = FInputAction();
	}
	FBcastAction(const FString& _userId, const EOpAction& _operation, const FInputAction& _inputAction)
	{
		userId = _userId;
		operation = _operation;
		inputAction = _inputAction;
	}
	FBcastAction(const TB_BcastAction& bcastAction)
	{
		auto cUserId = bcastAction.UserId()->c_str();

		wchar_t wUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, cUserId);

		userId = FString(wUserId);
		operation = static_cast<EOpAction>(bcastAction.Operation());
		inputAction = FInputAction(*bcastAction.InputAction());
	}

	flatbuffers::Offset<TB_BcastAction> Serialize(flatbuffers::FlatBufferBuilder& _fbb)
	{
		char cUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().WCharToMultiByte(cUserId, SIZE_USER_USER_ID, *userId);

		auto offsetUserId = _fbb.CreateString(cUserId);
		auto offsetOperation = GetOffsetOperation();
		auto offsetInputAction = inputAction.Serialize(_fbb);

		TB_BcastActionBuilder builder(_fbb);
		builder.add_UserId(offsetUserId);
		builder.add_Operation(offsetOperation);
		builder.add_InputAction(offsetInputAction);
		return builder.Finish();
	}

	OpAction GetOffsetOperation()
	{
		return EnumValuesOpAction()[static_cast<uint8>(operation)];
	}
};