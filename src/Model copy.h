#ifndef __MODEL
#define __MODEL

#include <vector>

using namespace std; //makes using vectors easy

#include "TeapotData.h"

class Model
{
public:
	
	Model()
	{
		for(int i=0; i<teapotNumVertices*3; i++)
		{
			positions.push_back(teapotVertices[i]);
		}
		
		for(int i=0; i<teapotNumFaces*3; i++)
		{
			elements.push_back(teapotVertexIndices[i]-1);
		}
	}
	
	GLfloat const * getPositions() const
	{ return &positions[0]; }
	
	GLfloat const * getNormals() const
	{ return &normals[0]; }
	
	GLuint const * getElements() const
	{ return &elements[0]; }
	
	size_t getElementCount() const
	{ return elements.size(); }
	
	size_t getElementBytes() const
	{ return elements.size()*sizeof(GLuint); }
	
	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }
	
	size_t getNormalBytes() const
	{ return normals.size()*sizeof(GLfloat); }
    
    size_t getColorBytes() const
    { return colors.size()*sizeof(GLfloat); }
	
	glm::vec3 getCentroid()
	{
		glm::vec3 min = getMinBound();
		glm::vec3 max = getMaxBound();
		glm::vec3 center;
		
		for(int c=0; c<3; c++)
			center = (min+max) * 0.5f;
		
		return center;
	}
	
	glm::vec3 getMinBound()
	{
		glm::vec3 bound;
		bound = glm::vec3(9999999.0);
		
		for(unsigned int i=0; i<this->positions.size(); i++)
		{
			for(int c=0; c<3; c++)
			{
				if(positions[i*3+c] < bound[c])
					bound[c] = positions[i*3+c];
			}
		}
		
		return bound;
	}
	
	glm::vec3 getMaxBound()
	{
		glm::vec3 bound;
		bound = glm::vec3(-9999999.0);
		
		for(unsigned int i=0; i<this->positions.size(); i++)
		{
			for(int c=0; c<3; c++)
			{
				if(positions[i*3+c] > bound[c])
					bound[c] = (positions[i*3+c]);
			}
		}
		
		return bound;
	}
    
    
    
    
    
    
    
    GLfloat const * getColor() const
    { return &colors[0]; }
    
    GLuint const * getElements() const
    { return &elements[0]; }
    
    size_t getVertexCount() const
    { return positions.size()/3; }
    
    size_t getPositionBytes() const
    { return positions.size()*sizeof(GLfloat); }
    
    size_t getColorBytes() const
    { return colors.size()*sizeof(GLfloat); }
    
    size_t getElementBytes() const
    { return elements.size()*sizeof(GLuint); }
    
    glm::vec3 getCentroid()
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
    
    
    
    
    
    
	
private:
	vector<GLfloat> positions;
	vector<GLfloat> normals;
    vector<GLfloat> colors;
	vector<GLuint> elements;
};

#endif
