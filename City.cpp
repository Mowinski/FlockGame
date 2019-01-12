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

    float x = 0.0f, z = 0.0f;

    for (auto row : matrixOfCity) {
        for(auto it = row.begin(); it != row.end(); ++it, x += spaceBetweenBuilding + buildingSize) {
            if (*it < 1.0f) { continue; }
            std::shared_ptr<Building> building = std::make_shared<Building>(D3DXVECTOR3{ x, 0.0f, z }, *it);
            building->OnInit();
            buildings.push_back(building);
        }
        z += spaceBetweenBuilding + buildingSize;
        x = 0.0f;
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
