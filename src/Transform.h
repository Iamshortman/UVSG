#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glmInclude.hpp"

class Transform
{
    public:
        Transform();
        ~Transform();
        vector3 position;
        vector3 motion;
        vector3 forward; //Used for Forward and Back movement
        vector3 vec_Up; //Used for Up and Down movement
        vector3 vec_Right; // Used for Left and Right movement

    protected:
    private:
};

#endif // TRANSFORM_H
