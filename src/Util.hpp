#ifndef UTIL_HPP
#define UTIL_HPP

#include "glmInclude.hpp"
#include "PhysicsWorld.hpp"

using namespace std;

//Just a basic util file for some common functions

void printQuat(const quaternion quat);
void printVec(const vector3 vec);
void printEndLine();

quaternion fromAxes(const vector3 forward, const vector3 up);

btVector3 toBtVec3(const vector3& vec);
vector3 toGlmVec3(const btVector3& vec);
btQuaternion toBtQuat(const quaternion& quat);
quaternion toGlmQuat(const btQuaternion& quat);

float toRad(float degree);

#endif //UTIL_HPP