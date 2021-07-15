// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Util/Enums.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributeComponent.generated.h"

class UCompUserAttribute;

UCLASS(Blueprintable, ClassGroup = (Custom))
class TESTPROJECT_API UCharacterAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Attribute)
	bool isAlive();

	UFUNCTION(BlueprintCallable, Category = Attribute)
	float UpdateAttribute(const ECharacterAttribute attribute, const float value);

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void UpdateAttrByCompUserAttr(UCompUserAttribute* const compUserAttribute);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attribute)
	TMap<ECharacterAttribute, float> attributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attribute)
	TMap<ECharacterAttribute, float> attributesMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
	UAnimMontage* montageForHit;
};