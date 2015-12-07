#ifndef COMPONETS_HPP
#define COMPONETS_HPP

#include "Rendering/Mesh.hpp"
#include "Rendering/TexturedMesh.hpp"
#include <vector>

class Transform
{
public:
	vector3D m_position;
	quaternionD m_orientation;
	vector3D m_scale = vector3D(1.0f); //Start m_scale with a default of 1.0f

	//Getters for the directional vectors.
	vector3D getPos() const { return m_position; };
	vector3D getForward() const { return m_orientation * vector3D(0.0f, 0.0f, 1.0f); };
	vector3D getUp() const { return m_orientation * vector3D(0.0f, 1.0f, 0.0f); };
	vector3D getRight() const { return m_orientation * vector3D(-1.0f, 0.0f, 0.0f); };
	quaternionD getOrientation() const { return m_orientation; };
	vector3D getScale() const { return m_scale; };

	void setPos(const vector3D& vec){ m_position = vec; };
	void setOrientation(const quaternionD& quat){ m_orientation = quat; }
	void setScale(const vector3D& vec){ m_scale = vec; }

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

		vector3F position = m_position;
		vector3F scale = m_scale;

		m_positionMatrix = glm::translate(matrix4(1.0F), position);
		rotationMatrix = glm::toMat4(m_orientation);
		m_scaleMatrix = glm::scale(matrix4(1.0F), scale);

		return m_positionMatrix * rotationMatrix * m_scaleMatrix;
	};

	matrix3 getNormalMatrix() const
	{
		return glm::toMat3(m_orientation);
	};
};

class Velocity
{
public:
	vector3D linearVelocity;
	vector3D angularVelocity;
};

#endif // COMPONETS_HPP
