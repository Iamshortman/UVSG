#ifndef UTIL_HPP
#define UTIL_HPP

#include "glmInclude.hpp"

using namespace std;

//Just a basic util file for some common functions

void printQuat(const quaternion quat);
void printVec(const vector3 quat);
quaternion fromAxes(const vector3 forward, const vector3 up);



float toRad(float degree);

#endif //UTIL_HPP