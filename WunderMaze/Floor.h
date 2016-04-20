#pragma once
#include "Model.h"
class Floor :
	public Model
{
public:
	Floor(int xSize, int ySize);
	~Floor();
	void init(string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset) override;

private:
	int xSize, ySize; //Size of the maze in cells, x/y
};