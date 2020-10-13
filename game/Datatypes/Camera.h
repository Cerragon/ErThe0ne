#pragma once
#include <cstdint>

#include "../../utils.h"

class Camera
{
public:
	[[nodiscard]] Vector2 World2Screen(const Vector3& position) const;
	
	std::uintptr_t vtable{};//0x0
	char padding0008[0x68]{};
	Vector3 forward;//0x70
	char padding007C[0x24]{};
	Vector3 origin;//0xA0
	char padding00Ac[0x84]{};
	float viewMatrix[4][4]{};//0x130
};

