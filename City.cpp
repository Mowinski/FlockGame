#include "City.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

City::City(std::string _filename) : filename(_filename)
{
}

bool City::onInit()
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
            if (!building->onInit()) { return false; }
            buildings.push_back(building);
        }
        z += spaceBetweenBuilding + buildingSize;
        x = -0.5f * mapWidthX + 2.0f + cityBorderX;
    }
    return true;
}

void City::onUpdate(float deltaTime)
{
}

void City::onRender()
{
    for (auto building : buildings) {
        building->onRender();
    }
}

BuildingVector City::getBuildingListNear(const D3DXVECTOR3& point, float range) const
{
    BuildingVector vector{};

    for (auto building : buildings) {
        if (building->getDistanceBetweenCenterAndPoint(point) <= range) {
            vector.push_back(building);
        }
    }
    return vector;
}

bool City::isCollideWithAnyBuilding(const std::shared_ptr<NavMeshItem>& item, float range) const
{
	BuildingVector buildings = getBuildingListNear(item->GetPosition(), range);
	auto collideCmp = [item](std::shared_ptr<Building> b) {return b->isCollide(item->collisionBox); };
	return std::any_of(buildings.begin(), buildings.end(), collideCmp);
}

bool City::isCollideWithAnyBuilding(const D3DXVECTOR3& point, float range) const
{
    BuildingVector buildings = getBuildingListNear(point, range);
    auto collideCmp = [point](std::shared_ptr<Building> b) {return b->isCollide(point); };
    return std::any_of(buildings.begin(), buildings.end(), collideCmp);
}

std::shared_ptr<Building> City::getBuildingActorIsColidingWith(const D3DXVECTOR3 & point, float range) const
{
	BuildingVector buildings = getBuildingListNear(point, range);
	auto collideCmp = [point](std::shared_ptr<Building> b) {return b->isCollide(point); };
	auto it = std::find_if(buildings.begin(), buildings.end(), collideCmp);
	if (it == buildings.end()) return nullptr;
	return *it;
}

bool City::LoadCityFile()
{
    std::ifstream file;
    file.open(filename);

    if (!file) { return false; }

    std::string line;
    std::string number;

    while (std::getline(file, line)) {
        std::vector<int> row;

        if (line[0] == '/' && line[1] == '/') { continue; }
        if (line.size() <= 0) { continue; }

        for (unsigned int i = 0; i < line.size(); ++i) {
            if (std::isdigit(line[i]) || line[i] == '.') {
                number += line[i];
            }

            else if (!number.empty()) {
                row.push_back(std::stoi(number));
                number.clear();
            }
        }
        row.push_back(std::stoi(number));
        matrixOfCity.push_back(row);
        number.clear();
    }
    return true;
}
