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
	std::shared_ptr<Building> getBuildingActorIsColidingWith(const D3DXVECTOR3& point, float range) const;

    float getMapWidth() const;
    float getMapHeight() const;

protected:
    CityMatrix matrixOfCity;
    std::string filename;

    std::vector<std::shared_ptr<Building>> buildings;

    bool LoadCityFile();

    const float buildingSize = 4.0f;
    const float spaceBetweenBuilding = 3.0f;
    const float cityBorderX = 3.0f;
    const float cityBorderZ = 3.0f;
    int buildingsInRow;
    int buildingsInCol;
};

