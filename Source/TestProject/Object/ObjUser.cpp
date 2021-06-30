#include "ObjUser.h"
#include "../Util/TPDefine.h"
#include <wchar.h>

FString UObjUser::BP_GetUserId() const
{
	return FString(userId);
}

UCompUserLocation* UObjUser::BP_GetCompUserLocation() const
{
	return userLocation;
}

wchar_t* UObjUser::GetObjectId() const
{
	return GetUserId();
}

bool UObjUser::IsValid() const
{
	return userId != nullptr && password != nullptr;
}

flatbuffers::Offset<TB_ObjUser> UObjUser::SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const
{
	char cUserId[SIZE_USER_USER_ID], cPassword[SIZE_USER_PASSWORD];
	TPUtil::GetInstance().WCharToChar(cUserId, SIZE_USER_USER_ID, userId);
	TPUtil::GetInstance().WCharToChar(cPassword, SIZE_USER_PASSWORD, password);
	auto offsetUserId = _fbb.CreateString(cUserId);
	auto offsetPassword = _fbb.CreateString(cPassword);
	auto offsetUserLocation = userLocation ? userLocation->SerializeFB(_fbb) : 0;

	TB_ObjUserBuilder builder(_fbb);
	builder.add_UserId(offsetUserId);
	builder.add_Password(offsetPassword);
	if (!offsetUserLocation.IsNull())
	{
		builder.add_UserLocation(offsetUserLocation);
	}
	return builder.Finish();
}

UObjUser* UObjUser::Create(wchar_t* const _userId, wchar_t* const _password)
{
	auto obj = NewObject<UObjUser>();
	obj->Init(_userId, _password);
	return obj;
}

void UObjUser::Init()
{
	this->userId = nullptr;
	this->password = nullptr;
	this->userLocation = nullptr;
}

void UObjUser::Init(wchar_t* const _userId, wchar_t* const _password)
{
	this->userId = new wchar_t[SIZE_USER_USER_ID];
	this->password = new wchar_t[SIZE_USER_PASSWORD];
	wcscpy_s(this->userId, SIZE_USER_USER_ID, _userId);
	wcscpy_s(this->password, SIZE_USER_PASSWORD, _password);
	this->userLocation = nullptr;
}

UObjUser::~UObjUser()
{
	if (userId)
	{
		delete[] userId;
	}
	if (password)
	{
		delete[] password;
	}
}

wchar_t* UObjUser::GetUserId() const
{
	return this->userId;
}

wchar_t* UObjUser::GetPassword() const
{
	return this->password;
}

UCompUserLocation* UObjUser::GetCompUserLocation() const
{
	return this->userLocation;
}

void UObjUser::SetCompUserLocation(UCompUserLocation* const _userLocation)
{
	this->userLocation = _userLocation;
}