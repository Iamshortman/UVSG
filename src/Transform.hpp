#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glmInclude.hpp"

class Transform
{
    public:
        Transform();
        ~Transform();

        matrix4 getModelMatrix();

        void update();


        //Location Data in the world
        vector3 position;
        quaternion rotation;

        // Used to calculate next tick
        vector3 motion; //motion in each axis in meters/sec
        vector3 rotationMotion; //rotation in roations(PI/2 rad = 1 rotation)/second;

        //the scale in each axis before rotation.
        vector3 scale;

        //Data From prev tick, for rendering
        vector3 prev_position;
        quaternion prev_rotation;

        //usefull for moving objects relitive to facing
        vector3 forward;
        vector3 up;
        vector3 right;

    protected:
    private:
};

#endif // TRANSFORM_H
