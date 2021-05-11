// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeComponent.h"
#include "GameFramework/Character.h"

bool UCharacterAttributeComponent::isAlive()
{
	auto hp = attributes.Find(ECharacterAttribute::Hp);
	if(hp && *hp > 0.f)
	{
		return true;
	}
	return false;
}

void UCharacterAttributeComponent::Hit(const AActor* attacker)
{
	auto attackerComp = attacker->GetComponentByClass(UCharacterAttributeComponent::StaticClass());
	auto attackerAttributeComp = Cast<UCharacterAttributeComponent>(attackerComp);

	// 공격 피해량 적용
	auto attackerStr = attackerAttributeComp->attributes.Find(ECharacterAttribute::Str);
	auto targetHp = this->attributes.Find(ECharacterAttribute::Hp);
	*targetHp = FMath::Clamp((*targetHp - *attackerStr), 0.f, *targetHp);

	UE_LOG(LogTemp, Log, TEXT("targetHp : %f"), *targetHp);
}
