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
				auto useInfo = NewObject<UAbilityUseInfo>();
				useInfo->Initialize(abilityCDO->abilityType, abilityCDO->abilityGroupType, abilityCDO->playToMontage);
				abilityUseInfoSet.Add(useInfo);
			}
		}
	}
}

bool UCharacterAbilityComponent::ContainsUseInfo(const FAbilityActivationInfo& activationInfo) const
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

UAbilityUseInfo* UCharacterAbilityComponent::FindUseInfo(const FAbilityActivationInfo& activationInfo) const
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

bool UCharacterAbilityComponent::ActivateAbility(const FAbilityActivationInfo& activationInfo)
{
	auto useInfo = FindUseInfo(activationInfo);
	if (useInfo && !useInfo->GetIsActivated())
	{
		CallActivateAbility(activationInfo, useInfo);
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

void UCharacterAbilityComponent::EndAbility(const FAbilityActivationInfo& activationInfo, UAbilityUseInfo* abilityUseInfo)
{
	K2_DeactivateAbility(activationInfo, abilityUseInfo);
	abilityUseInfo->SetIsActivated(false);
}

void UCharacterAbilityComponent::CallActivateAbility(const FAbilityActivationInfo& activationInfo, UAbilityUseInfo* abilityUseInfo)
{
	abilityUseInfo->SetIsActivated(true);
	K2_ActivateAbility(activationInfo, abilityUseInfo);
}