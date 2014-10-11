#ifndef _THREED_SPHERE_H
#define _THREED_SPHERE_H

#include "vector.h"
#include "isosurface.h"


class SphereIsosurface : public Isosurface {
public:
    SphereIsosurface(float rad);

    inline double calcDensity(float xt, float yt, float zt);

    virtual void fDensity(
        float x0, float y0, float z0,
        float dz, int num_points, float *densities);

    virtual void fNormal(
        const Vector *point, Vector *normal);

    virtual const Material &fMaterial(
        const Vector *point, float density);

    /*
     * data
     */

    Vector _center;
    float _radius;

    Isosurface::Material _mat;
};

#endif
