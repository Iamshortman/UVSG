#ifndef UTIL_HPP
#define UTIL_HPP

#include "GLM_Include.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Ship/Directions.hpp"

using namespace std;

//Just a basic util file for some common functions
int flipDirection(int direction);

vector3D getNormalFromDirectionVector3D(int direction);
vector3B getNormalFromDirectionvector3B(int direction);

void printQuat(const quaternionF quat);
void printVec(const vector3F vec);
void printVec(const vector3B vec);
void printVec(const vector3D vec);

void printEndLine();

quaternionF fromAxes(const vector3F forward, const vector3F up);

btVector3 toBtVec3(const vector3D& vec);
vector3D toGlmVec3(const btVector3& vec);
btQuaternion toBtQuat(const quaternionD& quat);
quaternionD toGlmQuat(const btQuaternion& quat);

float toRad(float degree);
double toRad(double degree);

float toDeg(float rad);
double toDeg(double rad);

int getPackedPos(vector3B pos);
vector3B getUnpackedPos(int packedPos);

#endif //UTIL_HPP