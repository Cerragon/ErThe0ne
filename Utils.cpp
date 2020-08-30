#include "utils.h"
#include "app.h"

int Utils::GetTextLength(const char* text)
{
	if (text == nullptr)
		return 0;

	for (auto i = 0; i < 0x7FFF; i++)
	{
		if (text[i] == '\0')
			return i;
	}

	return 0;
}

void Utils::ValidateInt(int& value, const int min, const int max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
}

void Utils::ValidateFloat(float& value, const float min, const float max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
}

void Utils::ValidateRgb(float* value)
{
	for (auto i = 0; i < 3; i++)
	{
		ValidateFloat(value[i], 0.0f, 1.0f);
	}
}

bool Utils::Valid(const DWORD64 ptr)
{
	if (ptr < 0x10000UL || ptr > 0xF000000000000ULL || ptr & 1)
		return false;

	return true;
}

void Utils::ProjectView(Vector3& dst, const Vector3& forward, const Vector3& origin, const float distance)
{
	dst[0] = origin[0] + forward[0] * distance;
	dst[1] = origin[1] + forward[1] * distance;
	dst[2] = origin[2] + forward[2] * distance;
}

float Utils::RadiansToDegrees(const float radians)
{
	return radians * (180.0f / 3.14159265f);
}

float Utils::GetDegrees(Vector3& src, Vector3& forward, Vector3& origin)
{
	Vector3 buffer;
	ProjectView(buffer, forward, origin, src.DistanceTo(origin));
	return RadiansToDegrees(sinf(src.DistanceTo(buffer) / origin.DistanceTo(buffer)));
}

bool Utils::WorldToScreen(const float* view, const Vector3& position, ImVec2& screenPos)
{
	float buffer[4];
	buffer[0] = view[0] * position[0] + -view[1] * -position[1] + view[2] * position[2] + view[3];
	buffer[1] = view[4] * position[0] + -view[5] * -position[1] + view[6] * position[2] + view[7];
	//Buffer[2] = View[8] * Position[0] + -View[9] * -Position[1] + View[10] * Position[2] + View[11];
	buffer[3] = view[12] * position[0] + -view[13] * -position[1] + view[14] * position[2] + view[15];
	if (buffer[3] < 0.1f)
		return false;

	float halfWindowSize[2];
	auto [width, height] = gApp->appWindow->GetSize();
	halfWindowSize[0] = static_cast<float>(width) * 0.5f;
	halfWindowSize[1] = static_cast<float>(height) * 0.5f;

	screenPos.x = halfWindowSize[0] + halfWindowSize[0] * buffer[0] / buffer[3];
	screenPos.y = halfWindowSize[1] - halfWindowSize[1] * buffer[1] / buffer[3];
	return true;
}

int Utils::GetRangedInt(const int min, const int max)
{
	if (min < max)
		return rand() % (1 + (max - min)) + (max - (max - min));

	return min;
}
