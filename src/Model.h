#ifndef __MODEL
#define __MODEL

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "objload/objLoader.h"
using namespace std; //makes using vectors easy

#define OBJ_VEC_SIZE 3
#define POS_DIM 3
#define TEX_DIM 2
#define MAX_FACE_SIZE 3

class Material
{
public:
	
	Material() {
		texturePath = NULL;
		
		Ka = glm::vec4(0.1, 0.1, 0.1, 1.0);
		Kd = glm::vec4(0.5, 0.5, 0.5, 1.0);
		Ks = glm::vec4(0.8, 0.8, 0.8, 1.0);
		specAlpha = 10;
	}
	
	Material(Material const & m) {
		texturePath = NULL;
		*this = m;
	}
	
	Material & operator=(Material const & m) {
		if(&m == this)
			return *this;

		if(texturePath != NULL)
			free(texturePath);
		
		Ka = m.Ka;
		Kd = m.Kd;
		Ks = m.Ks;
		specAlpha = m.specAlpha;
		
		texturePath = NULL;
		
		if(m.texturePath != NULL) {
			texturePath = (char*) malloc( (strlen(m.texturePath)+1)*sizeof(char));
			strncpy(texturePath, m.texturePath, strlen(m.texturePath));
		}
	}
	
	~Material() {
		if(texturePath != NULL)
			free(texturePath);
	}
	
	glm::vec4 Ka;
	glm::vec4 Kd;
	glm::vec4 Ks;
	float specAlpha;
	char * texturePath;
};

class Model
{
public:

	Model()
	{
		rotates = false;
	}

