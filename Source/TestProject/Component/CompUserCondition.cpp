#include "CompUserCondition.h"

bool UCompUserCondition::BP_GetIsCombatPosture() const
{
	return isCombatPosture;
}

bool UCompUserCondition::BP_GetIsDied() const
{
	return isDied;
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

UCompUserCondition* UCompUserCondition::Create(const TB_CompUserCondition& tb)
{
	auto comp = NewObject<UCompUserCondition>();
	comp->Init(tb.IsCombatPosture(), tb.IsDied());
	return comp;
}

UCompUserCondition::~UCompUserCondition()
{
}

bool UCompUserCondition::GetIsCombatPosture() const
{
	return isCombatPosture;
}

bool UCompUserCondition::GetIsDied() const
{
	return isDied;
}

void UCompUserCondition::Init()
{
	isValid = false;
	isCombatPosture = false;
	isDied = false;
}

void UCompUserCondition::Init(const bool _isCombatPosture, const bool _isDied)
{
	this->isValid = true;
	this->isCombatPosture = _isCombatPosture;
	this->isDied = _isDied;
}