#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <iostream>
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

		MazeGenerator * mg = MazeGenerator::instance();
		int xsize = mg->getXSize();

		this->P = glm::perspective(1.0f, 1.0f, 0.01f, sqrt(40*40+2.0f*xsize*xsize));
        C = state.getCameraMatrix();
		
		setupTextures(state.getModels());
		setupShader();
		setupBuffers(state.getModels());
	}

	void display(WorldState &state)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		//clear the old frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 mT = state.getModelTranslate();
        glm::mat4 mR = state.getModelRotate();
		glm::mat4 mC = state.getCameraMatrix();
		glm::mat4 M = mC*mR*mT;
		glm::mat4 N = glm::inverseTranspose(M);
        glm::vec4 lightPos = state.getLightPos();
        glm::vec4 camPos = state.getCameraPos();
		glm::vec4 camLook = state.getCameraLook();
		float camAngle = state.getCameraAngle();
        glm::mat4 L = state.getLightRotate();
        
        //hacky light source size change
//        GLfloat maxDis = state.getModel().getDimension()[2] * 3;
//        GLfloat distScale = 1.0f / (glm::length(L*lightPos - camPos) / maxDis);
 //       glPointSize(glm::mix(1.0f, 10.0f, distScale));
        
		//printf("cam %f %f %f\n", camPos[0], camPos[1], camPos[2]);
		//printf("look %f %f %f\n", camLook[0], camLook[1], camLook[2]);
		//printf("angle : %f\n", camAngle);
        //printf("light %f %f %f\n", lightPos[0], lightPos[1], lightPos[2]);

		float x = state.getXPos();
		float y = state.getYPos();

		float xPos = state.getXPosComp();
		float yPos = state.getYPosComp();

	

		//printf("x pos: %f, y pos: %f\n", x, y);
		//printf("x pos: %f, y pos: %f\n", xPos, yPos);
		//printf("x cell: %i, y cell: %i\n", state.getXCell(), state.getYCell());
		//printf(MAZE[state.getXCell()][state.getYCell()].left ? "left wall\n" : "no left wall\n");
		//printf(MAZE[state.getXCell()][state.getYCell()].up ? "up wall\n" : "no up wall\n");

		
		//use shader
		glUseProgram(shaderProg[0]);
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "C"), 1, GL_FALSE, &mC[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "mR"), 1, GL_FALSE, &mR[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "mT"), 1, GL_FALSE, &mT[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "M"), 1, GL_FALSE, &M[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "N"), 1, GL_FALSE, &N[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg[0], "L"), 1, GL_FALSE, &L[0][0]);
        glUniform4fv(glGetUniformLocation(shaderProg[0], "lightPos"), 1, &lightPos[0]);
        glUniform4fv(glGetUniformLocation(shaderProg[0], "camPos"), 1, &camPos[0]);
        glUniform1i(glGetUniformLocation(shaderProg[0], "shadingMode"), state.getShadingMode());
		
		glBindVertexArray(vertexArray);
		
		
		// draw!
		//glBindTexture(GL_TEXTURE_2D, textures[0]);
		//glBindTexture(GL_TEXTURE_2D, textures[1]);

		GLint texUnitID = 0;
		glActiveTexture(GL_TEXTURE0 + texUnitID);

		glUniform1i(glGetUniformLocation(shaderProg[0], "texSampler"), texUnitID);

		state.getModels().draw(shaderProg[0], mR);
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("model");


		glUseProgram(shaderProg[1]);

		glUniformMatrix4fv(glGetUniformLocation(shaderProg[1], "P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[1], "C"), 1, GL_FALSE, &mC[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[1], "mR"), 1, GL_FALSE, &mR[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[1], "mT"), 1, GL_FALSE, &mT[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[1], "M"), 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[1], "N"), 1, GL_FALSE, &N[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg[1], "L"), 1, GL_FALSE, &L[0][0]);
		glUniform4fv(glGetUniformLocation(shaderProg[1], "lightPos"), 1, &lightPos[0]);
		glUniform4fv(glGetUniformLocation(shaderProg[1], "camPos"), 1, &camPos[0]);
		glUniform1i(glGetUniformLocation(shaderProg[1], "shadingMode"), state.getShadingMode());

		glBindVertexArray(lightArray);
		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("light");

		// default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// clear old frame
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProg[2]);
		checkGLError("render tex2");
		glUniform2f(glGetUniformLocation(shaderProg[2], "resolution"), RESOLUTION, RESOLUTION);
		glUniform1f(glGetUniformLocation(shaderProg[2], "animationTime"), state.getAnimationTime());
		glActiveTexture(GL_TEXTURE0 + 0); // this 0 should match 0 in 2 lines
		glBindTexture(GL_TEXTURE_2D, renderTexture);
		glUniform1i(glGetUniformLocation(shaderProg[2], "texId"), 0);

		glBindVertexArray(quadVertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("render texture");
	}

	void buildRenderBuffers(size_t xRes, size_t yRes)
	{
		if(renderTexture != 0) {
			glDeleteTextures(1, &renderTexture);
			glDeleteRenderbuffers(1, &renderBuffer);
		}

		// framebuffer
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		// make renderbuffer for depth, attach
		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, xRes, yRes);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

		// make texture
		glGenTextures(1, &renderTexture);
		glBindTexture(GL_TEXTURE_2D, renderTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xRes, yRes, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		// attach texture to framebuffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
		GLenum colorBuffer = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &colorBuffer);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			cout << "Frame buffer setup failed\n";
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		checkGLError("frame buffer");
	}


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
		shaderProg[0] = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);
        
        
        char const * lightVPath = "resources/lightPos.vert";
        char const * lightFPath = "resources/lightPos.frag";
        shaderProg[1] = ShaderManager::shaderFromFile(&lightVPath, &lightFPath, 1, 1);

		char const * texVPath = "resources/texture.vert";
		char const * texFPath = "resources/texture.frag";
		shaderProg[2] = ShaderManager::shaderFromFile(&texVPath, &texFPath, 1, 1);


		checkGLError("shader");
	}

	void setupTextures(ModelManager & models)
	{
		const int numTextures = 5;

		GLuint tempTextures[numTextures];
		glGenTextures(numTextures, tempTextures);
		sf::Image image;


		char const * imagePaths[numTextures] = {"resources/transparentcorn.png", "resources/dirt.jpg", "resources/redpixel.png", "resources/yellowpixel.png", "resources/sunset.jpg"};

		for(int i = 0 ; i < numTextures ; ++i)
		{
			models.addTexture(tempTextures[i]);
			if(!image.loadFromFile(imagePaths[i])) {
				cerr << "Could not load: " << imagePaths[i] << endl;
				exit(2);
			}
			int texSizeX = image.getSize().x;
			int texSizeY = image.getSize().y;
			unsigned char * texData = (unsigned char*) image.getPixelsPtr();

			glBindTexture(GL_TEXTURE_2D, tempTextures[i]);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSizeX, texSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
			bool mipmapEnabled = true;
			if(mipmapEnabled)
			{
				glGenerateMipmap(GL_TEXTURE_2D);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
		checkGLError("texture");
	}

	void setupBuffers(ModelManager & models)
	{
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
		
		GLuint positionBuffer;
		GLuint texCoordBuffer;
		GLuint elementBuffer;
        GLint texCoordSlot;
        GLint positionSlot;
		
		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		vector<GLfloat> pos = models.getPosition();
		glBufferData(GL_ARRAY_BUFFER, models.getPositionBytes(), &pos[0], GL_STATIC_DRAW);
		positionSlot = glGetAttribLocation(shaderProg[0], "pos");
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Do the same thing for the color data
		glGenBuffers(1, &texCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		vector<GLfloat> coords = models.getTexCoord();
		glBufferData(GL_ARRAY_BUFFER, models.getTexCoordBytes(), &coords[0], GL_STATIC_DRAW);
		texCoordSlot =    glGetAttribLocation(shaderProg[0], "texCoord");
		glEnableVertexAttribArray(texCoordSlot);
		glVertexAttribPointer(texCoordSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// now the elements
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		vector<GLuint> ele = models.getElement();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, models.getElementBytes(), &ele[0], GL_STATIC_DRAW);
		//leave the element buffer active
        
		// quad for render to texture
		static const GLfloat quadVertexData[] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
		};
		

		checkGLError("setupx1");

		glGenVertexArrays(1, &quadVertexArray);
		glBindVertexArray(quadVertexArray);
		GLuint quadVertexBuffer;
		glGenBuffers(1, &quadVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);
		checkGLError("setupx3");
		glEnableVertexAttribArray(glGetAttribLocation(shaderProg[2], "pos"));
		glVertexAttribPointer(glGetAttribLocation(shaderProg[2], "pos"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		

		checkGLError("setupx2");

		//hacky way to draw the light
        glGenVertexArrays(1, &lightArray);
        glBindVertexArray(lightArray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBindVertexArray(0);

		checkGLError("setup");
	}
};

#endif