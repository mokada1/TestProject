#include "ObjUser.h"
#include "../Util/TPDefine.h"

FString UObjUser::BP_GetUserId() const
{
	return FString(userId);
}

UCompUserTransform* UObjUser::BP_GetCompUserTransform() const
{
	return userTransform;
}

wchar_t* UObjUser::GetObjectId() const
{
	return GetUserId();
}

bool UObjUser::IsValid() const
{
	return userId != nullptr;
}

flatbuffers::Offset<TB_ObjUser> UObjUser::SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const
{
	char cUserId[SIZE_USER_USER_ID];
	TPUtil::GetInstance().WCharToChar(cUserId, SIZE_USER_USER_ID, userId);
	
	auto offsetUserId = _fbb.CreateString(cUserId);
	auto offsetUserTransform = userTransform ? userTransform->SerializeFB(_fbb) : 0;

	TB_ObjUserBuilder builder(_fbb);
	builder.add_UserId(offsetUserId);
	if (!offsetUserTransform.IsNull())
	{
		builder.add_UserTransform(offsetUserTransform);
	}
	return builder.Finish();
}

UObjUser* UObjUser::Create(wchar_t* const _userId)
{
	auto obj = NewObject<UObjUser>();
	obj->Init(_userId);
	return obj;
}

void UObjUser::Init()
{
	this->userId = nullptr;
	this->userTransform = nullptr;
}

void UObjUser::Init(wchar_t* const _userId)
{
	this->userId = new wchar_t[SIZE_USER_USER_ID];
	wcscpy_s(this->userId, SIZE_USER_USER_ID, _userId);
	this->userTransform = nullptr;
}

UObjUser::~UObjUser()
{
	if (userId)
	{
		delete[] userId;
	}
}

wchar_t* UObjUser::GetUserId() const
{
	return this->userId;
}

UCompUserTransform* UObjUser::GetCompUserTransform() const
{
	return this->userTransform;
}

void UObjUser::SetCompUserTransform(UCompUserTransform* const _userTransform)
{
	this->userTransform = _userTransform;
}