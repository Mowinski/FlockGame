#include "LevelGround.h"

#include <fstream>
#include <sstream>


LevelGround::LevelGround(std::string filename_) : filename(filename_)
{
	ground = LoadFromFile("resources/meshes/unitbox.x");
}


LevelGround::~LevelGround()
{
}

void LevelGround::OnUpdate(float deltaTime)
{
}

void LevelGround::OnRender()
{
	Render(ground,
		   D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		   D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		   D3DXVECTOR3(100.0f, 0.10f, 100.0f),
		   D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
}

void LevelGround::OnRelease()
{
	Release(ground);
}


bool LevelGround::LoadCityFile()
{
	std::ifstream in(filename);
	if (!in) return false;

	std::string line;
	while (std::getline(in, line)) {
		if (line[0] == '/' && line[1] == '/') continue;
		if (line.size() <= 0) continue;

		std::stringstream ss(line);
		std::vector<int> row;
		while (ss) {
			std::string num;
			if (!std::getline(ss, num, ',')) break;

			row.push_back(std::stoi(num));
		}

		matrixOfCity.push_back(row);
	}

	return true;
}
