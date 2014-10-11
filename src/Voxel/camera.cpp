//----------------------------------------------------------------------------
// ThreeD Camera
//----------------------------------------------------------------------------

#include "camera.h"
#include "world.h"
#include "opengl.h"

using namespace std;

//----------------------------------------------------------------------------

Camera::Camera(World &world) :
    Transform(),
    _world(world)
{
}

//----------------------------------------------------------------------------

void Camera::draw()
{
    //Matrix saveMatrix = _matrix();
    //invert();
    glLoadIdentity();
    Transform::glMultMatrix();
    _world.draw();

}

//----------------------------------------------------------------------------

WorldLink *Camera::getSelection(int x, int y)
{
    glLoadIdentity();
    Transform::glMultMatrix();
    WorldLink *link = _world.getSelection(x, y);
    return link;
}
