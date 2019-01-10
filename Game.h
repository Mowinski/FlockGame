#pragma once
#include <memory>

#include "LevelGround.h"

class Game
{
public:
	Game();
	~Game();
	void Update(float deltaTime);
	void Render();
	bool Init(std::string filename);

private:
	std::shared_ptr<LevelGround> levelGround;
};

