#include "Sky.h"



Sky::Sky(int xSize, int ySize)
{
	this->xSize = xSize;
	this->ySize = ySize;
}


Sky::~Sky()
{
}

void Sky::init(string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset)
{
	immune = 0;
	preTrans = glm::mat4(1.0f);
	this->texNum = texNum;

	immune = 1;

	positions.push_back(glm::vec3(this->xSize / 2, 0, 0));
	texCoords.push_back(glm::vec2(1, 0.8));
	normals.push_back(glm::vec3(.0, .0, 1.0));

	positions.push_back(glm::vec3(-this->xSize / 2, 0, 0));
	texCoords.push_back(glm::vec2(0, 0.8));
	normals.push_back(glm::vec3(.0, .0, 1.0));

	positions.push_back(glm::vec3(this->xSize / 2, 40, 0));
	texCoords.push_back(glm::vec2(1, 0));
	normals.push_back(glm::vec3(.0, .0, 1.0));

	positions.push_back(glm::vec3(-this->xSize / 2, 40, 0));
	texCoords.push_back(glm::vec2(0, 0));
	normals.push_back(glm::vec3(.0, .0, 1.0));

	elements.push_back(1);
	elements.push_back(0);
	elements.push_back(2);

	elements.push_back(1);
	elements.push_back(2);
	elements.push_back(3);

	preTrans = glm::mat4(1.0f);
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0, 1, 0));
	postTrans.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(this->xSize / 2, 0, 0)));
	postTrans.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, this->ySize / 2)) * rot);
	postTrans.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(this->ySize / 2, 0, this->xSize)) * rot * rot);
	postTrans.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(this->ySize, 0, this->xSize / 2)) * rot * rot * rot);
}
