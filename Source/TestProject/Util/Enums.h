#pragma once

UENUM(BlueprintType)
enum class ECharacterAbility : uint8
{
	None		UMETA(DisplayName = "None"),
	MeleeAttack	UMETA(DisplayName = "MeleeAttack"),
	RangeAttack	UMETA(DisplayName = "RangeAttack"),
	Skill1		UMETA(DisplayName = "Skill1"),
	Skill2		UMETA(DisplayName = "Skill2"),
	Skill3		UMETA(DisplayName = "Skill3"),
};

UENUM(BlueprintType)
enum class ECharacterAbilityGroup : uint8
{
	None	UMETA(DisplayName = "None"),
	Melee	UMETA(DisplayName = "Melee"),
	Range	UMETA(DisplayName = "Range"),
	Skill	UMETA(DisplayName = "Skill"),
};

UENUM(BlueprintType)
enum class ECharacterAttribute : uint8
{
	None	UMETA(DisplayName = "None"),
	Hp		UMETA(DisplayName = "Hp"),
	Str		UMETA(DisplayName = "Str"),
};

UENUM(BlueprintType)
enum class EAlly : uint8
{
	Neutral		UMETA(DisplayName = "Neutral"),
	Player		UMETA(DisplayName = "Player"),
	Enemy		UMETA(DisplayName = "Enemy"),
	Friend		UMETA(DisplayName = "Friend"),
};

UENUM(BlueprintType)
enum class EOpMove : uint8
{
	None		UMETA(DisplayName = "None"),
	Start		UMETA(DisplayName = "Start"),
	Change		UMETA(DisplayName = "Change"),
	End			UMETA(DisplayName = "End"),	
};

UENUM(BlueprintType)
enum class EOpAction : uint8
{
	None			UMETA(DisplayName = "None"),
	Jump			UMETA(DisplayName = "Jump"),
	Vaulting		UMETA(DisplayName = "Vaulting"),
	DrawWeapon		UMETA(DisplayName = "DrawWeapon"),
	MeleeAttack		UMETA(DisplayName = "MeleeAttack"),
};