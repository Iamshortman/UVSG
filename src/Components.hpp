#ifndef COMPONETS_HPP
#define COMPONETS_HPP

#include "Mesh.hpp"
#include "TexturedMesh.hpp"
#include <vector>

class Transform
{
public:
	f64vec3 m_position;
	f64quat m_orientation;
	f64vec3 m_scale = f64vec3(1.0f); //Start m_scale with a default of 1.0f

	//Getters for the directional vectors.
	f64vec3 getPos() const { return m_position; };
	f64vec3 getForward() const { return m_orientation * f64vec3(0.0f, 0.0f, 1.0f); };
	f64vec3 getUp() const { return m_orientation * f64vec3(0.0f, 1.0f, 0.0f); };
	f64vec3 getRight() const { return m_orientation * f64vec3(-1.0f, 0.0f, 0.0f); };
	f64quat getOrientation() const { return m_orientation; };
	f64vec3 getScale() const { return m_scale; };

	void setPos(const f64vec3& vec){ m_position = vec; };
	void setOrientation(const f64quat& quat){ m_orientation = quat; }
	void setScale(const f64vec3& vec){ m_scale = vec; }

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

		vector3 position = m_position;
		vector3 scale = m_scale;

		m_positionMatrix = glm::translate(matrix4(1.0F), position);
		rotationMatrix = glm::toMat4(m_orientation);
		m_scaleMatrix = glm::scale(matrix4(1.0F), scale);

		return m_positionMatrix * rotationMatrix * m_scaleMatrix;
	};

	matrix3 getNormalMatrix() const
	{
		return glm::toMat3(m_orientation);
	};


	//Scales the transform up or down 
	//Normaly used for scaling down the planets in the distance;
	Transform getScaledTransform(double scale)
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
	f64vec3 linearVelocity;
	f64vec3 angularVelocity;
};

#endif // COMPONETS_HPP
