#pragma once

UENUM(BluePrintType)
enum class ECharacterState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-HandedWeapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BluePrintType)
enum class EActionState : uint8
{
	EAS_Unoccupied,
	EAS_Attacking,
	EAS_Equipping
};