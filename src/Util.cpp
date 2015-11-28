#include "Util.hpp"

void printQuat(const quaternion quat)
{
	std::cout << "{ " << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w << "}";
}

void printVec(const vector3 vec)
{
	std::cout << "{ " << vec.x << ", " << vec.y << ", " << vec.z << "}";
}

void printEndLine()
{
	std::cout << std::endl;
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

	return glm::toQuat(rotationMatrix);
};

btVector3 toBtVec3(const vector3& vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

vector3 toGlmVec3(const btVector3& vec)
{
	return vector3(vec.getX(), vec.getY(), vec.getZ());
}

btQuaternion toBtQuat(const quaternion& quat)
{
	return btQuaternion(quat.x, quat.y, quat.z, quat.w);
}

quaternion toGlmQuat(const btQuaternion& quat)
{
	return quaternion(quat.getW(), quat.getX(), quat.getY(), quat.getZ());
}

float toRad(float degree)
{
	return degree * 0.0174532925f;
}

double toRad(double degree)
{
	return degree * 0.0174532925;
}