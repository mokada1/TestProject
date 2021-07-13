#include "CompUserAttribute.h"

float UCompUserAttribute::BP_GetHp() const
{
	return hp;
}

float UCompUserAttribute::BP_GetStr() const
{
	return str;
}

bool UCompUserAttribute::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserAttribute> UCompUserAttribute::SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const
{
	TB_CompUserAttributeBuilder builder(_fbb);
	builder.add_Hp(hp);
	builder.add_Str(str);
	return builder.Finish();
}

UCompUserAttribute* UCompUserAttribute::Create(const TB_CompUserAttribute& tb)
{
	auto comp = NewObject<UCompUserAttribute>();
	comp->Init(tb.Hp(), tb.Str());
	return comp;
}

UCompUserAttribute::~UCompUserAttribute()
{
}

float UCompUserAttribute::GetHp() const
{
	return hp;
}

float UCompUserAttribute::GetStr() const
{
	return str;
}

void UCompUserAttribute::Init()
{
	isValid = false;
	hp = 0.f;
	str = 0.f;
}

void UCompUserAttribute::Init(const float _hp, const float _str)
{
	this->isValid = true;
	this->hp = _hp;
	this->str = _str;
}
