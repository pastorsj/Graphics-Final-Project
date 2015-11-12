#ifndef __MODELMANAGER_H
#define __MODELMANAGER_H

#include "MazeGenerator.h"
#include "Model.h"
//#include "glm/glm.hpp"
#include <vector>
#include <iostream>
using namespace std;

class ModelManager
{
public:
	ModelManager()
	{
	}

	void addTexture(GLuint newTextureId)
	{
		textures.push_back(newTextureId);
	}

	void init()
	{
		makeMaze();
		models.resize(7);
		models[0].init(string("wall"), 0);
		models[1].init(string("floor"), 1);
		models[2].init(string("resources/teapot.obj"), 1, 4, 4, true, -0.25);
		models[3].init(string("resources/gourd.obj"), 2, 1, 7, true);
		models[4].init(string("resources/sphere.obj"), 3, 4, 6, true);
		models[5].init(string("resources/teddy.obj"), 3, 7, 1, true);
		models[6].init(string("resources/cow.obj"), 3, 5, 2, false);
	}

	vector<GLfloat> const getPosition() const
	{
		vector<GLfloat> positions;
		vector<glm::vec3> modelPos;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			modelPos = models[i].getPosition();
			for(int j = 0 ; j < modelPos.size() ; ++j)
			{
				positions.push_back(modelPos[j][0]);
				positions.push_back(modelPos[j][1]);
				positions.push_back(modelPos[j][2]);
			}
		}
		return positions;
	}

	vector<GLuint> const getElement() const
	{
		vector<GLuint> elements;
		vector<GLuint> modelEles;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			modelEles = models[i].getElement();
			for(int j = 0 ; j < modelEles.size() ; ++j)
			{
				elements.push_back(modelEles[j]);
			}
		}
		return elements;
	}

	vector<GLfloat> const getTexCoord() const
	{
		vector<GLfloat> coords;
		vector<glm::vec2> modelCoords;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			modelCoords = models[i].getTexCoord();
			for(int j = 0 ; j < modelCoords.size() ; ++j)
			{
				coords.push_back(modelCoords[j][0]);
				coords.push_back(modelCoords[j][1]);
			}
		}
		return coords;
	}

	size_t getPositionBytes() const
	{
		size_t runningTotal = 0;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			runningTotal += models[i].getPositionBytes();
		}
		return runningTotal;
	}

	size_t getElementBytes() const
	{
		size_t runningTotal = 0;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			runningTotal += models[i].getElementBytes();
		}
		return runningTotal;
	}

	size_t getTexCoordBytes() const
	{
		size_t runningTotal = 0;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			runningTotal += models[i].getTexCoordBytes();
		}
		return runningTotal;
	}

	void draw(GLuint shaderProg, glm::mat4 mR)
	{
		size_t prevElements = 0;
		size_t prevVertices = 0;

		for(int i = 0 ; i < models.size() ; ++i)
		{
			models[i].draw(shaderProg, mR, prevElements, prevVertices, textures);
		}
	}

	vector<Model> & getRawModels()
	{
		return models;
	}


private:
	vector<Model> models;
	vector<GLuint> textures;
	
};


#endif