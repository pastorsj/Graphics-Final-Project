#ifndef __MODEL
#define __MODEL

#include <vector>
#include <iostream>
#include "glm/glm.hpp"
#include "objload/objLoader.h"
#include "MazeGenerator.h"
using namespace std; //makes using vectors easy

class Model
{
public:

    void init()
    {
		makeMaze();

		GLfloat centerX = ((GLfloat)xsize-1) / 2.0f;
		GLfloat centerY = ((GLfloat)ysize-1) / 2.0f;

		initWall();
		initFloor();

		glm::mat4 finalTrans = glm::translate(glm::mat4(1.0f), glm::vec3(-centerX, 0, -centerY));
		glm::mat4 leftWall = glm::translate(glm::rotate(glm::mat4(1.0f),PI/2,glm::vec3(0,1,0)), glm::vec3(-0.5,0,-0.5));
        for(int i = 1 ; i < xsize ; ++i)
		{
			for(int j = 1 ; j < ysize ; ++j)
			{
				glm::mat4 buildTrans = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));
				if(MAZE[i][j].up)
				{
					wallTransforms.push_back(finalTrans * buildTrans);
				}
				if(MAZE[i][j].left)
				{
					wallTransforms.push_back(finalTrans * buildTrans * leftWall);
				}
				if(j > 1 && i < xsize - 1)
				{
					floorTransforms.push_back(finalTrans * buildTrans);
				}
			}
		}
	}
	
	vector<GLfloat> const getWallPosition() const
	{ return wallPositions; }
	
	vector<GLfloat> const getWallColor() const
	{ return wallColors; }
	
	vector<GLuint> const getWallElements() const
	{ return wallElements; }
	
	size_t getWallVertexCount() const
	{ return wallPositions.size()/3; }
	
	size_t getWallPositionBytes() const
	{ return wallPositions.size()*sizeof(GLfloat); }
	
	size_t getWallColorBytes() const
	{ return wallColors.size()*sizeof(GLfloat); }
	
	size_t getWallElementBytes() const
	{ return wallElements.size()*sizeof(GLuint); }
	
	vector<GLfloat> const getFloorPosition() const
	{ return floorPositions; }
	
	vector<GLfloat> const getFloorColor() const
	{ return floorColors; }
	
	vector<GLuint> const getFloorElements() const
	{ return floorElements; }
	
	size_t getFloorVertexCount() const
	{ return floorPositions.size()/3; }
	
	size_t getFloorPositionBytes() const
	{ return floorPositions.size()*sizeof(GLfloat); }
	
	size_t getFloorColorBytes() const
	{ return floorColors.size()*sizeof(GLfloat); }
	
	size_t getFloorElementBytes() const
	{ return floorElements.size()*sizeof(GLuint); }

	vector<glm::mat4> getWallTransforms() const
	{ return this->wallTransforms; }

	vector<glm::mat4> getFloorTransforms() const
	{ return this->floorTransforms; }
    
