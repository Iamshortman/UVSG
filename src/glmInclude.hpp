#ifndef GLMINCLUDE_HPP
#define GLMINCLUDE_HPP
//Used to include GLM in correct order
#include <stdlib.h>
#include <iostream>

//GML needs this to not complain about rotations being deprecated
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

typedef glm::uvec3 vector3I;
typedef glm::vec4 vector4;
typedef glm::vec3 vector3;
typedef glm::vec2 vector2;
typedef glm::mat4 matrix4;
typedef glm::mat3 matrix3;
typedef glm::quat quaternion;

#endif