    void init(string objName, int texNum, int xCell = 0, int yCell = 0, bool rotate = false, float yOffset = 0)
    {
		immune = 0;
		preTrans = glm::mat4(1.0f);
		this->texNum = texNum;
		if(strcmp(objName.c_str(), "wall") == 0)
		{
			initWall();
		}
		else if(strcmp(objName.c_str(), "floor") == 0)
		{
			initFloor();
		}
		else if(strcmp(objName.c_str(), "sky") == 0)
		{
			initSky();
			immune = 1;
		}
		else
		{
			objLoader loader;
			loader.load(objName.c_str());
			
			Material defaultMaterial;
			materials.push_back(defaultMaterial);

			for(size_t i = 0 ; i < loader.materialCount ; ++i)
			{
				Material m;
				for(size_t c = 0 ; c < 3 ; ++c)
				{
					m.Ka[c] = loader.materialList[i]->amb[c];
					m.Kd[c] = loader.materialList[i]->diff[c];
					m.Ks[c] = loader.materialList[i]->spec[c];
					m.specAlpha = loader.materialList[i]->shiny;
				}
				materials.push_back(m);
			}

			positions.resize(loader.vertexCount);
			texCoords.resize(loader.vertexCount);

			switchMaterialAt.push_back(0);

			for(size_t f = 0 ; f < loader.faceCount; ++f)
			{
				obj_face const * face = loader.faceList[f];

				int faceMaterial = face->material_index+1;
				bool firstMaterial = activeMaterial.size() == 0;
				bool materialChanged = !firstMaterial && activeMaterial[activeMaterial.size() - 1] != faceMaterial;
				if(firstMaterial || materialChanged)
				{
					activeMaterial.push_back(faceMaterial);
				}

				if(!firstMaterial && materialChanged)
				{
					switchMaterialAt.push_back(f);
				}

				bool isNotTriangle = face->vertex_count != MAX_FACE_SIZE;
				if(isNotTriangle) {
					cerr << "Skipping non-triangle face " << f << "." << endl;
					continue;
				}

				bool indicesMatch = true;
				for(size_t v=0 ; v < face->vertex_count ; ++v)
				{
					int pId = face->vertex_index[v];
					int tId = face->texture_index[v];

					indicesMatch = indicesMatch && (pId == tId);
				}

				if( !indicesMatch ) {
				//	cerr << "OBJ has non-matching pos/tex indices. Final model may be incorrect." << endl;
				}

				for(size_t v = 0 ; v < face->vertex_count ; ++v)
				{
					elements.push_back(face->vertex_index[v]);
				}

				int pId[MAX_FACE_SIZE];
				int tId[MAX_FACE_SIZE];

				glm::vec3 p[POS_DIM];
				glm::vec2 t[POS_DIM];

				for(size_t v = 0 ; v < face->vertex_count ; ++v)
				{
					pId[v] = face->vertex_index[v];
					tId[v] = face->texture_index[v];
				}

				for(size_t v = 0 ; v < face->vertex_count ; ++v)
				{
					for(size_t c = 0 ; c < POS_DIM ; ++c)
					{
						p[v][c] = loader.vertexList[pId[v]]->e[c];
					}
				}

				for(size_t v = 0 ; v < face->vertex_count ; ++v)
				{
					bool validTexCoord = tId[v] >= 0;
					if(validTexCoord)
					{
						for(size_t c = 0 ; c < TEX_DIM ; ++c)
						{
							t[v][c] = loader.textureList[tId[v]]->e[c];
						}
					}
				}

				for(size_t v = 0 ; v < face->vertex_count ; ++v)
				{
					positions[pId[v]] = p[v];
					texCoords[pId[v]] = t[v];
				}
			}
			switchMaterialAt.push_back(loader.faceCount);
			
			// custom stuff
			min = computeMinBound();
			max = computeMaxBound();
			center = computeCentroid();
			dim = computeDimension();

			glm::mat4 moveCenter = glm::translate(glm::mat4(1.0f), -center);
			int maxDim = dim[0] > dim[2] ? dim[0] : dim[2];
			maxDim = maxDim > dim[1] ? maxDim : dim[1];
			float scaler = 0.8f / maxDim;
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaler));
			glm::mat4 moveCell = glm::translate(glm::mat4(1.0f), glm::vec3(xCell, yOffset, yCell+0.5));
			rotates = rotate;
			preTrans = scale * moveCenter;
			postTrans.push_back(moveCell);
			cellx = xCell;
			celly = yCell;
			found = false;
		}
	}

	void draw(GLuint shaderProg, glm::mat4 mR, size_t & prevElements, size_t & prevVertices, vector<GLuint> & textures)
	{
		glBindTexture(GL_TEXTURE_2D, textures[texNum]);
		GLint texUnitID = 0;
		glActiveTexture(GL_TEXTURE0+texUnitID);
		glUniform1i(glGetUniformLocation(shaderProg, "texSampler"), texUnitID);
		glUniform1i(glGetUniformLocation(shaderProg, "immune"), immune);
		vector<glm::mat4> postTransList;
		glm::mat4 preTrans, postTrans;
		glm::mat4 ident = glm::mat4(1.0f);
		
		preTrans = this->getPreTrans();
		postTransList = this->getPostTrans();
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "preTrans"), 1, GL_FALSE, &preTrans[0][0]);
		if(this->getRotate())
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
			//cout << this->getElement().size() << " " << prevElements << " " << prevVertices << endl;
			glDrawElementsBaseVertex(GL_TRIANGLES, this->getElement().size(), GL_UNSIGNED_INT, (void*)(prevElements*sizeof(GLfloat)), prevVertices);
		}
		
		prevElements += this->getElement().size();
		prevVertices += this->getVertexCount();
	}

	void setTexNum(int tex){
		texNum = tex;
	}
	
	vector<glm::vec3> const getPosition() const
	{ return positions; }

	vector<glm::vec2> const getTexCoord() const
	{ return texCoords; }
	
	vector<GLuint> const getElement() const
	{ return elements; }
	
	size_t getVertexCount() const
	{ return positions.size(); }
	
	size_t getPositionBytes() const
	{ return positions.size()*3*sizeof(GLfloat); }
	
	size_t getTexCoordBytes() const
	{ return texCoords.size()*2*sizeof(GLfloat); }
	
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

	int getXcell()
	{ return cellx; }

	int getYcell()
	{ return celly; }

	bool hasBeenFound() {
		return found;
	}

	void find() {
		found = true;
		printf("Model found\n");
	}
    
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
		float positionCount = 1.0f/(positions.size());

		for(int i = 0 ; i < positions.size() ; ++i)
		{
			center[0] = positions[i][0] * positionCount;
			center[1] = positions[i][1] * positionCount;
			center[2] = positions[i][2] * positionCount;
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

		for(int i = 0 ; i < positions.size() ; ++i)
		{
			for(int c = 0 ; c < 3 ; ++c)
			{
				if(positions[i][c] < bound[c])
				{
					bound[c] = positions[i][c];
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

		for(int i = 0 ; i < positions.size() ; ++i)
		{
			for(int c = 0 ; c < 3 ; ++c)
			{
				if(positions[i][c] > bound[c])
				{
					bound[c] = positions[i][c];
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
		positions.push_back(glm::vec3(-0.5, -0.5, 0));
		texCoords.push_back(glm::vec2(0, 1));

		positions.push_back(glm::vec3(0.5, -0.5, 0));
		texCoords.push_back(glm::vec2(1, 1));

		positions.push_back(glm::vec3(0.5, 0.5, 0));
		texCoords.push_back(glm::vec2(1, 0));

		positions.push_back(glm::vec3(-0.5, 0.5, 0));
		texCoords.push_back(glm::vec2(0, 0));

		positions.push_back(glm::vec3(0, 0, 0));
		texCoords.push_back(glm::vec2(0.5, 0.5));

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
		
		glm::mat4 leftWall = glm::translate(glm::rotate(glm::mat4(1.0f),PI/2,glm::vec3(0,1,0)), glm::vec3(-0.5,0,-0.5));
		for(int i = 1 ; i < xsize ; ++i)
		{
			for(int j = 1 ; j < ysize ; ++j)
			{
				glm::mat4 buildTrans = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));
				MazeGenerator * mg = MazeGenerator::instance();
				if(mg->getCell(i,j).up)
				{
					postTrans.push_back(buildTrans);
				}
				if(mg->getCell(i,j).left)
				{
					postTrans.push_back(buildTrans * leftWall);
				}
			}
		}
	}

	void initFloor()
	{
		positions.push_back(glm::vec3(-0.5, -0.5, -1));
		texCoords.push_back(glm::vec2(0,1));

		positions.push_back(glm::vec3(0.5, -0.5, -1));
		texCoords.push_back(glm::vec2(1,1));

		positions.push_back(glm::vec3(0.5, -0.5, 0));
		texCoords.push_back(glm::vec2(1,0));

		positions.push_back(glm::vec3(-0.5, -0.5, 0));
		texCoords.push_back(glm::vec2(0,0));
		
		positions.push_back(glm::vec3(0, -0.5, -0.5));
		texCoords.push_back(glm::vec2(0.5, 0.5));

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

	void initSky()
	{
		positions.push_back(glm::vec3(xsize/2, 0, 0));
		texCoords.push_back(glm::vec2(1, 0.8));
		positions.push_back(glm::vec3(-xsize/2, 0, 0));
		texCoords.push_back(glm::vec2(0, 0.8));
		positions.push_back(glm::vec3(xsize/2, 40, 0));
		texCoords.push_back(glm::vec2(1, 0));
		positions.push_back(glm::vec3(-xsize/2, 40, 0));
		texCoords.push_back(glm::vec2(0, 0));

		elements.push_back(1);
		elements.push_back(0);
		elements.push_back(2);

		elements.push_back(1);
		elements.push_back(2);
		elements.push_back(3);

		preTrans = glm::mat4(1.0f);
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), PI/2, glm::vec3(0,1,0));
		postTrans.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(xsize/2, 0, 0)));
		postTrans.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, xsize/2)) * rot);
		postTrans.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(xsize/2, 0, xsize)) * rot * rot);
		postTrans.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(xsize, 0, xsize/2)) * rot * rot * rot);
	}

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