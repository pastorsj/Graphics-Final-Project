#ifndef __RENDERENGINE
#define __RENDERENGINE

#pragma once
#include "glm/glm.hpp"
#include "WorldState.h"
#include "ShaderManager.h"

class RenderEngine
{
public:
	RenderEngine();

	~RenderEngine();

	void init(WorldState & state);;
	void display(WorldState &state);

	void buildRenderBuffers(size_t xRes, size_t yRes);

private:
	bool initialized;
	GLuint shaderProg[3];
	GLuint textures[2];
	GLuint vertexArray;
	GLuint lightArray;

	// Render to Texture Variables
	GLuint quadVertexArray;
	GLuint frameBuffer;
	GLuint renderTexture;
	GLuint renderBuffer;
	GLuint renderTextureOverlay;
	GLuint renderTextureVictoryOverlay;

	glm::mat4 P;
	glm::mat4 C;
	glm::mat4 M;


	float initLoader();

	void setupShader();

	void setupTextures(ModelManager & models);

	void setupBuffers(ModelManager & models);

	GLint setupBuffer(GLuint bufferIndex, vector<GLfloat> buffer, size_t totalSize, string name, GLint sizePer);
};

#endif