#include "ObjUser.h"
#include "../Util/TPDefine.h"

FString UObjUser::BP_GetUserId() const
{
	return FString(userId);
}

UCompUserTransform* UObjUser::BP_GetCompTransform() const
{
	return compTransform;
}

UCompUserCondition* UObjUser::BP_GetCompCondition() const
{
	return compCondition;
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
	auto offsetTransform = compTransform ? compTransform->SerializeFB(_fbb) : 0;
	auto offsetCondition = compCondition ? compCondition->SerializeFB(_fbb) : 0;

	TB_ObjUserBuilder builder(_fbb);
	builder.add_UserId(offsetUserId);
	builder.add_Transform(offsetTransform);
	builder.add_Condition(offsetCondition);
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
	this->compTransform = nullptr;
	this->compCondition = nullptr;
}

void UObjUser::Init(wchar_t* const _userId)
{
	this->userId = new wchar_t[SIZE_USER_USER_ID];
	wcscpy_s(this->userId, SIZE_USER_USER_ID, _userId);
	this->compTransform = nullptr;
	this->compCondition = nullptr;
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

UCompUserTransform* UObjUser::GetCompTransform() const
{
	return this->compTransform;
}

UCompUserCondition* UObjUser::GetCompCondition() const
{
	return this->compCondition;
}

void UObjUser::SetCompTransform(UCompUserTransform* const _compTransform)
{
	this->compTransform = _compTransform;
}

void UObjUser::SetCompCondition(UCompUserCondition* const _compCondition)
{
	this->compCondition = _compCondition;
}