// Fill out your copyright notice in the Description page of Project Settings.


#include "AbliltyUseInfo.h"

void UAbliltyUseInfo::Initialize(ECharacterAbility _abilityType, ECharacterAbilityGroup _abilityGroupType)
{
	this->abilityType = _abilityType;
	this->abilityGroupType = _abilityGroupType;
	this->isActivated = false;
}

ECharacterAbility UAbliltyUseInfo::GetAbilityType() const
{
	return this->abilityType;
}

ECharacterAbilityGroup UAbliltyUseInfo::GetAbilityGroupType() const
{
	return this->abilityGroupType;
}

bool UAbliltyUseInfo::GetIsActivated() const
{
	return this->isActivated;
}

void UAbliltyUseInfo::SetIsActivated(const bool _isActivated)
{
	this->isActivated = _isActivated;
}
