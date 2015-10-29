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
		//objLoader loader;
		//loader.load("resources/cube.obj");
		//loader.load("resources/sphere.obj");
		//loader.load("resources/teapot.obj");
        //loader.load("resources/test.obj");
		/*
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
		*/

		GLfloat centerX = ((GLfloat)xsize-1) / 2.0f;
		GLfloat centerY = ((GLfloat)ysize-1) / 2.0f;

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
			}
		}

		/*
        //Compute all face normals
        vector<glm::vec3> vertexNormals;
        //fill this with position.size 0 vec3s
        for(size_t i = 0; i < positions.size(); i++) {
            vertexNormals.push_back(glm::vec3(0.0f));
        }
        
        for(size_t i = 0; i < elements.size(); i+=3) {
            size_t vertexId[3];
            for(size_t v=0; v<3; v++)
                vertexId[v] = elements[i+v];
            
            glm::vec3 vertices[3];
            for(size_t v=0; v<3; v++)
                for(size_t c=0; c<3; c++)
                    vertices[v][c] = positions[ vertexId[v]*3 + c ];
            
            glm::vec3 a = vertices[1] - vertices[0];
            glm::vec3 b = vertices[2] - vertices[1];
            glm::vec3 faceNormal = glm::normalize(glm::cross(a, b));
            
            //Accumulate face normals at vertices
            for(size_t j = 0; j < 3; j++) {
                //for each vertex on the triangle, add the face normal to that normal
                vertexNormals[vertexId[j] * 3] += faceNormal;
            }

        }
        
        //Normalize vertex normals
        for(size_t i = 0; i < vertexNormals.size(); i++) {
            vertexNormals[i] = glm::normalize(vertexNormals[i]);
            vertexNormals[i] = (vertexNormals[i] + 1.0f) * 0.5f;
        }
		
        
        for(size_t i = 0; i < elements.size(); i++) {
            for (size_t v = 0; v<3; v++) {
                colors[elements[i+v]*3 + v] = 1;
            }
        }
		*/

		

        min = computeMinBound();
        max = computeMaxBound();
        center = computeCentroid();
        dim = computeDimension();
	}
	
	vector<GLfloat> const getPosition() const
	{ return positions; }
	
	vector<GLfloat> const getColor() const
	{ return colors; }
	
	vector<GLuint> const getElements() const
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

	vector<glm::mat4> getWallTransforms() const
	{ return this->wallTransforms; }
    
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
	
	vector<GLfloat> positions;
	vector<GLfloat> colors;
	vector<GLuint> elements;
	vector<glm::mat4> wallTransforms;
	size_t objectCount;
    
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 dim;
    glm::vec3 center;
};

#endif