#pragma once

#include "../../Util/TPDefine.h"
#include "../../Util/TPUtil.h"
#include "../../TP_generated.h"
#include "BcastRotationSync.generated.h"

USTRUCT(BlueprintType)
struct FBcastRotationSync
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString userId;

	UPROPERTY(BlueprintReadWrite)
	FVector rotation;

	FBcastRotationSync()
	{
		userId = nullptr;
		rotation = FVector(0.f, 0.f, 0.f);
	}
	FBcastRotationSync(const FString& _userId, const FVector& _rotation)
	{
		userId = _userId;
		rotation = _rotation;
	}
	FBcastRotationSync(const TB_BcastRotationSync& bcastRotationSync)
	{
		auto cUserId = bcastRotationSync.UserId()->c_str();

		wchar_t wUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, cUserId);

		auto stRotation = bcastRotationSync.Rotation();

		userId = FString(wUserId);
		rotation = FVector(stRotation->x(), stRotation->y(), stRotation->z());
	}

	flatbuffers::Offset<TB_BcastRotationSync> Serialize(flatbuffers::FlatBufferBuilder& _fbb)
	{
		char cUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().WCharToMultiByte(cUserId, SIZE_USER_USER_ID, *userId);

		auto offsetUserId = _fbb.CreateString(cUserId);
		ST_Vec3 stRotation(rotation.X, rotation.Y, rotation.Z);

		TB_BcastRotationSyncBuilder builder(_fbb);
		builder.add_UserId(offsetUserId);
		builder.add_Rotation(&stRotation);
		return builder.Finish();
	}
};