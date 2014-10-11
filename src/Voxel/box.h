
class BoxIsosurface : public Isosurface {
public:
    BoxIsosurface(const Vector &size);

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
    Vector _length2;

    Isosurface::Material _mat;
};
