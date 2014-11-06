#include "Transform.hpp"

Transform::Transform()
{
    position = vector3();
    prev_position = vector3();

    rotation = quaternion();
    prev_rotation = quaternion();

    motion = vector3();
    rotationMotion = vector3();

    scale = vector3(1.0F, 1.0F, 1.0F);

    forward = vector3(0.0F, 0.0F, 1.0F);
    up = vector3(0.0F, 1.0F, 0.0F);
    right = vector3(1.0F, 0.0F, 0.0F);

}

Transform::~Transform()
{
    //dtor
}

void Transform::update()
{
     matrix4 RotationMatrix = glm::toMat4(rotation);
     //I have no idea why the right vec needs to be opposite.
     right = -glm::vec3(RotationMatrix[0][0], RotationMatrix[0][1], RotationMatrix[0][2]);
     up = glm::vec3(RotationMatrix[1][0], RotationMatrix[1][1], RotationMatrix[1][2]);
     forward = glm::vec3(RotationMatrix[2][0], RotationMatrix[2][1], RotationMatrix[2][2]);
}

matrix4 Transform::getModelMatrix()
{
    matrix4 PositionMatrix = glm::translate(matrix4(1.0F), position);
    matrix4 RotationMatrix = glm::toMat4(rotation);
    matrix4 ScaleMatrix = glm::scale(matrix4(1.0F), scale);


    return PositionMatrix * RotationMatrix * ScaleMatrix;
}
