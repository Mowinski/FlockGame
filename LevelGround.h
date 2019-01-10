#pragma once
#include <vector>
#include <string>

#include "Renderable.h"
#include "CPR_Framework.h"

class LevelGround : public Renderable
{
public:
	LevelGround(std::string filename_);
	~LevelGround();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnRelease() override;
	/* Load city matrix from file. Skip every line which starts at comment chars '//' or empty line.
		Every line with data, should contains int number seperated by comma sign ','
		Return: true if file is valid and data is loaded, otherwise return false
	*/
	bool LoadCityFile();
private:
	std::string filename;
	std::vector<std::vector<int>> matrixOfCity;

	Mesh* ground;
};

