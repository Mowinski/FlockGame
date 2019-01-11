#include "City.h"

#include <fstream>
#include <sstream>

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
        for (auto buildingHeight : row) {
            std::shared_ptr<Building> building = std::make_shared<Building>(D3DXVECTOR3{ x, 0.0f, z }, buildingHeight);
            building->OnInit();
            buildings.push_back(building);
            x += spaceBetweenBuilding + buildingSize;
        }
        z += spaceBetweenBuilding + buildingSize;
        x = 0.0f;
    }

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
