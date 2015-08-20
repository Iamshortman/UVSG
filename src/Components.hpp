#ifndef COMPONETS_HPP
#define COMPONETS_HPP

#include "Mesh.hpp"
#include <btBulletDynamicsCommon.h>
#include <entityxInclude.hpp>

class Transform
{
public:
	vector3 position;
	quaternion orientation;
	
	//Getters for the directional vectors.
	const vector3 getPos(){ return position; };
	const vector3 getForward(){ return orientation * vector3(0.0f, 0.0f, 1.0f); };
	const vector3 getUp(){ return orientation * vector3(0.0f, 1.0f, 0.0f); };
	const vector3 getRight(){ return orientation * vector3(-1.0f, 0.0f, 0.0f); };

	const matrix4 getModleMatrix()
	{
		matrix4 positionMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();
		matrix4 scaleMatrix = matrix4();

		positionMatrix = glm::translate(matrix4(1.0F), position);
		rotationMatrix = glm::toMat4(orientation);
		//TODO SCALEMatrix

		return positionMatrix * rotationMatrix * scaleMatrix;
	};

	const matrix3 getNormalMatrix()
	{
		return glm::toMat3(orientation);
	};

};

class Velocity
{
public:
	vector3 linearVelocity;
	vector3 angularVelocity;
};

struct MeshComponent
{
	Mesh mesh;
	vector3 offset;
};

struct inputControl
{

};

struct CameraLock
{

};

struct DebugVelocity
{
	vector3 prevPos;
};

#endif // COMPONETS_HPP