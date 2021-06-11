// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAbilityComponent.h"

void UCharacterAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void UCharacterAbilityComponent::Initialize()
{
	if (abilityUseInfoSet.Num() > 0)
	{
		for (auto& abilityUseInfo : abilityUseInfoSet)
		{
			abilityUseInfo->SetIsActivated(false);
		}
	}
	else
	{
		for (auto& ability : abilities)
		{
			auto abilityCDO = ability.GetDefaultObject();
			if (abilityCDO)
			{
				auto useInfo = NewObject<UAbliltyUseInfo>();
				useInfo->Initialize(abilityCDO->abilityType, abilityCDO->abilityGroupType);
				abilityUseInfoSet.Add(useInfo);
			}
		}
	}	
}

bool UCharacterAbilityComponent::ContainsUseInfo(const FAbliltyActivationInfo& activationInfo) const
{
	for (auto& abilityUseInfo : abilityUseInfoSet)
	{
		if (abilityUseInfo->GetAbilityType() == activationInfo.abilityType)
		{
			return true;
		}
	}
	return false;
}

UAbliltyUseInfo* UCharacterAbilityComponent::FindUseInfo(const FAbliltyActivationInfo& activationInfo) const
{
	for (auto& abilityUseInfo : abilityUseInfoSet)
	{
		if (abilityUseInfo->GetAbilityType() == activationInfo.abilityType)
		{
			return abilityUseInfo;
		}
	}
	return nullptr;
}

UCharacterAbility* UCharacterAbilityComponent::FindAbility(const FAbliltyActivationInfo& activationInfo) const
{
	for (auto& ability : abilities)
	{
		auto abilityCDO = ability.GetDefaultObject();
		if (abilityCDO->abilityType == activationInfo.abilityType)
		{
			return abilityCDO;
		}
	}
	return nullptr;
}

void UCharacterAbilityComponent::SetIsActivated(ECharacterAbility abilityType, bool isActivated)
{
	for (auto& abilityUseInfo : abilityUseInfoSet)
	{
		if (abilityUseInfo->GetAbilityType() == abilityType)
		{
			abilityUseInfo->SetIsActivated(isActivated);
		}
	}
}

bool UCharacterAbilityComponent::ActivateAbility(const FAbliltyActivationInfo& activationInfo)
{
	auto useInfo = FindUseInfo(activationInfo);
	if (useInfo && !useInfo->GetIsActivated())
	{
		auto ability = FindAbility(activationInfo);
		if (ability)
		{
			ability->CallActivateAbility(activationInfo, useInfo);
		}		
		return true;
	}
	return false;
}

bool UCharacterAbilityComponent::IsActivatedAbility(const ECharacterAbilityGroup abilityGroup) const
{
	for (auto& abilityUseInfo : abilityUseInfoSet)
	{
		if (abilityUseInfo->GetAbilityGroupType() == abilityGroup && abilityUseInfo->GetIsActivated())
		{
			return true;
		}
	}
	return false;
}
