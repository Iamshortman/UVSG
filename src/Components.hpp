#ifndef COMPONETS_HPP
#define COMPONETS_HPP

#include "entityxInclude.hpp"

#include "Mesh.hpp"
#include "TexturedMesh.hpp"
#include <vector>

class Transform
{
public:
	vector3 position;
	quaternion orientation;
	vector3 scale = vector3(1.0f); //Star scale with a default of 1.0f

	//Getters for the directional vectors.
	const vector3 getPos(){ return position; };
	const vector3 getForward(){ return orientation * vector3(0.0f, 0.0f, 1.0f); };
	const vector3 getUp(){ return orientation * vector3(0.0f, 1.0f, 0.0f); };
	const vector3 getRight(){ return orientation * vector3(-1.0f, 0.0f, 0.0f); };
	const quaternion getOrientation(){ return orientation; };
	const vector3 getScale(){ return scale; };

	void setPos(const vector3& vec){ position = vec; };
	void setOrientation(const quaternion& quat){ orientation = quat; }
	void setScale(const vector3& vec){ scale = vec; }

	void setTransform(Transform& transform) 
	{
		setPos(transform.getPos());
		setOrientation(transform.getOrientation());
		setScale(transform.getScale());
	};

	const matrix4 getModleMatrix()
	{
		matrix4 positionMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();
		matrix4 scaleMatrix = matrix4();

		positionMatrix = glm::translate(matrix4(1.0F), position);
		rotationMatrix = glm::toMat4(orientation);
		scaleMatrix = glm::scale(matrix4(1.0F), scale);
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

struct ModelComponent
{
	std::vector<MeshComponent> meshes;
};

struct TexturedMeshComponent
{
	TexturedMesh mesh;
};

struct CameraLock
{

};

#endif // COMPONETS_HPP