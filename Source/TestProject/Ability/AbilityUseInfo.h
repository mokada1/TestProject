// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Util/Enums.h"
#include "UObject/Object.h"
#include "Animation/AnimMontage.h"
#include "AbilityUseInfo.generated.h"

UCLASS()
class TESTPROJECT_API UAbilityUseInfo : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(ECharacterAbility _abilityType, ECharacterAbilityGroup _abilityGroupType, UAnimMontage* _playToMontage);

	UFUNCTION(BlueprintCallable, Category = Ability)
	bool GetIsActivated() const;	

	UFUNCTION(BlueprintCallable, Category = Ability)
	ECharacterAbility GetAbilityType() const;

	UFUNCTION(BlueprintCallable, Category = Ability)
	ECharacterAbilityGroup GetAbilityGroupType() const;

	UFUNCTION(BlueprintCallable, Category = Ability)
	UAnimMontage* GetPlayToMontage() const;

	void SetIsActivated(const bool _isActivated);
	
private:
	bool isActivated;
	ECharacterAbility abilityType;
	ECharacterAbilityGroup abilityGroupType;
	UAnimMontage* playToMontage;
};

USTRUCT(BlueprintType)
struct FAbilityActivationInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
		AActor* owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
		ECharacterAbility abilityType;
};
