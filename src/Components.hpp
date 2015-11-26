#ifndef COMPONETS_HPP
#define COMPONETS_HPP

#include "entityxInclude.hpp"

#include "Mesh.hpp"
#include "TexturedMesh.hpp"
#include <vector>

class Transform
{
public:
	vector3 m_position;
	quaternion m_orientation;
	vector3 m_scale = vector3(1.0f); //Start m_scale with a default of 1.0f

	//Getters for the directional vectors.
	vector3 getPos() const { return m_position; };
	vector3 getForward() const { return m_orientation * vector3(0.0f, 0.0f, 1.0f); };
	vector3 getUp() const { return m_orientation * vector3(0.0f, 1.0f, 0.0f); };
	vector3 getRight() const { return m_orientation * vector3(-1.0f, 0.0f, 0.0f); };
	quaternion getOrientation() const { return m_orientation; };
	vector3 getScale() const { return m_scale; };

	void setPos(const vector3& vec){ m_position = vec; };
	void setOrientation(const quaternion& quat){ m_orientation = quat; }
	void setScale(const vector3& vec){ m_scale = vec; }

	void setTransform(const Transform& transform)
	{
		this->setPos(transform.getPos());
		this->setOrientation(transform.getOrientation());
		this->setScale(transform.getScale());
	};

	void setPositionAndRotationFromTransform(const Transform& transform)
	{
		this->setPos(transform.getPos());
		this->setOrientation(transform.getOrientation());
	};

	matrix4 getModleMatrix() const
	{
		matrix4 m_positionMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();
		matrix4 m_scaleMatrix = matrix4();

		m_positionMatrix = glm::translate(matrix4(1.0F), m_position);
		rotationMatrix = glm::toMat4(m_orientation);
		m_scaleMatrix = glm::scale(matrix4(1.0F), m_scale);

		return m_positionMatrix * rotationMatrix * m_scaleMatrix;
	};

	matrix3 getNormalMatrix() const
	{
		return glm::toMat3(m_orientation);
	};


	//Scales the transform up or down 
	//Normaly used for scaling down the planets in the distance;
	Transform getScaledTransform(float scale)
	{
		Transform tempTransform;
		tempTransform.setOrientation(m_orientation);
		tempTransform.setPos(m_position * scale);
		tempTransform.setScale(m_scale * scale);
		return tempTransform;
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
