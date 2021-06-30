#pragma once

#include "../../Util/TPDefine.h"
#include "../../Util/TPUtil.h"
#include "../../Util/Enums.h"
#include "../../TP_generated.h"
#include "BcastInputAction.generated.h"

USTRUCT(BlueprintType)
struct FBcastInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString userId;

	UPROPERTY(BlueprintReadWrite)
	EOpAction operation;

	FBcastInputAction()
	{
		userId = nullptr;
		operation = EOpAction::None;
	}
	FBcastInputAction(const FString& _userId, const EOpAction& _operation)
	{
		userId = _userId;
		operation = _operation;
	}
	FBcastInputAction(const TB_BcastInputAction& bcastInputAction)
	{
		auto cUserId = bcastInputAction.UserId()->c_str();

		wchar_t wUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, cUserId);

		userId = FString(wUserId);
		operation = static_cast<EOpAction>(bcastInputAction.Operation());
	}

	flatbuffers::Offset<TB_BcastInputAction> Serialize(flatbuffers::FlatBufferBuilder& _fbb)
	{
		char cUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().WCharToMultiByte(cUserId, SIZE_USER_USER_ID, *userId);

		auto offsetUserId = _fbb.CreateString(cUserId);
		auto offsetOperation = GetOffsetOperation();

		TB_BcastInputActionBuilder builder(_fbb);
		builder.add_UserId(offsetUserId);
		builder.add_Operation(offsetOperation);
		return builder.Finish();
	}

	OpAction GetOffsetOperation()
	{
		return EnumValuesOpAction()[static_cast<uint8>(operation)];
	}
};