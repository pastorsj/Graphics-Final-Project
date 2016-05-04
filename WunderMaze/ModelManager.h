#ifndef __MODELMANAGER_H
#define __MODELMANAGER_H

#pragma once
#include "MazeGenerator.h"
#include "Model.h"
#include "ConfigFileLoader.h"
//#include "glm/glm.hpp"
#include <vector>
#include <iostream>
using namespace std;

class ModelManager
{
private:
	// CHANGE THIS TO CHANGE MAZE SIZE
	int DESIRED_X_SIZE = 7;
	int DESIRED_Y_SIZE = 7;
	int seed = -1;

public:
	ModelManager();
	~ModelManager();

	void addTexture(GLuint newTextureId);

	void init();

	int getNumObjects();

	glm::vec2 getRandomCell();

	vector<GLfloat> const getPosition() const;

	vector<GLfloat> const getNormal() const;

	vector<GLuint> const getElement() const;

	vector<GLfloat> const getTexCoord() const;

	size_t getPositionBytes() const;

	size_t getElementBytes() const;

	size_t getTexCoordBytes() const;

	size_t getNormalBytes() const;

	void draw(GLuint shaderProg, glm::mat4 mR);

	vector<Model*> & getRawModels();

	MazeGenerator& getMazeGenerator();

	void setSeed(int seed);

private:

	bool checkDuplicate(glm::vec2 toCheck);

	vector<Model*> models;
	vector<GLuint> textures;
	vector<glm::vec2> modelLocations;
	MazeGenerator mg;
};
#endif