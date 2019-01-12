#include "City.h"

#include <fstream>
#include <sstream>
#include <algorithm>

City::City(std::string _filename) : filename(_filename)
{
}


City::~City()
{
}

bool City::OnInit()
{
    if (!LoadCityFile()) { return false; }
    buildingsInRow = matrixOfCity[0].size();
    buildingsInCol = matrixOfCity.size();
    const float mapWidthX = buildingsInRow * 7.0f + 3.0f;
    const float mapWidthZ = buildingsInCol * 7.0f + 3.0f;

    float x = -0.5f * mapWidthX + 2.0f + cityBorderX;
    float z = -0.5f * mapWidthZ + 2.0f + cityBorderZ;

    for (auto row : matrixOfCity) {
        for(auto it = row.begin(); it != row.end(); ++it, x += spaceBetweenBuilding + buildingSize) {
            if (*it < 1.0f) { continue; } // @TODO Remove it!
            std::shared_ptr<Building> building = std::make_shared<Building>(D3DXVECTOR3{ x, 0.0f, z }, *it);
            if (!building->OnInit()) { return false; }
            buildings.push_back(building);
        }
        z += spaceBetweenBuilding + buildingSize;
        x = -0.5f * mapWidthX + 2.0f + cityBorderX;
    }
    return true;
}

void City::OnUpdate(float deltaTime)
{
}

void City::OnRender()
{
    for (auto building : buildings) {
        building->OnRender();
    }
}

D3DXVECTOR3 City::GetPosition() const
{
    return D3DXVECTOR3();
}

BuildingVector City::getBuildingListNear(D3DXVECTOR3 point, float range) const
{
    BuildingVector vector{};

    for (auto building : buildings) {
        if (building->getDistanceBetweenCenterAndPoint(point) <= range) {
            vector.push_back(building);
        }
    }
    return vector;
}

bool City::isCollideWithAnyBuilding(const D3DXVECTOR3& point, float range) const
{
    BuildingVector buildings = getBuildingListNear(point, range);
    auto collideCmp = [point](std::shared_ptr<Building> b) {return b->isCollide(point); };
    return std::any_of(buildings.begin(), buildings.end(), collideCmp);
}

float City::getMapWidth() const
{
    return buildingsInRow * 4.0f + (buildingsInRow-1) * 3.0f + 2.0f * cityBorderX;
}

float City::getMapHeight() const
{
    return buildingsInCol * 4.0f + (buildingsInCol - 1) * 3.0f + 2.0f * cityBorderZ;
}

bool City::isCollideWithAnyBuilding(std::shared_ptr<NavMeshItem> item, float range) const
{
    BuildingVector buildings = getBuildingListNear(item->position, range);
    auto collideCmp = [item](std::shared_ptr<Building> b) {return b->isCollide(item->collisionBox); };

    return std::any_of(buildings.begin(), buildings.end(), collideCmp);
}


bool City::LoadCityFile()
{
    std::ifstream in{ filename };
    if (!in) { return false; }

    std::string line;
    while (std::getline(in, line)) {
        if (line[0] == '/' && line[1] == '/') { continue; }
        if (line.size() <= 0) { continue; }

        std::stringstream ss{ line };
        std::vector<int> row{};
        while (ss) {
            std::string num;
            if (!std::getline(ss, num, ',')) { break; }

            row.push_back(std::stoi(num));
        }

        matrixOfCity.push_back(row);
    }

    return true;
}
