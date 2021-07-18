// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Util/Enums.h"
#include "Animation/AnimMontage.h"
#include "UObject/Object.h"
#include "CharacterAbility.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom))
class TESTPROJECT_API UCharacterAbility : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
		ECharacterAbility abilityType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
		ECharacterAbilityGroup abilityGroupType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
		UAnimMontage* playToMontage;
};