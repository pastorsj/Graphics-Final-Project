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

	void init()
	{
		makeMaze();
		models.resize(3);
		models[0].init(string("wall"));
		models[1].init(string("floor"));
		models[2].init(string("resources/teapot.obj"));
	}

	vector<GLfloat> const getPosition() const
	{
		vector<GLfloat> positions;
		vector<GLfloat> modelPos;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			modelPos = models[i].getPosition();
			for(int j = 0 ; j < modelPos.size() ; ++j)
			{
				positions.push_back(modelPos[j]);
			}
		}
		return positions;
	}

	vector<GLfloat> const getColor() const
	{
		vector<GLfloat> colors;
		vector<GLfloat> modelColor;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			modelColor = models[i].getColor();
			for(int j = 0 ; j < modelColor.size() ; ++j)
			{
				colors.push_back(modelColor[j]);
			}
		}
		return colors;
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

	size_t getPositionBytes() const
	{
		size_t runningTotal = 0;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			runningTotal += models[i].getPositionBytes();
		}
		return runningTotal;
	}

	size_t getColorBytes() const
	{
		size_t runningTotal = 0;
		for(int i = 0 ; i < models.size() ; ++i)
		{
			runningTotal += models[i].getColorBytes();
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

	void draw(GLuint shaderProg, glm::mat4 mR)
	{
		size_t prevElements = 0;
		size_t prevVertices = 0;
		vector<glm::mat4> postTransList;
		glm::mat4 preTrans, postTrans;
		glm::mat4 ident = glm::mat4(1.0f);
		for(int i = 0 ; i < models.size() ; ++i)
		{
			preTrans = models[i].getPreTrans();
			postTransList = models[i].getPostTrans();
			glUniformMatrix4fv(glGetUniformLocation(shaderProg, "preTrans"), 1, GL_FALSE, &preTrans[0][0]);
			if(models[i].getRotate())
			{
				glUniformMatrix4fv(glGetUniformLocation(shaderProg, "mR"), 1, GL_FALSE, &mR[0][0]);
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(shaderProg, "mR"), 1, GL_FALSE, &ident[0][0]);
			}
			for(int j = 0 ; j < postTransList.size() ; ++j)
			{
				postTrans = postTransList[j];
				glUniformMatrix4fv(glGetUniformLocation(shaderProg, "postTrans"), 1, GL_FALSE, &postTrans[0][0]);
				//cout << models[i].getElement().size() << " " << prevElements << " " << prevVertices << endl;
				glDrawElementsBaseVertex(GL_TRIANGLES, models[i].getElement().size(), GL_UNSIGNED_INT, (void*)(prevElements*sizeof(GLfloat)), prevVertices);
			}
			prevElements += models[i].getElement().size();
			prevVertices += models[i].getVertexCount();
		}
	}

	vector<Model> & getRawModels()
	{
		return models;
	}


private:
	vector<Model> models;
	
};


#endif