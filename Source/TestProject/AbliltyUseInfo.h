// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPUtil.h"
#include "UObject/Object.h"
#include "AbliltyUseInfo.generated.h"

UCLASS()
class TESTPROJECT_API UAbliltyUseInfo : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(ECharacterAbility _abilityType, ECharacterAbilityGroup _abilityGroupType);

	ECharacterAbility GetAbilityType() const;
	ECharacterAbilityGroup GetAbilityGroupType() const;
	bool GetIsActivated() const;
	void SetIsActivated(const bool _isActivated);
private:
	ECharacterAbility abilityType;
	ECharacterAbilityGroup abilityGroupType;
	bool isActivated;
};
