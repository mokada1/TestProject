// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeComponent.h"
#include "GameFramework/Character.h"
#include "../Component/CompUserAttribute.h"

bool UCharacterAttributeComponent::isAlive()
{
	auto hp = attributes.Find(ECharacterAttribute::Hp);
	if(hp && *hp > 0.f)
	{
		return true;
	}
	return false;
}

float UCharacterAttributeComponent::UpdateAttribute(const ECharacterAttribute attribute, const float value)
{
	auto hp = attributes.Find(attribute);
	auto diff = value - *hp;
	*hp = value;
	return diff;
}

void UCharacterAttributeComponent::UpdateAttrByCompUserAttr(UCompUserAttribute* const compUserAttribute)
{
	auto hp = attributes.Find(ECharacterAttribute::Hp);
	auto maxHp = attributesMax.Find(ECharacterAttribute::Hp);
	auto str = attributes.Find(ECharacterAttribute::Str);
	if (hp)
	{
		*hp = compUserAttribute->GetHp();
	}
	if (maxHp)
	{
		*maxHp = compUserAttribute->GetMaxHp();
	}
	if (str)
	{
		*str = compUserAttribute->GetStr();
	}
}
