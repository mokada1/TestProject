#include "CompUserCondition.h"

bool UCompUserCondition::BP_GetIsCombatPosture() const
{
	return isCombatPosture;
}

bool UCompUserCondition::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserCondition> UCompUserCondition::SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const
{
	TB_CompUserConditionBuilder builder(_fbb);
	builder.add_IsCombatPosture(isCombatPosture);
	return builder.Finish();
}

UCompUserCondition* UCompUserCondition::Create(const bool _isCombatPosture)
{
	auto comp = NewObject<UCompUserCondition>();
	comp->Init(_isCombatPosture);
	return comp;
}

UCompUserCondition::~UCompUserCondition()
{
}

bool UCompUserCondition::GetIsCombatPosture() const
{
	return isCombatPosture;
}

void UCompUserCondition::Init()
{
	isValid = false;
	isCombatPosture = false;
}

void UCompUserCondition::Init(const bool _isCombatPosture)
{
	isValid = true;
	this->isCombatPosture = _isCombatPosture;
}