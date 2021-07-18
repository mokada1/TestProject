// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityUseInfo.h"

void UAbilityUseInfo::Initialize(ECharacterAbility _abilityType, ECharacterAbilityGroup _abilityGroupType, UAnimMontage* _playToMontage)
{
	this->isActivated = false;
	this->abilityType = _abilityType;
	this->abilityGroupType = _abilityGroupType;
	this->playToMontage = _playToMontage;
}

bool UAbilityUseInfo::GetIsActivated() const
{
	return this->isActivated;
}

ECharacterAbility UAbilityUseInfo::GetAbilityType() const
{
	return this->abilityType;
}

ECharacterAbilityGroup UAbilityUseInfo::GetAbilityGroupType() const
{
	return this->abilityGroupType;
}

UAnimMontage* UAbilityUseInfo::GetPlayToMontage() const
{
	return this->playToMontage;
}

void UAbilityUseInfo::SetIsActivated(const bool _isActivated)
{
	this->isActivated = _isActivated;
}
