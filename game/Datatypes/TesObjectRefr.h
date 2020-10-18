#pragma once
#include <cstdint>

#include "TesItem.h"
#include "../../utils.h"

enum class ActorState
{
	Unknown,
	Alive,
	Dead,
	Downed,
};

class ActorSnapshotComponent
{
public:
	//ActorCoreSnapshotComponent
	std::uintptr_t actorCorevtable; //0x0
	char actorCorePadding0008[0x98];
	//ActorServerAuthSnapshotData    // 0x38 isInvulnerable, 0x3C isProtected, 0x3D IsPlayerProtected, 0x130 hostileState, 0x138 reconScopeTargetState
	std::uintptr_t vtable; //0xA0
	char padding0008[0x30];
	char isInvulnerable; //0x38
	char unk0039; //0x39
	char unk003A; //0x3A
	char isEssential; //0x3B
	char isProtected; //0x3C
	char isPlayerProtected; //0x3D
	char padding003C[0x3A];
	float maxHealth; //0x78
	float modifiedHealth; //0x7C
	char padding0078[0x4];
	float lostHealth; //0x84
	char padding0080[0xA0];
	std::uint8_t epicRank; //0x128
	char padding0121[0xF];
	std::uint64_t hostileState; //0x138
	std::uint64_t reconScopeTargetState; //0x140
};

class Inventory
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x58];
	std::uintptr_t entryArrayBegin;//0x60
	std::uintptr_t entryArrayEnd;//0x68
};

class InventoryEntry
{
public:
	std::uintptr_t baseObjectPtr;//0x0
	std::uintptr_t instancePtr;
	std::uintptr_t displayPtr;//0x10
	char padding0018[0x8];
	std::uintptr_t iterations;//0x20
	char equipFlag;//0x28
	char padding0025[0x3];
	std::uint32_t itemId;//0x2C
	char favoriteIndex;//0x30
	char padding0031[0x7];
};

class TesObjectRefr
{
public:
	[[nodiscard]] FormType GetFormType() const { return static_cast<FormType>(formType); }
	[[nodiscard]] TesItem GetBaseObject() const;
	[[nodiscard]] ActorState GetActorState() const;

	[[nodiscard]] bool IsEssential() const { return GetActorSnapshot().isEssential; }
	[[nodiscard]] bool IsHostile() const { return GetActorSnapshot().hostileState != 0; }

	[[nodiscard]] std::uint8_t GetEpicRank() const { return GetActorSnapshot().epicRank; }

	[[nodiscard]] float GetCurrentHealth() const
	{
		const auto snapshot = GetActorSnapshot();
		return snapshot.maxHealth + snapshot.modifiedHealth + snapshot.lostHealth;
	}

	[[nodiscard]] std::vector<InventoryEntry> GetInventory() const;

private:
	[[nodiscard]] ActorSnapshotComponent GetActorSnapshot() const;

public:
	std::uintptr_t vtable; //0x0
	char padding0008[0x8];
	char harvestFlagA; //0x10
	char padding0011[0x8];
	char harvestFlagB; //0x19
	char padding001A[0x6];
	std::uint32_t formId; //0x20
	char padding0024[0x2]; //0x24
	std::uint8_t formType; //0x26
	char padding0027[0x11]; //0x27
	char idValue[4]; //0x38
	char padding003C[0x14];
	std::uintptr_t vtable0050; //0x50
	char padding0058[0x8];
	float pitch; //0x60
	char padding0064[0x4];
	float yaw; //0x68
	char padding006C[0x4];
	Vector3 position; //0x70
	char padding007C[0x4];
	std::uintptr_t inventoryPtr; //0x80
	char padding0088[0x8];
	std::uintptr_t actorCorePtr; //0x90
	char padding0098[0x10];
	std::uintptr_t cellPtr; //0xA8
	std::uintptr_t skeletonPtr; //0xB0
	std::uintptr_t baseObjectPtr; //0xB8
	char padding00C0[0xE];
	char spawnFlag; //0xCE
	char padding00Cf[0xD9];
	char movementFlag; //0x1A8
	char sprintFlag; //0x1A9
	char healthFlag; //0x1AA
	char padding019B[0xAA9];
	std::uint32_t playerStashFormId; //0xC54
	char padding0C58[0x140];
	std::uintptr_t playerKnownRecipes;

	std::uintptr_t ptr; //not really there, fill it manually
};

