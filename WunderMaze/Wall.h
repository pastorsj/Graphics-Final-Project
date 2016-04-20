#pragma once
#include "Model.h"
class Wall :
	public Model
{
public:
	Wall(MazeGenerator& mg);
	~Wall();
	void init(string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset) override;

private:
	MazeGenerator* mazeGen;
};