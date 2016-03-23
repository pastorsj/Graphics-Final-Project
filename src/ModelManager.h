#ifndef __MODELMANAGER_H
#define __MODELMANAGER_H

#pragma once
#include "MazeGenerator.h"
#include "Model.h"
//#include "glm/glm.hpp"
#include <vector>
#include <iostream>
using namespace std;

class ModelManager
{
private:
	// CHANGE THIS TO CHANGE MAZE SIZE
	const int DESIRED_X_SIZE = 20;
	const int DESIRED_Y_SIZE = 20;

public:
	ModelManager();

	void addTexture(GLuint newTextureId);

	void init();

	int getNumObjects();

	int getRandomCell();

	vector<GLfloat> const getPosition() const;

	vector<GLuint> const getElement() const;

	vector<GLfloat> const getTexCoord() const;

	size_t getPositionBytes() const;

	size_t getElementBytes() const;

	size_t getTexCoordBytes() const;

	void draw(GLuint shaderProg, glm::mat4 mR);

	vector<Model> & getRawModels();

private:
	vector<Model> models;
	vector<GLuint> textures;
};
#endif