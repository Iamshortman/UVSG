#ifndef MESHCOMPONENT_HPP
#define MESHCOMPONENT_HPP

#include "Component.hpp"
#include "Mesh.hpp"

class MeshComponent : public Component
{
    public:
        MeshComponent(GameObject* object, Mesh* mesh);
        virtual ~MeshComponent();
        virtual void render();
        virtual void update();

    protected:
    private:
        Mesh* mesh;
};

#endif // MESHCOMPONENT_HPP
