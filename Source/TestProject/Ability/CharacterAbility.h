// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Util/Enums.h"
#include "AbliltyUseInfo.h"
#include "Animation/AnimMontage.h"
#include "UObject/Object.h"
#include "CharacterAbility.generated.h"

USTRUCT(BlueprintType)
struct FAbliltyActivationInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	AActor* owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	ECharacterAbility abilityType;
};

UCLASS(Blueprintable, ClassGroup = (Custom))
class TESTPROJECT_API UCharacterAbility : public UObject
{
	GENERATED_BODY()
	
public:
	friend class UCharacterAbilityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
		ECharacterAbility abilityType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
		ECharacterAbilityGroup abilityGroupType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
		UAnimMontage* playToMontage;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "ActivateAbility", meta = (ScriptName = "ActivateAbility"))
		void K2_ActivateAbility(const FAbliltyActivationInfo& activationInfo, UAbliltyUseInfo* abilityUseInfo);
	
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "DeactivateAbility", meta = (ScriptName = "DeactivateAbility"))
		void K2_DeactivateAbility(const FAbliltyActivationInfo& activationInfo, UAbliltyUseInfo* abilityUseInfo);

	UFUNCTION(BlueprintCallable, Category = Ability)
	void EndAbility(const FAbliltyActivationInfo& activationInfo, UAbliltyUseInfo* abilityUseInfo);

	void CallActivateAbility(const FAbliltyActivationInfo& activationInfo, UAbliltyUseInfo* abilityUseInfo);
};