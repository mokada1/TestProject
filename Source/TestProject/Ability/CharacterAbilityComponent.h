// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Util/TPUtil.h"
#include "CharacterAbility.h"
#include "AbilityUseInfo.h"
#include "Components/ActorComponent.h"
#include "CharacterAbilityComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom))
class TESTPROJECT_API UCharacterAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = Ability)
	bool ActivateAbility(const FAbilityActivationInfo& activationInfo);

	UFUNCTION(BlueprintCallable, Category = Ability)
	bool IsActivatedAbility(const ECharacterAbilityGroup abilityGroup) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
		TSet<TSubclassOf<UCharacterAbility>> abilities;

	UPROPERTY(BlueprintReadOnly, Category = Ability)
		TSet<UAbilityUseInfo*> abilityUseInfoSet;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "ActivateAbility", meta = (ScriptName = "ActivateAbility"))
		void K2_ActivateAbility(const FAbilityActivationInfo& activationInfo, UAbilityUseInfo* abilityUseInfo);

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "DeactivateAbility", meta = (ScriptName = "DeactivateAbility"))
		void K2_DeactivateAbility(const FAbilityActivationInfo& activationInfo, UAbilityUseInfo* abilityUseInfo);

private:
	void Initialize();
	
	bool ContainsUseInfo(const FAbilityActivationInfo& activationInfo) const;
	UAbilityUseInfo* FindUseInfo(const FAbilityActivationInfo& activationInfo) const;
	void SetIsActivated(ECharacterAbility abilityType, bool isActivated);

	UFUNCTION(BlueprintCallable, Category = Ability)
	void EndAbility(const FAbilityActivationInfo& activationInfo, UAbilityUseInfo* abilityUseInfo);

	void CallActivateAbility(const FAbilityActivationInfo& activationInfo, UAbilityUseInfo* abilityUseInfo);
};
