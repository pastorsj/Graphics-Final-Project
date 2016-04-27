#include "Model.h"

Material::Material() {
	texturePath = NULL;

	Ka = glm::vec4(0.1, 0.1, 0.1, 1.0);
	Kd = glm::vec4(0.5, 0.5, 0.5, 1.0);
	Ks = glm::vec4(0.8, 0.8, 0.8, 1.0);
	specAlpha = 10;
}

Material::Material(Material const & m) {
	texturePath = NULL;
	*this = m;
}

Material & Material::operator=(Material const & m) {
	if (&m == this)
		return *this;

	if (texturePath != NULL)
		free(texturePath);

	Ka = m.Ka;
	Kd = m.Kd;
	Ks = m.Ks;
	specAlpha = m.specAlpha;

	texturePath = NULL;

	if (m.texturePath != NULL) {
		texturePath = (char*)malloc((strlen(m.texturePath) + 1)*sizeof(char));
		strncpy(texturePath, m.texturePath, strlen(m.texturePath));
	}
}

Material::~Material() {
	if (texturePath != NULL)
		free(texturePath);
}

Model::Model()
{
	rotates = false;
}

void Model::init(string objName, int texNum, int xCell, int yCell, bool rotate, float yOffset)
{
	immune = 0;
	preTrans = glm::mat4(1.0f);
	this->texNum = texNum;

	objLoader loader;
	loader.load(objName.c_str());

	Material defaultMaterial;
	materials.push_back(defaultMaterial);

	for (size_t i = 0; i < loader.materialCount; ++i)
	{
		Material m;
		for (size_t c = 0; c < 3; ++c)
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
	normals.resize(loader.vertexCount);

	switchMaterialAt.push_back(0);

	printf("%s", objName.c_str());

	for (size_t f = 0; f < loader.faceCount; ++f)
	{
		obj_face const * face = loader.faceList[f];

		int faceMaterial = face->material_index + 1;
		bool firstMaterial = activeMaterial.size() == 0;
		bool materialChanged = !firstMaterial && activeMaterial[activeMaterial.size() - 1] != faceMaterial;
		if (firstMaterial || materialChanged)
		{
			activeMaterial.push_back(faceMaterial);
		}

		if (!firstMaterial && materialChanged)
		{
			switchMaterialAt.push_back(f);
		}

		//If the face isn't a triangle, skip it.
		if (face->vertex_count != MAX_FACE_SIZE) {
			cerr << "Skipping non-triangle face " << f << "." << endl;
			continue;
		}

		for (size_t v = 0; v < face->vertex_count; ++v)
		{
			elements.push_back(face->vertex_index[v]);

			int pId = face->vertex_index[v];
			int tId = face->texture_index[v];
			int normID = face->normal_index[v];

			positions[pId] = glm::vec3(loader.vertexList[pId]->e[0], loader.vertexList[pId]->e[1], loader.vertexList[pId]->e[2]);
			if (tId >= 0)
				texCoords[pId] = glm::vec2(loader.textureList[tId]->e[0], loader.textureList[tId]->e[1]);
			normals[pId] = glm::vec3(loader.normalList[normID]->e[0], loader.normalList[normID]->e[1], loader.normalList[normID]->e[2]);
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
	glm::mat4 moveCell = glm::translate(glm::mat4(1.0f), glm::vec3(xCell, yOffset, yCell + 0.5));
	rotates = rotate;
	preTrans = scale * moveCenter;
	postTrans.push_back(moveCell);
	cellx = xCell;
	celly = yCell;
	found = false;
}

void Model::draw(GLuint shaderProg, glm::mat4 mR, size_t & prevElements, size_t & prevVertices, vector<GLuint> & textures)
{
	glBindTexture(GL_TEXTURE_2D, textures[texNum]);
	GLint texUnitID = 0;
	glActiveTexture(GL_TEXTURE0 + texUnitID);
	glUniform1i(glGetUniformLocation(shaderProg, "texSampler"), texUnitID);
	glUniform1i(glGetUniformLocation(shaderProg, "immune"), immune);
	vector<glm::mat4> postTransList;
	glm::mat4 preTrans, postTrans;
	glm::mat4 ident = glm::mat4(1.0f);

	preTrans = this->getPreTrans();
	postTransList = this->getPostTrans();
	glUniformMatrix4fv(glGetUniformLocation(shaderProg, "preTrans"), 1, GL_FALSE, &preTrans[0][0]);
	if (this->getRotate())
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "mR"), 1, GL_FALSE, &mR[0][0]);
	}
	else
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "mR"), 1, GL_FALSE, &ident[0][0]);
	}

	//TODO Add support for multiple materials.
	//glUniform4fv(glGetUniformLocation(shaderProg, "Ka"), 1, &this->materials[0].Ka[0]);
	//glUniform4fv(glGetUniformLocation(shaderProg, "Kd"), 1, &this->materials[0].Kd[0]);
	//glUniform4fv(glGetUniformLocation(shaderProg, "Ks"), 1, &this->materials[0].Ks[0]);
	//glUniform1f(glGetUniformLocation(shaderProg, "specAlpha"), this->materials[0].specAlpha);

	for (int j = 0; j < postTransList.size(); ++j)
	{
		postTrans = postTransList[j];
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "postTrans"), 1, GL_FALSE, &postTrans[0][0]);
		//cout << this->getElement().size() << " " << prevElements << " " << prevVertices << endl;
		glDrawElementsBaseVertex(GL_TRIANGLES, this->getElement().size(), GL_UNSIGNED_INT, (void*)(prevElements*sizeof(GLfloat)), prevVertices);
	}

	prevElements += this->getElement().size();
	prevVertices += this->getVertexCount();
}