private:

	void makeWallDoubleSided()
	{
		int upperBound = wallElements.size() / 3;
		for(int i = 0 ; i < upperBound ; ++i)
		{
			wallElements.push_back(wallElements[i*3+2]);
			wallElements.push_back(wallElements[i*3+1]);
			wallElements.push_back(wallElements[i*3+0]);
		}
	}

	void initWall()
	{

		wallPositions.push_back(-0.5);
		wallPositions.push_back(-0.5);
		wallPositions.push_back(0);

		wallPositions.push_back(0.5);
		wallPositions.push_back(-0.5);
		wallPositions.push_back(0);

		wallPositions.push_back(0.5);
		wallPositions.push_back(0.5);
		wallPositions.push_back(0);

		wallPositions.push_back(-0.5);
		wallPositions.push_back(0.5);
		wallPositions.push_back(0);

		wallPositions.push_back(0);
		wallPositions.push_back(0);
		wallPositions.push_back(0);

		wallElements.push_back(0);
		wallElements.push_back(1);
		wallElements.push_back(4);

		wallElements.push_back(1);
		wallElements.push_back(2);
		wallElements.push_back(4);

		wallElements.push_back(2);
		wallElements.push_back(3);
		wallElements.push_back(4);

		wallElements.push_back(3);
		wallElements.push_back(0);
		wallElements.push_back(4);

		makeWallDoubleSided();

        for(size_t i=0; i<wallPositions.size()/3; i++) {
			if(i % 5 == 4)
			{
				wallColors.push_back(0);
				wallColors.push_back(0);
				wallColors.push_back(0);
			}
			else
			{
				wallColors.push_back(1);
				wallColors.push_back(1);
				wallColors.push_back(1);
			}
        }
	}

	void initFloor()
	{

		floorPositions.push_back(-0.5);
		floorPositions.push_back(-0.5);
		floorPositions.push_back(-1);

		floorPositions.push_back(0.5);
		floorPositions.push_back(-0.5);
		floorPositions.push_back(-1);

		floorPositions.push_back(0.5);
		floorPositions.push_back(-0.5);
		floorPositions.push_back(0);

		floorPositions.push_back(-0.5);
		floorPositions.push_back(-0.5);
		floorPositions.push_back(0);

		floorPositions.push_back(0);
		floorPositions.push_back(-0.5);
		floorPositions.push_back(-0.5);

		int offset = 5;
		
		floorElements.push_back(4+offset);
		floorElements.push_back(1+offset);
		floorElements.push_back(0+offset);
		
		floorElements.push_back(4+offset);
		floorElements.push_back(2+offset);
		floorElements.push_back(1+offset);
		
		floorElements.push_back(4+offset);
		floorElements.push_back(3+offset);
		floorElements.push_back(2+offset);
		
		floorElements.push_back(4+offset);
		floorElements.push_back(0+offset);
		floorElements.push_back(3+offset);

        for(size_t i=0; i<floorPositions.size()/3; i++) {
			if(i % 5 == 4)
			{
				floorColors.push_back(0);
				floorColors.push_back(0);
				floorColors.push_back(1);
			}
			else
			{
				floorColors.push_back(0);
				floorColors.push_back(1);
				floorColors.push_back(0);
			}
        }
	}
/*	
	glm::vec3 computeMinBound()
	{
		glm::vec3 bound;
		
		for(int c=0; c<3; c++)
			bound[c] = std::numeric_limits<float>::max();
		
		for(int i=0; i<positions.size(); i+=3)
		{
			for(int c=0; c<3; c++)
			{
				if(positions[i+c] < bound[c])
					bound[c] = positions[i+c];
			}
		}
		
		return bound;
	}
	
	glm::vec3 computeMaxBound()
	{
		glm::vec3 bound;
		
		for(int c=0; c<3; c++)
			bound[c] = -std::numeric_limits<float>::max();
		
		for(int i=0; i<positions.size(); i+=3)
		{
			for(int c=0; c<3; c++)
			{
				if(positions[i+c] > bound[c])
					bound[c] = positions[i+c];
			}
		}
		
		return bound;
	}
	
	glm::vec3 computeCentroid()
	{
		glm::vec3 center = glm::vec3(0);
		float positionCount = 1.0f/(positions.size()/3.0f);
		
		for(int i=0; i<positions.size(); i+=3)
		{
			center[0] += positions[i] * positionCount;
			center[1] += positions[i+1] * positionCount;
			center[2] += positions[i+2] * positionCount;
		}
		
		return center;
	}
	
	glm::vec3 computeDimension()
	{
		glm::vec3 max = getMaxBound();
		glm::vec3 min = getMinBound();
		glm::vec3 dim = max - min;
		return dim;
	}
*/	
	vector<GLfloat> wallPositions;
	vector<GLfloat> wallColors;
	vector<GLuint> wallElements;
	vector<glm::mat4> wallTransforms;
	vector<GLfloat> floorPositions;
	vector<GLfloat> floorColors;
	vector<GLuint> floorElements;
	vector<glm::mat4> floorTransforms;
	size_t objectCount;
};

#endif