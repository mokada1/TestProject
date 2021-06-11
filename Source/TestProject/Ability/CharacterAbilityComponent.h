// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Util/TPUtil.h"
#include "CharacterAbility.h"
#include "Components/ActorComponent.h"
#include "CharacterAbilityComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom))
class TESTPROJECT_API UCharacterAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
		TSet<TSubclassOf<UCharacterAbility>> abilities;

	UFUNCTION(BlueprintCallable, Category = Ability)
	bool ActivateAbility(const FAbliltyActivationInfo& activationInfo);

	UFUNCTION(BlueprintCallable, Category = Ability)
	bool IsActivatedAbility(const ECharacterAbilityGroup abilityGroup) const;

	UPROPERTY(BlueprintReadOnly, Category = Ability)
	TSet<UAbliltyUseInfo*> abilityUseInfoSet;
protected:
	virtual void BeginPlay() override;

private:
	void Initialize();
	
	bool ContainsUseInfo(const FAbliltyActivationInfo& activationInfo) const;
	UAbliltyUseInfo* FindUseInfo(const FAbliltyActivationInfo& activationInfo) const;
	UCharacterAbility* FindAbility(const FAbliltyActivationInfo& activationInfo) const;
	void SetIsActivated(ECharacterAbility abilityType, bool isActivated);
};
