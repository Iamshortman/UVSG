#include "MeshComponent.hpp"

MeshComponent::MeshComponent(GameObject* object, Mesh* meshObject)
:Component(object)
{
    mesh = meshObject;
}

void MeshComponent::update()
{

}

void MeshComponent::render()
{
    mesh->draw();
}

MeshComponent::~MeshComponent()
{
    //dtor
}
