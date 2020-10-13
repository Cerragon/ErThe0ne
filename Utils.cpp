#include "utils.h"

#include <algorithm>

void Utils::ValidateRgb(float* value)
{
	for (auto i = 0; i < 3; i++)
	{
		value[i] = std::clamp(value[i], 0.0f, 1.0f);
	}
}

bool Utils::Valid(const std::uintptr_t ptr)
{
	if (ptr < 0x10000UL || ptr > 0xF000000000000ULL || ptr & 1)
		return false;

	return true;
}

Vector3 Utils::ProjectView(const Vector3& forward, const Vector3& origin, const float distance)
{
	return origin + forward * distance;
}

float Utils::RadiansToDegrees(const float radians)
{
	return radians * (180.0f / 3.14159265358979323846f);
}

float Utils::GetDegrees(const Vector3& src, const Vector3& forward, const Vector3& origin)
{
	const auto buffer = ProjectView(forward, origin, src.DistanceTo(origin));
	return RadiansToDegrees(sin(src.DistanceTo(buffer) / origin.DistanceTo(buffer)));
}

int Utils::GetRangedInt(const int min, const int max)
{
	if (min < max)
		return rand() % (1 + (max - min)) + (max - (max - min));

	return min;
}
