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

UCompUserAttribute* UObjUser::BP_GetCompAttribute() const
{
	return compAttribute;
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

UObjUser* UObjUser::Create(const TB_ObjUser& tb)
{
	wchar_t wUserId[SIZE_USER_USER_ID];
	TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, tb.UserId()->c_str());

	auto obj = NewObject<UObjUser>();
	obj->Init(wUserId);

	auto newCompTransform = UCompUserTransform::Create(*tb.Transform());
	auto newCompCondition = UCompUserCondition::Create(*tb.Condition());
	auto newCompAttribute = UCompUserAttribute::Create(*tb.Attribute());

	obj->SetCompTransform(newCompTransform);
	obj->SetCompCondition(newCompCondition);
	obj->SetCompAttribute(newCompAttribute);

	return obj;
}

void UObjUser::Init()
{
	userId = nullptr;
	compTransform = nullptr;
	compCondition = nullptr;
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
	return userId;
}

UCompUserTransform* UObjUser::GetCompTransform() const
{
	return compTransform;
}

UCompUserCondition* UObjUser::GetCompCondition() const
{
	return compCondition;
}

UCompUserAttribute* UObjUser::GetCompAttribute() const
{
	return compAttribute;
}

void UObjUser::SetCompTransform(UCompUserTransform* const _compTransform)
{
	this->compTransform = _compTransform;
}

void UObjUser::SetCompCondition(UCompUserCondition* const _compCondition)
{
	this->compCondition = _compCondition;
}

void UObjUser::SetCompAttribute(UCompUserAttribute* const _compAttribute)
{
	this->compAttribute = _compAttribute;
}
