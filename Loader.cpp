#include "Loader.h"


Loader::Loader()
{
    Mesh* teapot = LoadFromFile("resources/meshes/teapot.x");
    Mesh* unitbox = LoadFromFile("resources/meshes/unitbox.x");
    Mesh* unitcylinder = LoadFromFile("resources/meshes/unitcylinder.x");
    Mesh* unitsphere = LoadFromFile("resources/meshes/unitsphere.x");
    meshes.insert(std::pair<std::string, Mesh*>("teapot", teapot));
    meshes.insert(std::pair<std::string, Mesh*>("unitbox", unitbox));
    meshes.insert(std::pair<std::string, Mesh*>("unitcylinder", unitcylinder));
    meshes.insert(std::pair<std::string, Mesh*>("unitsphere", unitsphere));
}


Loader::~Loader()
{
    for (auto itr = meshes.begin(); itr != meshes.end(); ++itr) {
        Release(itr->second);
    }
}

Mesh* Loader::GetMesh(std::string key)
{
    return meshes[key];
}
