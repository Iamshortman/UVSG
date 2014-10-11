//----------------------------------------------------------------------------
// ThreeD Transform
//----------------------------------------------------------------------------

#include "transform.h"
#include "opengl.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

#include <iostream>
using std::cout;
using std::endl;

//----------------------------------------------------------------------------

void Transform::glMultMatrix()
{
    glMultMatrixf((const float *)matrix());
}
