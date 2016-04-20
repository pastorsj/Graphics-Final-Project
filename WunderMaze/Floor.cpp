#include "Floor.h"



Floor::Floor(int xSize, int ySize)
{
	this->xSize = xSize;
	this->ySize = ySize;
}


Floor::~Floor()
{
}

void Floor::init(string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset)
{
	immune = 0;
	preTrans = glm::mat4(1.0f);
	this->texNum = texNum;

	positions.push_back(glm::vec3(-0.5, -0.5, -1));
	texCoords.push_back(glm::vec2(0, 1));
	normals.push_back(glm::vec3(.0, 1.0, .0));

	positions.push_back(glm::vec3(0.5, -0.5, -1));
	texCoords.push_back(glm::vec2(1, 1));
	normals.push_back(glm::vec3(.0, 1.0, .0));

	positions.push_back(glm::vec3(0.5, -0.5, 0));
	texCoords.push_back(glm::vec2(1, 0));
	normals.push_back(glm::vec3(.0, 1.0, .0));

	positions.push_back(glm::vec3(-0.5, -0.5, 0));
	texCoords.push_back(glm::vec2(0, 0));
	normals.push_back(glm::vec3(.0, 1.0, .0));

	positions.push_back(glm::vec3(0, -0.5, -0.5));
	texCoords.push_back(glm::vec2(0.5, 0.5));
	normals.push_back(glm::vec3(.0, 1.0, .0));


	elements.push_back(4);
	elements.push_back(1);
	elements.push_back(0);

	elements.push_back(4);
	elements.push_back(2);
	elements.push_back(1);

	elements.push_back(4);
	elements.push_back(3);
	elements.push_back(2);

	elements.push_back(4);
	elements.push_back(0);
	elements.push_back(3);

	for (int i = 1; i < this->xSize; ++i)
	{
		for (int j = 1; j < this->ySize; ++j)
		{
			glm::mat4 buildTrans = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));
			if (j > 1 && i < this->xSize - 1)
			{
				postTrans.push_back(buildTrans);
			}
		}
	}
}
