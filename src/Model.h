#ifndef __MODEL
#define __MODEL

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "objload/objLoader.h"
using namespace std; //makes using vectors easy

class Model
{
public:

    void init(string objName)
    {
		if(strcmp(objName.c_str(), "wall") == 0)
		{
			initWall();
		}
		else if(strcmp(objName.c_str(), "floor") == 0)
		{
			initFloor();
		}
		else
		{
			// do something else..  probably will integrate objloader here
		}
	}
	
	vector<GLfloat> const getPosition() const
	{ return positions; }
	
	vector<GLfloat> const getColor() const
	{ return colors; }
	
	vector<GLuint> const getElement() const
	{ return elements; }
	
	size_t getVertexCount() const
	{ return positions.size()/3; }
	
	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }
	
	size_t getColorBytes() const
	{ return colors.size()*sizeof(GLfloat); }
	
	size_t getElementBytes() const
	{ return elements.size()*sizeof(GLuint); }

	vector<glm::mat4> getTransforms(int x = 0, int y = 0) const
	{ return this->transforms; }
    
private:

	void makeDoubleSided()
	{
		int upperBound = elements.size() / 3;
		for(int i = 0 ; i < upperBound ; ++i)
		{
			elements.push_back(elements[i*3+2]);
			elements.push_back(elements[i*3+1]);
			elements.push_back(elements[i*3+0]);
		}
	}

	void initWall()
	{

		positions.push_back(-0.5);
		positions.push_back(-0.5);
		positions.push_back(0);

		positions.push_back(0.5);
		positions.push_back(-0.5);
		positions.push_back(0);

		positions.push_back(0.5);
		positions.push_back(0.5);
		positions.push_back(0);

		positions.push_back(-0.5);
		positions.push_back(0.5);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);

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

        for(size_t i=0; i<positions.size()/3; i++) {
			if(i % 5 == 4)
			{
				colors.push_back(0);
				colors.push_back(0);
				colors.push_back(0);
			}
			else
			{
				colors.push_back(1);
				colors.push_back(1);
				colors.push_back(1);
			}
        }

		glm::mat4 leftWall = glm::translate(glm::rotate(glm::mat4(1.0f),PI/2,glm::vec3(0,1,0)), glm::vec3(-0.5,0,-0.5));
		for(int i = 1 ; i < xsize ; ++i)
		{
			for(int j = 1 ; j < ysize ; ++j)
			{
				glm::mat4 buildTrans = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));
				if(MAZE[i][j].up)
				{
					transforms.push_back(buildTrans);
				}
				if(MAZE[i][j].left)
				{
					transforms.push_back(buildTrans * leftWall);
				}
			}
		}
	}

	void initFloor()
	{

		positions.push_back(-0.5);
		positions.push_back(-0.5);
		positions.push_back(-1);

		positions.push_back(0.5);
		positions.push_back(-0.5);
		positions.push_back(-1);

		positions.push_back(0.5);
		positions.push_back(-0.5);
		positions.push_back(0);

		positions.push_back(-0.5);
		positions.push_back(-0.5);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(-0.5);
		positions.push_back(-0.5);

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

        for(size_t i=0; i<positions.size()/3; i++) {
			if(i % 5 == 4)
			{
				colors.push_back(0);
				colors.push_back(0);
				colors.push_back(1);
			}
			else
			{
				colors.push_back(0);
				colors.push_back(1);
				colors.push_back(0);
			}
        }

		for(int i = 1 ; i < xsize ; ++i)
		{
			for(int j = 1 ; j < ysize ; ++j)
			{
				glm::mat4 buildTrans = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));
				if(j > 1 && i < xsize - 1)
				{
					transforms.push_back(buildTrans);
				}
			}
		}
	}

	vector<GLfloat> positions;
	vector<GLfloat> colors;
	vector<GLuint> elements;
	vector<glm::mat4> transforms;
	size_t objectCount;
};

#endif