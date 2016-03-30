#include "ModelManager.h"

ModelManager::ModelManager()
{
}

void ModelManager::addTexture(GLuint newTextureId)
{
	textures.push_back(newTextureId);
}

MazeGenerator& ModelManager::getMazeGenerator() {
	return this->mg;
}

void ModelManager::init()
{
	mg.makeMaze(DESIRED_X_SIZE, DESIRED_Y_SIZE);
	models.resize(8);
	models[0].init(mg, string("wall"), 0);
	models[1].init(mg, string("floor"), 1);
	glm::vec2 modelCell = this->getRandomCell();
	models[2].init(mg, string("resources/teapot.obj"), 2, modelCell.x, modelCell.y, true, -0.25);
	modelCell = this->getRandomCell();
	models[3].init(mg, string("resources/gourd.obj"), 2, modelCell.x, modelCell.y, true);
	modelCell = this->getRandomCell();
	models[4].init(mg, string("resources/sphere.obj"), 2, modelCell.x, modelCell.y, true);
	modelCell = this->getRandomCell();
	models[5].init(mg, string("resources/teddy.obj"), 2, modelCell.x, modelCell.y, true);
	modelCell = this->getRandomCell();
	models[6].init(mg, string("resources/cow.obj"), 2, modelCell.x, modelCell.y, false);
	models[7].init(mg, string("sky"), 4);
}

int ModelManager::getNumObjects() {
	return models.size() - 3;
}

glm::vec2 ModelManager::getRandomCell() {
	glm::vec2 toCheck;
	do {
		toCheck.x = rand() % (DESIRED_X_SIZE - 4) + 2;
		toCheck.y = rand() % (DESIRED_X_SIZE - 4) + 2;
	} while (this->checkDuplicate(toCheck));
	modelLocations.push_back(toCheck);
	return toCheck;
}

bool ModelManager::checkDuplicate(glm::vec2 toCheck) {
	bool found = false;
	for (int i = 0; i < modelLocations.size(); ++i) {
		if (modelLocations[i].x == toCheck.x && modelLocations[i].y == toCheck.y) {
			found = true;
			break;
		}
	}
	return found;
}

vector<GLfloat> const ModelManager::getPosition() const
{
	vector<GLfloat> positions;
	vector<glm::vec3> modelPos;
	for (int i = 0; i < models.size(); ++i)
	{
		modelPos = models[i].getPosition();
		for (int j = 0; j < modelPos.size(); ++j)
		{
			positions.push_back(modelPos[j][0]);
			positions.push_back(modelPos[j][1]);
			positions.push_back(modelPos[j][2]);
		}
	}
	return positions;
}

vector<GLuint> const ModelManager::getElement() const
{
	vector<GLuint> elements;
	vector<GLuint> modelEles;
	for (int i = 0; i < models.size(); ++i)
	{
		modelEles = models[i].getElement();
		for (int j = 0; j < modelEles.size(); ++j)
		{
			elements.push_back(modelEles[j]);
		}
	}
	return elements;
}

vector<GLfloat> const ModelManager::getTexCoord() const
{
	vector<GLfloat> coords;
	vector<glm::vec2> modelCoords;
	for (int i = 0; i < models.size(); ++i)
	{
		modelCoords = models[i].getTexCoord();
		for (int j = 0; j < modelCoords.size(); ++j)
		{
			coords.push_back(modelCoords[j][0]);
			coords.push_back(modelCoords[j][1]);
		}
	}
	return coords;
}

size_t ModelManager::getPositionBytes() const
{
	size_t runningTotal = 0;
	for (int i = 0; i < models.size(); ++i)
	{
		runningTotal += models[i].getPositionBytes();
	}
	return runningTotal;
}

size_t ModelManager::getElementBytes() const
{
	size_t runningTotal = 0;
	for (int i = 0; i < models.size(); ++i)
	{
		runningTotal += models[i].getElementBytes();
	}
	return runningTotal;
}

size_t ModelManager::getTexCoordBytes() const
{
	size_t runningTotal = 0;
	for (int i = 0; i < models.size(); ++i)
	{
		runningTotal += models[i].getTexCoordBytes();
	}
	return runningTotal;
}

void ModelManager::draw(GLuint shaderProg, glm::mat4 mR)
{
	size_t prevElements = 0;
	size_t prevVertices = 0;

	for (int i = 0; i < models.size(); ++i)
	{
		models[i].draw(shaderProg, mR, prevElements, prevVertices, textures);
	}
}

vector<Model> & ModelManager::getRawModels()
{
	return models;
}