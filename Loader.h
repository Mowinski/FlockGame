#pragma once
#include "CPR_Framework.h"
#include <map>

class Loader {
public:
    Loader();
    ~Loader();

    Mesh* GetMesh(std::string key);
protected:
    std::map<std::string, Mesh*> meshes;
};
