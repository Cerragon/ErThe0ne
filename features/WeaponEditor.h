#pragma once
#include <unordered_map>

class WeaponEditor
{
public:
	static void EditWeapons(bool enabled);
	static bool InfiniteAmmo(bool enabled);

private:
	struct WeaponInfo
	{
		std::uintptr_t ptr;
		struct Weapon
		{
			std::uintptr_t vtable;//0x08
			char padding0008[0x18];
			std::uint32_t formId;//0x20
			char padding0024[0x19C];
			std::uintptr_t keywordArrayPtr;//0x1C0
			int keywordArraySize;//0x1C8
			char padding01D0[0xE0];
			std::uintptr_t aimModelPtr;//0x2B0
			char padding02B8[0x44];
			float reloadSpeed;//0x2FC
			char padding0300[0x4];
			float speed;//0x304
			float reach;//0x308
			float reachEngagementMult; //0x030C
			float minRange; //0x0310
			float maxRange; //0x0314
			float padding0318; //0x0318
			float attackDelay; //0x031C
			float padding0320; //0x0320
			float damage; //0x0324
			float padding0328; //0x0328
			float padding032C; //0x032C
			float weight; //0x0330
			float padding0334; //0x0334
			float actionPointCost;//0x338
			char padding033C[0x24];
			unsigned char flagA;//0x360
			unsigned char flagB;//0x361
			unsigned char flagC;//0x362
			unsigned char flagD;//0x363
			char padding0364[0x1C];
			short capacity;//0x380
			short baseDamage;//0x382

			bool operator==(const Weapon&) const = default;
		} weaponData;

		struct AimModel
		{
			std::uintptr_t vtable;//0x8
			char padding0008[0x18];
			std::uint32_t formId;//0x20
			char padding0024[0x4];
			char spreadData[0x18];//0x28
			char recoilData[0x24];//0x40
			float sway;//0x64

			bool operator==(const AimModel&) const = default;
		} aimModelData;
	};
	static void ResetWeapons();

	inline static std::unordered_map<std::uint32_t, WeaponInfo> originalWeaponValues = {};
};
