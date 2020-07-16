#pragma once
#include <Windows.h>

class Utils final {
public:
	static void ValidateFloat(float& value, float min, float max);
	static void ValidateRgb(float* value);
	static void ValidateInt(int& value, int min, int max);
	static int GetRangedInt(int min, int max);

	static int GetTextLength(const char* text);

	static bool Valid(DWORD64 ptr);

	static float GetDistance(const float* a1, const float* a2);
	static float GetDegrees(float* src, float* forward, float* origin);
	static bool WorldToScreen(const float* view, const float* position, float* screen);
	static void ProjectView(float* dst, const float* forward, const float* origin, float distance);

private:
	static float RadiansToDegrees(float radians);

	virtual void Dummy() = 0;
};
