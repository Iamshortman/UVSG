#include "Transform.hpp"

Transform::Transform()
{
    position = vector3();

    //rotation = quaternion();

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

void Transform::translate(float x, float y, float z)
{
    translate(vector3(x, y, z));
}

void Transform::setPosition(const vector3& vec)
{
    position = vec;
}

void Transform::setPosition(float x, float y, float z)
{
    setPosition(vector3(x, y, z));
}

void Transform::setScale(const vector3& vec)
{
    scale = vec;
}

void Transform::setScale(float x, float y, float z)
{
    setScale(vector3(x, y, z));
}

matrix4 Transform::getModelMatrix()
{
    matrix4 PositionMatrix = glm::translate(matrix4(1.0F), position);
    matrix4 RotationMatrix = matrix4(); //glm::toMat4(orientation);
    matrix4 ScaleMatrix = glm::scale(matrix4(1.0F), scale);


    return PositionMatrix * RotationMatrix * ScaleMatrix;
}