void Model::setTexNum(int tex) {
	texNum = tex;
}

vector<glm::vec3> const Model::getPosition() const
{
	return positions;
}

vector<glm::vec3> const Model::getNormal() const
{
	return normals;
}

vector<glm::vec2> const Model::getTexCoord() const
{
	return texCoords;
}

vector<GLuint> const Model::getElement() const
{
	return elements;
}

size_t Model::getVertexCount() const
{
	return positions.size();
}

size_t Model::getPositionBytes() const
{
	return positions.size() * 3 * sizeof(GLfloat);
}

size_t Model::getTexCoordBytes() const
{
	return texCoords.size() * 2 * sizeof(GLfloat);
}

size_t Model::getElementBytes() const
{
	return elements.size()*sizeof(GLuint);
}

size_t Model::getNormalBytes() const
{
	return normals.size()* 3 * sizeof(GLfloat);
}

glm::vec3 Model::getMinBound()
{
	return min;
}

glm::vec3 Model::getMaxBound()
{
	return max;
}

glm::vec3 Model::getCentroid()
{
	return center;
}

glm::vec3 Model::getDimension()
{
	return dim;
}

glm::mat4 Model::getPreTrans(int x, int y) const
{
	return this->preTrans;
}

vector<glm::mat4> Model::getPostTrans(int x, int y) const
{
	return this->postTrans;
}

bool Model::getRotate()
{
	return rotates;
}

int Model::getXcell()
{
	return cellx;
}

int Model::getYcell()
{
	return celly;
}

bool Model::hasBeenFound() {
	return found;
}

void Model::find() {
	found = true;
	printf("Model found\n");
}

void Model::makeDoubleSided()
{
	int upperBound = elements.size() / 3;
	for (int i = 0; i < upperBound; ++i)
	{
		elements.push_back(elements[i * 3 + 2]);
		elements.push_back(elements[i * 3 + 1]);
		elements.push_back(elements[i * 3 + 0]);
	}
}

glm::vec3 Model::computeCentroid()
{
	glm::vec3 center = glm::vec3(0);
	float positionCount = 1.0f / (positions.size());

	for (int i = 0; i < positions.size(); ++i)
	{
		center[0] = positions[i][0] * positionCount;
		center[1] = positions[i][1] * positionCount;
		center[2] = positions[i][2] * positionCount;
	}

	return center;
}

glm::vec3 Model::computeMinBound()
{
	glm::vec3 bound;

	for (int c = 0; c < 3; ++c)
	{
		bound[c] = std::numeric_limits<float>::max();
	}

	for (int i = 0; i < positions.size(); ++i)
	{
		for (int c = 0; c < 3; ++c)
		{
			if (positions[i][c] < bound[c])
			{
				bound[c] = positions[i][c];
			}
		}
	}
	return bound;
}

glm::vec3 Model::computeMaxBound()
{
	glm::vec3 bound;

	for (int c = 0; c < 3; ++c)
	{
		bound[c] = -std::numeric_limits<float>::max();
	}

	for (int i = 0; i < positions.size(); ++i)
	{
		for (int c = 0; c < 3; ++c)
		{
			if (positions[i][c] > bound[c])
			{
				bound[c] = positions[i][c];
			}
		}
	}
	return bound;
}

glm::vec3 Model::computeDimension()
{
	glm::vec3 max = getMaxBound();
	glm::vec3 min = getMinBound();
	glm::vec3 dim = max - min;
	return dim;
}