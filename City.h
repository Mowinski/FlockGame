#pragma once
#include "Building.h"
#include "NavMeshItem.h"
#include "Renderable.h"
#include <memory>
#include <vector>

using CityMatrix = std::vector<std::vector<int>>;
using BuildingVector = std::vector<std::shared_ptr<Building>>;

class City : public Renderable {
public:
    City(std::string _filename);
    ~City() = default;

    bool onInit();
    void onUpdate(float deltaTime) override;
    void onRender() override;

    BuildingVector getBuildingListNear(const D3DXVECTOR3& point, float range) const;
    bool isCollideWithAnyBuilding(const std::shared_ptr<NavMeshItem>& item, float range) const;
    bool isCollideWithAnyBuilding(const D3DXVECTOR3& point, float range) const;
	std::shared_ptr<Building> getBuildingActorIsColidingWith(const D3DXVECTOR3& point, float range) const;

	float getMapWidth() const { return buildingsInRow * 4.0f + (buildingsInRow - 1) * 3.0f + 2.0f * cityBorderX; };
	float getMapHeight() const { return buildingsInCol * 4.0f + (buildingsInCol - 1) * 3.0f + 2.0f * cityBorderZ; };

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
