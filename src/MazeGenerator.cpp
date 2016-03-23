#include "MazeGenerator.h"

MazeGenerator * MazeGenerator::mg;

MazeGenerator::MazeGenerator() {

}

MazeGenerator * MazeGenerator::instance() {
	if (mg == NULL) {
		mg = new MazeGenerator;
	}
	return mg;
}

int MazeGenerator::getXSize() {
	return xsize;
}

int MazeGenerator::getYSize() {
	return ysize;
}

cell MazeGenerator::getCell(int xCoord, int yCoord) {
	return maze[xCoord][yCoord];
}

int MazeGenerator::makeMaze(int xsizetemp, int ysizetemp) {
	srand((unsigned int)time(NULL)); //seed random number generator with system time
	initialize(xsizetemp, ysizetemp);      //initialize the maze
	generate();        //generate the maze
	return 0;
}


void MazeGenerator::initialize(int xsizetemp, int ysizetemp) {
	this->xsize = xsizetemp;
	this->ysize = ysizetemp;

	maze.resize(xsize);
	for (int i = 0; i < xsize; ++i) {
		maze[i].resize(ysize);
	}

	//Initialize the maze!
	for (int x = 0; x < xsize; x++) {
		for (int y = 0; y < ysize; y++) {
			//The maze cells on the edges of the maze are "in" to provide padding. Otherwise, all maze cells are not in.
			maze[x][y].in = (x == 0 || x == xsize - 1 || y == 0 || y == ysize - 1) ? 1 : 0;
			//All maze cells have all walls existing by default, except the perimeter cells.
			maze[x][y].up = (x == 0 || x == xsize - 1 || y == 0) ? 0 : 1;
			maze[x][y].left = (x == 0 || y == 0 || y == ysize - 1) ? 0 : 1;
		}
	}
	return;
}

void MazeGenerator::generate() {
	const int UP = 0;
	const int DOWN = 1;
	const int LEFT = 2;
	const int RIGHT = 3;
	long numin = 1;     //Number of cells in the maze.
	int xcur = 1, ycur = 1;//start growing from the corner. It could theoretically start growing from anywhere, doesn't matter.
	maze[xcur][ycur].in = 1;
	int whichway;
	bool success;
	do {
		do {
			//randomly find a cell that's in the maze
			xcur = rand() % (xsize - 2) + 1;
			ycur = rand() % (ysize - 2) + 1;
		} while (!maze[xcur][ycur].in ||
			(maze[xcur][ycur - 1].in && maze[xcur][ycur + 1].in && maze[xcur - 1][ycur].in && maze[xcur + 1][ycur].in));
		//
		do {
			//Randomly grow the maze if possible.
			success = 0;
			whichway = rand() % 4;
			switch (whichway) {
			case UP:
				if (!maze[xcur][ycur - 1].in) {
					success = 1;
					maze[xcur][ycur].up = 0;
					maze[xcur][ycur - 1].prevx = xcur;
					maze[xcur][ycur - 1].prevy = ycur;
					ycur--;
				}
				break;
			case DOWN:
				if (!maze[xcur][ycur + 1].in) {
					success = 1;
					maze[xcur][ycur + 1].up = 0;
					maze[xcur][ycur + 1].prevx = xcur;
					maze[xcur][ycur + 1].prevy = ycur;
					ycur++;
				}
				break;
			case LEFT:
				if (!maze[xcur - 1][ycur].in) {
					success = 1;
					maze[xcur][ycur].left = 0;
					maze[xcur - 1][ycur].prevx = xcur;
					maze[xcur - 1][ycur].prevy = ycur;
					xcur--;
				}
				break;
			case RIGHT:
				if (!maze[xcur + 1][ycur].in) {
					success = 1;
					maze[xcur + 1][ycur].left = 0;
					maze[xcur + 1][ycur].prevx = xcur;
					maze[xcur + 1][ycur].prevy = ycur;
					xcur++;
				}
				break;
			}
		} while (!success);
		maze[xcur][ycur].in = 1;
		numin++; //Every iteration of this loop, one maze cell is added to the maze.
	} while (numin < (xsize - 2)*(ysize - 2));
	return;
}