#pragma once
#include <cmath>
#include <cstdint>

class Vector2
{
public:
	Vector2() = default;
	Vector2(const float _x, const float _y) : x(_x), y(_y) {}

	float  operator[] (const size_t idx) const { return (&x)[idx]; }
	float& operator[] (const size_t idx) { return (&x)[idx]; }

	Vector2& operator=(const Vector2& other) = default;

	Vector2 operator*(const float multiplier) const
	{
		return Vector2(x * multiplier, y * multiplier);
	}

	Vector2 operator/(const float divisor) const
	{
		return *this * (1.f / divisor);
	}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
	
	Vector2& operator+=(const Vector2 other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2& operator-=(const Vector2 other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	[[nodiscard]] float Length() const
	{
		return sqrt(x * x + y * y);
	}

	[[nodiscard]] float DistanceTo(const Vector2& other) const
	{
		return (*this - other).Length();
	}

	float x, y;
};

class Vector3
{
public:
	Vector3() = default;
	Vector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

	float  operator[] (const size_t idx) const { return (&x)[idx]; }
	float& operator[] (const size_t idx) { return (&x)[idx]; }

	Vector3& operator=(const Vector3& other) = default;

	Vector3 operator*(const float multiplier) const
	{
		return Vector3(x * multiplier, y * multiplier, z * multiplier);
	}

	Vector3 operator/(const float divisor) const
	{
		return *this * (1.f / divisor);
	}
	
	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3& operator+=(const Vector3 other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}
	
	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3& operator-=(const Vector3 other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	[[nodiscard]] float Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	
	[[nodiscard]] float DistanceTo(const Vector3& other) const
	{
		return (*this - other).Length();
	}

	float x, y, z;
};

class Utils final {
public:
	static void ValidateRgb(float* value);
	static int GetRangedInt(int min, int max);

	static bool Valid(std::uintptr_t ptr);

	static float GetDegrees(const Vector3& src, const Vector3& forward, const Vector3& origin);
	static Vector3 ProjectView(const Vector3& forward, const Vector3& origin, float distance);

private:
	static float RadiansToDegrees(float radians);

	virtual void Dummy() = 0;
};
