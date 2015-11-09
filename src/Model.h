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

	Model()
	{
		rotates = false;
	}

    void init(string objName)
    {
		preTrans = glm::mat4(1.0f);
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
			objLoader loader;
			loader.load(objName.c_str());
			//loader.load("resources/sphere.obj");
			//loader.load("resources/teapot.obj");
			//loader.load("resources/test.obj");
		
			for(size_t i=0; i<loader.vertexCount; i++) {
				positions.push_back(loader.vertexList[i]->e[0]);
				positions.push_back(loader.vertexList[i]->e[1]);
				positions.push_back(loader.vertexList[i]->e[2]);
				//printf("v%zu: %f %f %f\n", i, positions[i*3+0], positions[i*3+1], positions[i*3+2]);
			}
		
			for(size_t i=0; i<loader.faceCount; i++) {
				if(loader.faceList[i]->vertex_count != 3) {
					fprintf(stderr, "Only triangle primitives are supported.\n");
					exit(1);
				}
			
				elements.push_back(loader.faceList[i]->vertex_index[0]);
				elements.push_back(loader.faceList[i]->vertex_index[1]);
				elements.push_back(loader.faceList[i]->vertex_index[2]);
				//printf("f%zu: %i %i %i\n", i, elements[i*3+0], elements[i*3+1], elements[i*3+2]);
			}

			for(size_t i = 0 ; i < elements.size() ; ++i)
			{
				colors.push_back((rand()%255)/255.0f); // this probably should change
			}

			min = computeMinBound();
			max = computeMaxBound();
			center = computeCentroid();
			dim = computeDimension();

			int desX = 4;
			int desY = 4;

			glm::mat4 moveCenter = glm::translate(glm::mat4(1.0f), -center);
			int maxDim = dim[0] > dim[2] ? dim[0] : dim[2];
			float scaler = 0.8f / maxDim;
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaler));
			glm::mat4 moveCell = glm::translate(glm::mat4(1.0f), glm::vec3(desX, -0.25, desY+0.5));
			rotates = true;
			preTrans = scale * moveCenter;
			postTrans.push_back(moveCell);
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

	glm::vec3 getMinBound()
	{ return min; }

	glm::vec3 getMaxBound()
	{ return max; }

	glm::vec3 getCentroid()
	{ return center; }

	glm::vec3 getDimension()
	{ return dim; }

	glm::mat4 getPreTrans(int x = 0, int y = 0) const
	{ return this->preTrans; }

	vector<glm::mat4> getPostTrans(int x = 0, int y = 0) const
	{ return this->postTrans; }

	bool getRotate()
	{ return rotates; }
    
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

	glm::vec3 computeCentroid()
	{
		glm::vec3 center = glm::vec3(0);
		float positionCount = 1.0f/(positions.size()/3.0f);

		for(int i = 0 ; i < positions.size() ; i += 3)
		{
			center[0] = positions[i] * positionCount;
			center[1] = positions[i+1] * positionCount;
			center[2] = positions[i+2] * positionCount;
		}

		return center;	
	}

	glm::vec3 computeMinBound()
	{
		glm::vec3 bound;

		for(int c = 0 ; c < 3 ; ++c)
		{
			bound[c] = std::numeric_limits<float>::max();
		}

		for(int i = 0 ; i < positions.size() ; i+=3)
		{
			for(int c = 0 ; c < 3 ; ++c)
			{
				if(positions[i+c] < bound[c])
				{
					bound[c] = positions[i+c];
				}
			}
		}
		return bound;
	}

	glm::vec3 computeMaxBound()
	{
		glm::vec3 bound;

		for(int c = 0 ; c < 3 ; ++c)
		{
			bound[c] = -std::numeric_limits<float>::max();
		}

		for(int i = 0 ; i < positions.size() ; i+=3)
		{
			for(int c = 0 ; c < 3 ; ++c)
			{
				if(positions[i+c] > bound[c])
				{
					bound[c] = positions[i+c];
				}
			}
		}
		return bound;
	}

	glm::vec3 computeDimension()
	{
		glm::vec3 max = getMaxBound();
		glm::vec3 min = getMinBound();
		glm::vec3 dim = max - min;
		return dim;
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
					postTrans.push_back(buildTrans);
				}
				if(MAZE[i][j].left)
				{
					postTrans.push_back(buildTrans * leftWall);
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
					postTrans.push_back(buildTrans);
				}
			}
		}
	}

	vector<GLfloat> positions;
	vector<GLfloat> colors;
	vector<GLuint> elements;
	glm::mat4 preTrans;
	vector<glm::mat4> postTrans;
	size_t objectCount;
	bool rotates;
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 dim;
	glm::vec3 center;
};

#endif