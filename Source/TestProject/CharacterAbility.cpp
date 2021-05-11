// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAbility.h"

void UCharacterAbility::EndAbility(const FAbliltyActivationInfo& activationInfo, UAbliltyUseInfo* abilityUseInfo)
{
	K2_DeactivateAbility(activationInfo, abilityUseInfo);
	abilityUseInfo->SetIsActivated(false);
}

void UCharacterAbility::CallActivateAbility(const FAbliltyActivationInfo& activationInfo, UAbliltyUseInfo* abilityUseInfo)
{
	abilityUseInfo->SetIsActivated(true);
	K2_ActivateAbility(activationInfo, abilityUseInfo);
}