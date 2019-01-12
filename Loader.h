#pragma once
#include <map>
#include "CPR_Framework.h"
class Loader {
public:
    Loader();
    ~Loader();

    Mesh* GetMesh(std::string key);
protected:
    std::map<std::string, Mesh*> meshes;
};

