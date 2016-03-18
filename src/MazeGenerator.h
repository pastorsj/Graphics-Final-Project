//Purpy Pupple's amazing maze generator.
//Released under the CC-BY-SA 3.0 License and the GFDL

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// changes the size of the maze
#define xsize 20
#define ysize 20

long numin=1;     //Number of cells in the maze.

struct cell{
    bool in;  //Is this cell in the maze?
    bool up;  //Does the wall above this cell exist?
    bool left;//Does the wall to the left of this cell exist?
    int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

class MazeGenerator {
private:

	cell MAZE[xsize][ysize];

	static MazeGenerator * mg;

	MazeGenerator() {

	}

public:

	static MazeGenerator * instance() {
		if (mg == NULL) {
			mg = new MazeGenerator;
		}
		return mg;
	}

	cell getCell(int xCoord, int yCoord) {
		return MAZE[xCoord][yCoord];
	}

	int makeMaze() {
		srand((unsigned int)time(NULL)); //seed random number generator with system time
		initialize();      //initialize the maze
		generate();        //generate the maze
		return 0;
	}


	void initialize() {
		//Initialize the maze!
		for (int x = 0; x < xsize; x++) {
			for (int y = 0; y < ysize; y++) {
				//The maze cells on the edges of the maze are "in" to provide padding. Otherwise, all maze cells are not in.
				MAZE[x][y].in = (x == 0 || x == xsize - 1 || y == 0 || y == ysize - 1) ? 1 : 0;
				//All maze cells have all walls existing by default, except the perimeter cells.
				MAZE[x][y].up = (x == 0 || x == xsize - 1 || y == 0) ? 0 : 1;
				MAZE[x][y].left = (x == 0 || y == 0 || y == ysize - 1) ? 0 : 1;
			}
		}
		return;
	}

	void generate() {
		const int UP = 0;
		const int DOWN = 1;
		const int LEFT = 2;
		const int RIGHT = 3;

		int xcur = 1, ycur = 1;//start growing from the corner. It could theoretically start growing from anywhere, doesn't matter.
		MAZE[xcur][ycur].in = 1;
		int whichway;
		bool success;
		do {
			do {
				//randomly find a cell that's in the maze
				xcur = rand() % (xsize - 2) + 1;
				ycur = rand() % (ysize - 2) + 1;
			} while (!MAZE[xcur][ycur].in ||
				(MAZE[xcur][ycur - 1].in && MAZE[xcur][ycur + 1].in && MAZE[xcur - 1][ycur].in && MAZE[xcur + 1][ycur].in));
			//
			do {
				//Randomly grow the maze if possible.
				success = 0;
				whichway = rand() % 4;
				switch (whichway) {
				case UP:
					if (!MAZE[xcur][ycur - 1].in) {
						success = 1;
						MAZE[xcur][ycur].up = 0;
						MAZE[xcur][ycur - 1].prevx = xcur;
						MAZE[xcur][ycur - 1].prevy = ycur;
						ycur--;
					}
					break;
				case DOWN:
					if (!MAZE[xcur][ycur + 1].in) {
						success = 1;
						MAZE[xcur][ycur + 1].up = 0;
						MAZE[xcur][ycur + 1].prevx = xcur;
						MAZE[xcur][ycur + 1].prevy = ycur;
						ycur++;
					}
					break;
				case LEFT:
					if (!MAZE[xcur - 1][ycur].in) {
						success = 1;
						MAZE[xcur][ycur].left = 0;
						MAZE[xcur - 1][ycur].prevx = xcur;
						MAZE[xcur - 1][ycur].prevy = ycur;
						xcur--;
					}
					break;
				case RIGHT:
					if (!MAZE[xcur + 1][ycur].in) {
						success = 1;
						MAZE[xcur + 1][ycur].left = 0;
						MAZE[xcur + 1][ycur].prevx = xcur;
						MAZE[xcur + 1][ycur].prevy = ycur;
						xcur++;
					}
					break;
				}
			} while (!success);
			MAZE[xcur][ycur].in = 1;
			numin++; //Every iteration of this loop, one maze cell is added to the maze.
		} while (numin < (xsize - 2)*(ysize - 2));
		return;
	}
};

MazeGenerator * MazeGenerator::mg;