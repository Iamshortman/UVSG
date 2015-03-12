#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "glmInclude.hpp"

class Transform
{
    public:
        Transform();
        ~Transform();

        //Moves position by amount.
        void translate(const vector3& vec);
        void translate(const float&  x, const float&  y, const float&  z);

        //Sets the position of the transform.
        void setPosition(const vector3& vec);
        void setPosition(const float&  x, const float&  y, const float&  z);

        //returns the position.
        const vector3 getPosition();

        //Turns the object
        void rotate(const vector3& axis, const float& angle);
        void rotate(const float&  x, const float&  y, const float&  z, const float& angle);

        void setRotation();

        //Sets the Roatation of the object
        const quat getRotation();

        //Sets the scale of the transform.
        void setScale(const vector3& vec);
        void setScale(const float&  x, const float&  y, const float&  z);

        //returns the scale.
        const vector3 getScale();

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
        quat rotation;

        //the scale in each axis.
        vector3 scale;

};

#endif // TRANSFORM_HPP
