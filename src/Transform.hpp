#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glmInclude.hpp"

class Transform
{
    public:
        Transform();
        ~Transform();

        //Moves position by amount.
        void translate(const vector3& vec);
        void translate(float x, float y, float z);

        //Sets the position of the transform.
        void setPosition(const vector3& vec);
        void setPosition(float x, float y, float z);

        //Sets the scale of the transform.
        void setScale(const vector3& vec);
        void setScale(float x, float y, float z);

        //Gets the diffrent sides off the transform.
        vector3 getForward();
        vector3 getUp();
        vector3 getRight();

        matrix4 getModelMatrix();

    protected:
    private:
        //position of the object in the world.
        vector3 position;

        //rotation of the object, stored in rad;
        quat roatation;

        //the scale in each axis.
        vector3 scale;


};

#endif // TRANSFORM_H
