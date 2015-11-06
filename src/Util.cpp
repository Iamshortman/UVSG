#include "Util.hpp"

void printQuat(const quaternion quat)
{
	std::cout << "{ " << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w << "} \n";
}

void printVec(const vector3 vec)
{
	std::cout << "{ " << vec.x << ", " << vec.y << ", " << vec.z << "} \n";
}

quaternion fromAxes(const vector3 forward, const vector3 up)
{
	vector3 right = glm::normalize(glm::cross(forward, up));

	matrix3 rotationMatrix = matrix3();

	rotationMatrix[0][0] = right.x;
	rotationMatrix[0][1] = right.y;
	rotationMatrix[0][2] = right.z;

	rotationMatrix[1][0] = up.x;
	rotationMatrix[1][1] = up.y;
	rotationMatrix[1][2] = up.z;

	rotationMatrix[2][0] = forward.x;
	rotationMatrix[2][1] = forward.y;
	rotationMatrix[2][2] = forward.z;

	return toQuat(rotationMatrix);
};

float toRad(float degree)
{
	return degree * 0.0174532925f;
}