#pragma once
#include "Model.h"
class Wall :
	public Model
{
public:
	Wall();
	~Wall();
	void init(MazeGenerator& mazeGen, string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset) override;
};

