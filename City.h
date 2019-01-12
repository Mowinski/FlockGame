#pragma once
#include <vector>
#include <memory>

#include "Renderable.h"
#include "Building.h"
#include "NavMeshItem.h"

using CityMatrix = std::vector<std::vector<int>>;
using BuildingVector = std::vector<std::shared_ptr<Building>>;

class City : public Renderable {
public:
    City(std::string _filename);
    ~City();


    bool OnInit();
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    D3DXVECTOR3 GetPosition() const override;

    BuildingVector getBuildingListNear(D3DXVECTOR3 point, float range) const;
    bool isCollideWithAnyBuilding(std::shared_ptr<NavMeshItem> item, float range) const;
    bool isCollideWithAnyBuilding(const D3DXVECTOR3& point, float range) const;

protected:
    CityMatrix matrixOfCity;
    std::string filename;

    std::vector<std::shared_ptr<Building>> buildings;

    /* Load city matrix from file. Skip every line which starts at comment chars '//' or empty line.
        Every line with data, should contains int number seperated by comma sign ','
        Return: true if file is valid and data is loaded, otherwise return false
    */
    bool LoadCityFile();

    const float buildingSize = 4.0f;
    const float spaceBetweenBuilding = 3.0f;
};

