#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "GLHelper.h"
#include "WorldState.h"

class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
		
		//camera
        this->P = glm::perspective(1.0f, 1.0f, 0.1f, 100.0f);
        this->C = glm::mat4(1);
		this->M = glm::mat4(1);
	}

	~RenderEngine()
	{
		if(initialized)
		{
			// Clean up the buffers
			//glDeleteBuffers(1, &positionBuffer);
			//glDeleteBuffers(1, &colorBuffer);
		}
	}

	void init(WorldState & state)
	{
		initialized = true;

		float ver = initLoader();
		if( ver < 1.0f ) {
			printf("OpenGL is not supported.\n");
			exit(1);
		}
		printf("OpenGL version %.1f is supported.\n", ver);
		
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
        
		glm::vec3 dim = state.getModel().getDimension();
		float maxDim = std::max(dim[0], std::max(dim[1], dim[2]));
		this->P = glm::perspective(1.0f, 1.0f, maxDim*0.01f, maxDim*10.0f);
        C = state.getCameraMatrix();
		
		setupShader();
		setupBuffers(state.getModel());
	}

	void display(WorldState & state)
	{
		//clear the old frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 mT = state.getModelTranslate();
        glm::mat4 mR = state.getModelRotate();
		glm::mat4 M = C*mR*mT;
		glm::mat4 N = glm::inverseTranspose(M);
        glm::vec4 lightPos = state.getLightPos();
        glm::vec4 camPos = state.getCameraPos();
        glm::mat4 L = state.getLightRotate();
        
        //hacky light source size change
        GLfloat maxDis = state.getModel().getDimension()[2] * 3;
        GLfloat distScale = 1.0f / (glm::length(L*lightPos - camPos) / maxDis);
        glPointSize(glm::mix(1.0f, 10.0f, distScale));
        
        //printf("cam %f %f %f\n", camPos[0], camPos[1], camPos[2]);
        //printf("light %f %f %f\n", lightPos[0], lightPos[1], lightPos[2]);
		
		//use shader
		glUseProgram(shaderProg);
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "C"), 1, GL_FALSE, &C[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "mR"), 1, GL_FALSE, &mR[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "mT"), 1, GL_FALSE, &mT[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "M"), 1, GL_FALSE, &M[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "N"), 1, GL_FALSE, &N[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "L"), 1, GL_FALSE, &L[0][0]);
        glUniform4fv(glGetUniformLocation(shaderProg, "lightPos"), 1, &lightPos[0]);
        glUniform4fv(glGetUniformLocation(shaderProg, "camPos"), 1, &camPos[0]);
        glUniform1i(glGetUniformLocation(shaderProg, "shadingMode"), state.getShadingMode());
		
		//draw
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, state.getModel().getElements().size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
        checkGLError("model");
        
        
        glUseProgram(lightProg);
        
        glUniformMatrix4fv(glGetUniformLocation(lightProg, "P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lightProg, "C"), 1, GL_FALSE, &C[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lightProg, "mR"), 1, GL_FALSE, &mR[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lightProg, "mT"), 1, GL_FALSE, &mT[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightProg, "M"), 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightProg, "N"), 1, GL_FALSE, &N[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lightProg, "L"), 1, GL_FALSE, &L[0][0]);
        glUniform4fv(glGetUniformLocation(lightProg, "lightPos"), 1, &lightPos[0]);
        glUniform4fv(glGetUniformLocation(lightProg, "camPos"), 1, &camPos[0]);
        glUniform1i(glGetUniformLocation(lightProg, "shadingMode"), state.getShadingMode());
        
        glBindVertexArray(lightArray);
        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
        checkGLError("light");
	}


private:
	bool initialized;
	GLuint shaderProg;
    GLuint lightProg;
	GLuint vertexArray;
    GLuint lightArray;
	
	glm::mat4 P;
	glm::mat4 C;
	glm::mat4 M;

	
	float initLoader()
	{
		float ver = 0.0f;
#ifdef GLEW
		glewExperimental = GL_TRUE;
		
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
		
		if (GLEW_VERSION_1_1) { ver = 1.1f; }
		if (GLEW_VERSION_1_2) { ver = 1.2f; }
		if (GLEW_VERSION_1_3) { ver = 1.3f; }
		if (GLEW_VERSION_1_4) { ver = 1.4f; }
		if (GLEW_VERSION_1_5) { ver = 1.5f; }
		
		if (GLEW_VERSION_2_0) { ver = 2.0f; }
		if (GLEW_VERSION_2_1) { ver = 2.1f; }
		
		if (GLEW_VERSION_3_0) { ver = 3.0f; }
		if (GLEW_VERSION_3_1) { ver = 3.1f; }
		if (GLEW_VERSION_3_2) { ver = 3.2f; }
		if (GLEW_VERSION_3_3) { ver = 3.3f; }
		
		if (GLEW_VERSION_4_0) { ver = 4.0f; }
		if (GLEW_VERSION_4_1) { ver = 4.1f; }
		if (GLEW_VERSION_4_2) { ver = 4.2f; }
		if (GLEW_VERSION_4_3) { ver = 4.3f; }
		if (GLEW_VERSION_4_4) { ver = 4.4f; }
		if (GLEW_VERSION_4_5) { ver = 4.5f; }
#endif
		
#ifdef GL3W
		if (gl3wInit()) {
			fprintf(stderr, "failed to initialize OpenGL\n");
		}
		
		if (gl3wIsSupported(1, 1)) { ver = 1.1f; }
		if (gl3wIsSupported(1, 2)) { ver = 1.2f; }
		if (gl3wIsSupported(1, 3)) { ver = 1.3f; }
		if (gl3wIsSupported(1, 4)) { ver = 1.4f; }
		if (gl3wIsSupported(1, 5)) { ver = 1.5f; }
		
		if (gl3wIsSupported(2, 0)) { ver = 2.0f; }
		if (gl3wIsSupported(2, 1)) { ver = 2.1f; }
		
		if (gl3wIsSupported(3, 0)) { ver = 3.0f; }
		if (gl3wIsSupported(3, 1)) { ver = 3.1f; }
		if (gl3wIsSupported(3, 2)) { ver = 3.2f; }
		if (gl3wIsSupported(3, 3)) { ver = 3.3f; }
		
		if (gl3wIsSupported(4, 0)) { ver = 4.0f; }
		if (gl3wIsSupported(4, 1)) { ver = 4.1f; }
		if (gl3wIsSupported(4, 2)) { ver = 4.2f; }
		if (gl3wIsSupported(4, 3)) { ver = 4.3f; }
		if (gl3wIsSupported(4, 4)) { ver = 4.4f; }
		if (gl3wIsSupported(4, 5)) { ver = 4.5f; }
#endif
		
		return ver;
	}

	void setupShader()
	{
		char const * vertPath = "resources/simple.vert";
		char const * fragPath = "resources/simple.frag";
		shaderProg = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);
        
        
        char const * lightVPath = "resources/lightPos.vert";
        char const * lightFPath = "resources/lightPos.frag";
        lightProg = ShaderManager::shaderFromFile(&lightVPath, &lightFPath, 1, 1);

		checkGLError("shader");
	}

	void setupBuffers(Model & model)
	{
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
		
		GLuint positionBuffer;
		GLuint colorBuffer;
		GLuint elementBuffer;
        GLint colorSlot;
        GLint positionSlot;
		
		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getPositionBytes(), &model.getPosition()[0], GL_STATIC_DRAW);
		positionSlot = glGetAttribLocation(shaderProg, "pos");
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Do the same thing for the color data
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getColorBytes(), &model.getColor()[0], GL_STATIC_DRAW);
		colorSlot =    glGetAttribLocation(shaderProg, "colorIn");
		glEnableVertexAttribArray(colorSlot);
		glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// now the elements
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getElementBytes(), &model.getElements()[0], GL_STATIC_DRAW);
		//leave the element buffer active
        
		//hacky way to draw the light
        glGenVertexArrays(1, &lightArray);
        glBindVertexArray(lightArray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBindVertexArray(0);

		checkGLError("setup");
	}
};

#endif