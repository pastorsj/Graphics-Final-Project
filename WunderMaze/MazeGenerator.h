//Purpy Pupple's amazing maze generator.
//Released under the CC-BY-SA 3.0 License and the GFDL
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

struct cell {
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

class MazeGenerator {
private:

	std::vector<std::vector<cell>> maze;
	int xsize, ysize;

public:

	MazeGenerator();

	int getXSize();

	int getYSize();

	cell getCell(int xCoord, int yCoord);

	int makeMaze(int xsizetemp, int ysizetemp, int seedOverride);

	void initialize(int xsizetemp, int ysizetemp);

	void generate();
};