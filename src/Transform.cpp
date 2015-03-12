#include "Transform.hpp"

Transform::Transform()
{
    position = vector3();

    rotation = quat(0.0F, 0.0F, 1.0F, 0.0F);

    scale = vector3(1.0F, 1.0F, 1.0F);

}

Transform::~Transform()
{
    //dtor
}

void Transform::translate(const vector3& vec)
{
    position += vec;
}

void Transform::translate(const float& x, const float& y, const float& z)
{
    translate(vector3(x, y, z));
}

void Transform::setPosition(const vector3& vec)
{
    position = vec;
}

void Transform::setPosition(const float& x, const float& y, const float& z)
{
    setPosition(vector3(x, y, z));
}

const vector3 Transform::getPosition()
{
    return position;
}

void Transform::rotate(const vector3& axis, const float& angle)
{
    quat rot = glm::angleAxis(angle, axis);
    rotation *= rot;
    rotation = glm::normalize(rotation);
}

void Transform::rotate(const float&  x, const float&  y, const float&  z, const float& angle)
{
    rotate(vector3(x, y, z), angle);
}

const quat Transform::getRotation()
{
    return rotation;
}

void Transform::setScale(const vector3& vec)
{
    scale = vec;
}

void Transform::setScale(const float& x, const float& y, const float& z)
{
    setScale(vector3(x, y, z));
}

const vector3 Transform::getScale()
{
    return scale;
}

matrix4 Transform::getModelMatrix()
{
    matrix4 PositionMatrix = glm::translate(matrix4(1.0F), position);
    matrix4 RotationMatrix = matrix4(); //glm::toMat4(orientation);
    matrix4 ScaleMatrix = glm::scale(matrix4(1.0F), scale);


    return PositionMatrix * RotationMatrix * ScaleMatrix;
}
