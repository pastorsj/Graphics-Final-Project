#pragma once
#include "Model.h"
class Floor :
	public Model
{
public:
	Floor();
	~Floor();
	void init(MazeGenerator& mazeGen, string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset) override;
};

