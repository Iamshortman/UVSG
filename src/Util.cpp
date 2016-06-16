#include "Util.hpp"

//This number flips the direction enum to the opposite one
int flipDirection(int direction)
{
	if (direction % 2)
	{
		//If number is odd sub 1
		return direction - 1;
	}
	else
	{
		//If number is even (or zero) add 1
		return direction + 1;
	}
}

vector3D getNormalFromDirectionVector3D(int direction)
{
	vector3D normals[] =
	{
		vector3D(0, 1, 0),
		vector3D(0, -1, 0),
		vector3D(0, 0, 1),
		vector3D(0, 0, -1),
		vector3D(1, 0, 0),
		vector3D(-1, 0, 0),
	};

	return normals[direction];
}

vector3B getNormalFromDirectionvector3B(int direction)
{
	vector3B normals[] =
	{
		vector3B(0, 1, 0),
		vector3B(0, -1, 0),
		vector3B(0, 0, 1),
		vector3B(0, 0, -1),
		vector3B(1, 0, 0),
		vector3B(-1, 0, 0),
	};

	return normals[direction];
}

void printQuat(const quaternionF quat)
{
	std::cout << "{" << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w << "}";
}

void printVec(const vector3F vec)
{
	std::cout << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
}

void printVec(const vector3B vec)
{
	std::cout << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
}

void printVec(const vector3D vec)
{
	std::cout << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
}

void printEndLine()
{
	std::cout << std::endl;
}

quaternionF fromAxes(const vector3F forward, const vector3F up)
{
	vector3F right = glm::normalize(glm::cross(forward, up));

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

btVector3 toBtVec3(const vector3D& vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

vector3D toGlmVec3(const btVector3& vec)
{
	return vector3D(vec.getX(), vec.getY(), vec.getZ());
}

btQuaternion toBtQuat(const quaternionD& quat)
{
	return btQuaternion(quat.x, quat.y, quat.z, quat.w);
}

quaternionD toGlmQuat(const btQuaternion& quat)
{
	return quaternionD(quat.getW(), quat.getX(), quat.getY(), quat.getZ());
}

float toRad(float degree)
{
	return degree * 0.0174532925f;
}

double toRad(double degree)
{
	return degree * 0.0174532925;
}

float toDeg(float rad)
{
	return rad * 57.295779513f;
}

double toDeg(double rad)
{
	return rad * 57.295779513;
}

int getPackedPos(vector3B pos)
{
	unsigned int x = pos.x;
	unsigned int y = pos.y;
	unsigned int z = pos.z;

	return (x & 0x000000FF) | ((y << 8) & 0x0000FF00) | ((z << 16) & 0x00FF0000);
}

vector3B getUnpackedPos(int packedPos)
{
	return vector3B((packedPos), (packedPos & 0x0000FF00) >> 8, (packedPos & 0x00FF0000) >> 16);
}