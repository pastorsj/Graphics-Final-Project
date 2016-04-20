#pragma once
#include "Model.h"
class Sky :
	public Model
{
public:
	Sky(int xSize, int ySize);
	~Sky();
	void init(string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset) override;

private:
	int xSize, ySize; //Size of the maze in cells, x/y
};