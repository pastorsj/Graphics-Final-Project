#ifndef __MODEL
#define __MODEL

#include <vector>
#include "glm/glm.hpp"
#include "objload/objLoader.h"
using namespace std; //makes using vectors easy

class Model
{
public:
	

    void init()
    {
		objLoader loader;
		//loader.load("resources/cube.obj");
		loader.load("resources/sphere.obj");
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
        
        for(size_t i=0; i<positions.size(); i++) {
            colors.push_back(0);
            colors.push_back(0);
            colors.push_back(0);
        }

        //TODO compute the vertex normals by averaging the face normals
        
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
                colors[elements[i+v]*3 + v] = vertexNormals[elements[i+v]*3][v];
            }
        }
        
        
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
    
private:
	
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
	size_t objectCount;
    
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 dim;
    glm::vec3 center;
};

#endif