
#include "threed.h"
#include "box.h"

//----------------------------------------------------------------------------

BoxIsosurface::BoxIsosurface(const Vector &size)
{
    Vector length = size / 2.0f;

    _center = Vector();
    _length2 = length * length;

    Vector corner1 = _center - length;
    Vector corner2 = _center + length;
    Isosurface::addBoundingBox(
        BoundingBox(corner1, corner2));

    _mat.color = Color(0.0f, 1.0f, 0.0f);  // green
    _mat.ambient = Color();
    _mat.diffuse = 1.0f;
    _mat.specular = 1.0f;
    _mat.brilliance = 1.0f;

    Isosurface::setTransform(Transform());
}

//----------------------------------------------------------------------------

double BoxIsosurface::calcDensity(float xt, float yt, float zt)
{
    xt -= _center.x();
    yt -= _center.y();
    zt -= _center.z();

    double xd = (xt * xt) - _length2.x();
    double yd = (yt * yt) - _length2.y();
    double zd = (zt * zt) - _length2.z();
    double d = THREED_MAX(zd, THREED_MAX(xd, yd));

    return d;
}

//----------------------------------------------------------------------------

void BoxIsosurface::fDensity(
    float x0, float y0, float z0,
    float dz, int num_points, float *densities)
{
    for (int i = 0; i < num_points; ++i) {
        float xt, yt, zt;
        _globalTransInv.transform(x0, y0, z0, &xt, &yt, &zt);

        densities[i] = calcDensity(xt, yt, zt);

        z0 += dz;
    }
}

//----------------------------------------------------------------------------

void BoxIsosurface::fNormal(
    const Vector *point, Vector *normal)
{
    float xt, yt, zt;
    _globalTransInv.transform(
        point->x(), point->y(), point->z(),
        &xt, &yt, &zt);

    double d = calcDensity(xt, yt, zt);
    float nx = calcDensity(xt + 0.001f, yt, zt) - d;
    float ny = calcDensity(xt, yt + 0.001f, zt) - d;
    float nz = calcDensity(xt, yt, zt + 0.001f) - d;

    _globalTransInv.transformNormal(nx, ny, nz, &nx, &ny, &nz);
    *normal = Vector(nx, ny, nz).normalized();
}

//----------------------------------------------------------------------------

const Isosurface::Material &BoxIsosurface::fMaterial(
    const Vector *point, float density)
{
    return _mat;
}
