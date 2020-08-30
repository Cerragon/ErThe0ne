#pragma once
#include <cmath>
#include <Windows.h>
#include "imgui/imgui.h"

class Vector3
{
public:
	Vector3() { x = y = z = 0.0f; }
	Vector3(const float _x, const float _y, const float _z) { x = _x; y = _y; z = _z; }
	
	float  operator[] (const size_t idx) const { return (&x)[idx]; }
	float& operator[] (const size_t idx) { return (&x)[idx]; }

	[[nodiscard]] float DistanceTo(const Vector3& other) const
	{
		const auto xd = other.x - x;
		const auto yd = other.y - y;
		const auto zd = other.z - z;
		return sqrtf(xd * xd + yd * yd + zd * zd);
	}

	float x, y, z;
};

class Utils final {
public:
	static void ValidateFloat(float& value, float min, float max);
	static void ValidateRgb(float* value);
	static void ValidateInt(int& value, int min, int max);
	static int GetRangedInt(int min, int max);

	static int GetTextLength(const char* text);

	static bool Valid(DWORD64 ptr);

	static float GetDegrees(Vector3& src, Vector3& forward, Vector3& origin);
	static bool WorldToScreen(const float* view, const Vector3& position, ImVec2& screenPos);
	static void ProjectView(Vector3& dst, const Vector3& forward, const Vector3& origin, float distance);

private:
	static float RadiansToDegrees(float radians);

	virtual void Dummy() = 0;
};
