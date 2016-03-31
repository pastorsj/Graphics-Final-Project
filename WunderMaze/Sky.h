#pragma once
#include "Model.h"
class Sky :
	public Model
{
public:
	Sky();
	~Sky();
	void init(MazeGenerator& mazeGen, string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset) override;
};

