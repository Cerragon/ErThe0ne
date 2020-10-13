#include "Camera.h"

#include "../../app.h"

Vector2 Camera::World2Screen(const Vector3& position) const
{
	Vector2 result = {};
	
	const auto w = viewMatrix[3][0] * position.x + viewMatrix[3][1] * position.y + viewMatrix[3][2] * position.z + viewMatrix[3][3];

	if (w < 0.01f)
		return result;
	
	result.x = viewMatrix[0][0] * position.x + viewMatrix[0][1] * position.y + viewMatrix[0][2] * position.z + viewMatrix[0][3];
	result.y = viewMatrix[1][0] * position.x + viewMatrix[1][1] * position.y + viewMatrix[1][2] * position.z + viewMatrix[1][3];

	const auto invw = 1.0f / w;
	result.x *= invw;
	result.y *= invw;

	const auto windowSize = gApp->GetWindowSize();

	result.x = windowSize.x * 0.5f + result.x * windowSize.x * 0.5f;
	result.y = windowSize.y * 0.5f - result.y * windowSize.y * 0.5f;

	return result;
}
