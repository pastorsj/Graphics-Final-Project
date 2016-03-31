#include "Wall.h"



Wall::Wall()
{
}


Wall::~Wall()
{
}

void Wall::init(MazeGenerator& mazeGen, string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset)
{
	mg = mazeGen;
	immune = 0;
	preTrans = glm::mat4(1.0f);
	this->texNum = texNum;

	positions.push_back(glm::vec3(-0.5, -0.5, 0));
	texCoords.push_back(glm::vec2(0, 1));

	positions.push_back(glm::vec3(0.5, -0.5, 0));
	texCoords.push_back(glm::vec2(1, 1));

	positions.push_back(glm::vec3(0.5, 0.5, 0));
	texCoords.push_back(glm::vec2(1, 0));

	positions.push_back(glm::vec3(-0.5, 0.5, 0));
	texCoords.push_back(glm::vec2(0, 0));

	positions.push_back(glm::vec3(0, 0, 0));
	texCoords.push_back(glm::vec2(0.5, 0.5));

	elements.push_back(0);
	elements.push_back(1);
	elements.push_back(4);

	elements.push_back(1);
	elements.push_back(2);
	elements.push_back(4);

	elements.push_back(2);
	elements.push_back(3);
	elements.push_back(4);

	elements.push_back(3);
	elements.push_back(0);
	elements.push_back(4);

	makeDoubleSided();

	glm::mat4 leftWall = glm::translate(glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0, 1, 0)), glm::vec3(-0.5, 0, -0.5));
	for (int i = 1; i < mg.getXSize(); ++i)
	{
		for (int j = 1; j < mg.getYSize(); ++j)
		{
			glm::mat4 buildTrans = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));
			if (mg.getCell(i, j).up)
			{
				postTrans.push_back(buildTrans);
			}
			if (mg.getCell(i, j).left)
			{
				postTrans.push_back(buildTrans * leftWall);
			}
		}
	}
}
