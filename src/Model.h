#ifndef __MODEL
#define __MODEL

#define NOMINMAX

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "MazeGenerator.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_integer.hpp"
#include "objload/objLoader.h"
#include <iostream>
#include <limits>
#define PI 3.14159f
#define GL3W
#ifdef GLEW
#define GLEW_STATIC
#include "glew/glew.h"
#endif
#ifdef GL3W
#include "gl3w/gl3w.h"
#endif
using namespace std; //makes using vectors easy

#define OBJ_VEC_SIZE 3
#define POS_DIM 3
#define TEX_DIM 2
#define MAX_FACE_SIZE 3

class Material
{
public:
	
	Material();
	
	Material(Material const & m);
	
	Material & operator=(Material const & m);
	
	~Material();
	
	glm::vec4 Ka;
	glm::vec4 Kd;
	glm::vec4 Ks;
	float specAlpha;
	char * texturePath;
};

class Model
{
public:

	Model();

	void init(string objName, int texNum, int xCell = 0, int yCell = 0, bool rotate = false, float yOffset = 0);

	void draw(GLuint shaderProg, glm::mat4 mR, size_t & prevElements, size_t & prevVertices, vector<GLuint> & textures);

	void setTexNum(int tex);
	
	vector<glm::vec3> const getPosition() const;

	vector<glm::vec2> const getTexCoord() const;
	
	vector<GLuint> const getElement() const;
	
	size_t getVertexCount() const;
	
	size_t getPositionBytes() const;
	
	size_t getTexCoordBytes() const;
	
	size_t getElementBytes() const;

	glm::vec3 getMinBound();

	glm::vec3 getMaxBound();

	glm::vec3 getCentroid();

	glm::vec3 getDimension();

	glm::mat4 getPreTrans(int x = 0, int y = 0) const;

	vector<glm::mat4> getPostTrans(int x = 0, int y = 0) const;

	bool getRotate();

	int getXcell();

	int getYcell();

	bool hasBeenFound();

	void find();
    
private:

	void makeDoubleSided();

	glm::vec3 computeCentroid();

	glm::vec3 computeMinBound();

	glm::vec3 computeMaxBound();

	glm::vec3 computeDimension();

	void initWall();

	void initFloor();

	void initSky();

	vector<glm::vec3> positions;
	vector<glm::vec3> normals;
	vector<glm::vec2> texCoords;
	vector<GLuint> elements;
	vector<Material> materials;
	vector<size_t> activeMaterial;
	vector<size_t> switchMaterialAt;
	glm::mat4 preTrans;
	vector<glm::mat4> postTrans;
	size_t objectCount;
	bool rotates;
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 dim;
	glm::vec3 center;
	int cellx;
	int celly;
	int texNum;
	bool found;
	int immune;
};

#endif