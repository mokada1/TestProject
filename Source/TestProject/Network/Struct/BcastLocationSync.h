#pragma once

#include "../../Util/TPDefine.h"
#include "../../Util/TPUtil.h"
#include "../../TP_generated.h"
#include "BcastLocationSync.generated.h"

USTRUCT(BlueprintType)
struct FBcastLocationSync
{
	GENERATED_BODY()

	public:
	UPROPERTY(BlueprintReadWrite)
	FString userId;

	UPROPERTY(BlueprintReadWrite)
	FVector location;

	FBcastLocationSync()
	{
		userId = nullptr;
		location = FVector(0.f, 0.f, 0.f);
	}
	FBcastLocationSync(const FString& _userId, const FVector& _location)
	{
		userId = _userId;
		location = _location;
	}
	FBcastLocationSync(const TB_BcastLocationSync& bcastLocationSync)
	{
		auto cUserId = bcastLocationSync.UserId()->c_str();

		wchar_t wUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, cUserId);

		auto stLocation = bcastLocationSync.Location();

		userId = FString(wUserId);
		location = FVector(stLocation->x(), stLocation->y(), stLocation->z());
	}

	flatbuffers::Offset<TB_BcastLocationSync> Serialize(flatbuffers::FlatBufferBuilder& _fbb)
	{
		char cUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().WCharToMultiByte(cUserId, SIZE_USER_USER_ID, *userId);

		auto offsetUserId = _fbb.CreateString(cUserId);
		ST_Vec3 stLocation(location.X, location.Y, location.Z);

		TB_BcastLocationSyncBuilder builder(_fbb);
		builder.add_UserId(offsetUserId);
		builder.add_Location(&stLocation);
		return builder.Finish();
	}
};