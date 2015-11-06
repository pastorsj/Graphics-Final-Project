#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#include "Model.h"

#define NUM_TRACKED_FRAMES 10

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	float currentTime;
	float cameraAngle;
	bool running;
	Model model;
    int shadingMode;
	
    glm::vec3 cameraPos;
    glm::vec3 cameraLook;
    glm::vec3 cameraUp;
    
    glm::vec4 lightPos;
    glm::vec3 lightIntensity;
    
    glm::mat4 lightRotate;
    glm::mat4 lightIncrement;
    glm::mat4 modelRotate;
    glm::mat4 modelIncrement;
    glm::mat4 modelTranslate;
    glm::mat4 cameraMatrix;

	glm::mat4 cameraRotate;
	glm::mat4 cameraIncrement;
	glm::mat4 cameraTranslate;
	
	bool lightRotating;
	bool modelRotating;

	bool movingForward;
	bool movingBackward;
	bool rotatingLeft;
	bool rotatingRight;

	GLfloat TRANSLATION_SENSITIVITY;
	float ROTATION_SENSITIVITY;
	// The cell within the maze that the player is currently in
	int xCell;
	int yCell;
	// The x and y position of the player/camera within a cell. This assumes that 0,0 corresponds to the top left corner of a cell, while 1,1 is the bottom right
	float xPos;
	float yPos;

public:
	WorldState()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;

		xCell = 1;
		yCell = 1;
		xPos = 0;
		yPos = 0;
		TRANSLATION_SENSITIVITY = 0.03f;
		ROTATION_SENSITIVITY = 0.03f;

		cameraRotate = glm::mat4(1);
		cameraIncrement = glm::rotate(glm::mat4(1), 0.02f, glm::vec3(0, 1, 0));
        
        shadingMode = 0;
		running = true;
		model = Model();
        model.init();
		
		cameraAngle = 0;
		//cameraPos = glm::vec3(center[0],camDistance*std::max(xsize,ysize),center[2]);
		cameraPos = glm::vec3(0, 0, 0);
        cameraLook = glm::vec3(1,0,0);
        cameraUp = glm::vec3(0,1,0);
	}
	
	void updateFrameTime(float timeAsSeconds)
	{
		for(size_t i=1; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = frameTimes[i-1];
		frameTimes[0] = timeAsSeconds;
	}
	
	void printFPS() const
	{
		float sum = 0.0f;
		float avg = 0.0f;
		float fps = 0.0f;
		
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			sum += frameTimes[i];
		
		avg = sum / NUM_TRACKED_FRAMES;
		fps = 1.0f / avg;
		printf("fps %f\n", fps);
	}

	void printMotionState() const
	{
		printf(movingForward ? "W" : "w");
		printf(movingBackward ? "S" : "s");
		printf(rotatingLeft ? "A" : "a");
		printf(rotatingRight ? "D" : "d");
	}
	
	Model const & getModel() const
	{ return model; }
	
	void setRunning(bool r)
	{ running = r; }

	bool isRunning() const
	{ return running; }

	float getCurrentTime() const
	{ return this->currentTime; }

	void timeStep(float t)
	{
		float elapsed = t - this->currentTime;
		this->updateFrameTime(elapsed);
		
		//spin light
		if(lightRotating)
			lightRotate = lightIncrement * lightRotate;
        
        //spin model
		if(modelRotating)
			modelRotate = modelIncrement * modelRotate;

		//move camera
		if (movingForward)
			stepForward();

		if (movingBackward)
			stepBackward();

		if (rotatingLeft)
			turnLeft();

		if (rotatingRight)
			turnRight();

		updateCameraAngle();
		updateCameraPosition();
		
		this->currentTime = t;
	}
	
	Model & getModel()
	{ return model; }
	
	glm::mat4 getModelTranslate() const
	{ return modelTranslate; }
    
    glm::mat4 getModelRotate() const
    { return modelRotate; }
    
    glm::mat4 getLightRotate() const
    { return lightRotate; }
    
    glm::vec4 getLightPos() const
    { return this->lightPos; }
    
    glm::vec3 getLightIntensity() const
    { return this->lightIntensity; }
    
    glm::mat4 getCameraMatrix() const
    { return glm::lookAt(cameraPos, cameraLook, cameraUp);; }
    
    void setShadingMode(int m)
    { this->shadingMode = m; }
    
    int getShadingMode() const
    { return this->shadingMode; }
    
    glm::vec4 getCameraPos() const
    { return glm::vec4(this->cameraPos, 1); }

	glm::vec4 getCameraLook() const
	{
		return glm::vec4(this->cameraLook, 1);
	}
	float getCameraAngle() const
	{
		return cameraAngle;
	}

	void stepForward()
	{
		/*cameraPos += glm::vec3(cameraLook[0] * TRANSLATION_SENSITIVITY, 
			cameraLook[1] * TRANSLATION_SENSITIVITY, 
			cameraLook[2] * TRANSLATION_SENSITIVITY); */
		xPos += cameraLook[0] * TRANSLATION_SENSITIVITY;
		yPos += cameraLook[2] * TRANSLATION_SENSITIVITY;

		if (xPos >= 1) {
			xPos = 0;
			xCell++;
		}
		if (yPos >= 1) {
			yPos = 0;
			yCell++;
		}
	}

	void stepBackward()
	{
		/*cameraPos -= glm::vec3(cameraLook[0] * TRANSLATION_SENSITIVITY,
			cameraLook[1] * TRANSLATION_SENSITIVITY,
			cameraLook[2] * TRANSLATION_SENSITIVITY); */
		xPos -= cameraLook[0] * TRANSLATION_SENSITIVITY;
		yPos -= cameraLook[2] * TRANSLATION_SENSITIVITY;

		if (xPos <= 0) {
			xPos = 1;
			xCell--;
		}
		if (yPos <= 0) {
			yPos = 1;
			yCell--;
		}
	}

	void turnRight()
	{
		cameraAngle += ROTATION_SENSITIVITY;
		if (cameraAngle > 2 * PI)
			cameraAngle = 0;
		//cameraRotate = -cameraIncrement * cameraRotate;
	}

	void turnLeft()
	{
		cameraAngle -= ROTATION_SENSITIVITY;
		if (cameraAngle < 0)
			cameraAngle = 2 * PI;
		//cameraRotate = cameraIncrement * cameraRotate;
	}

	void updateCameraAngle(){
		//glm::mat3 rotation = glm::mat3(cos(cameraAngle), 0, sin(cameraAngle), 0, 1, 0, -sin(cameraAngle), 0, cos(cameraAngle));
		//glm::vec3 baseVec = glm::vec3(1, 0, 0);
		cameraLook = glm::vec3(cos(cameraAngle) * 2 * PI, cameraLook[1], sin(cameraAngle) * 2 * PI);
		//cameraLook = rotation * baseVec;
		//glm::vec4 rot = cameraRotate * glm::vec4(1, 0, 0, 0);
		//cameraLook = glm::vec3(rot[0], rot[1], rot[2]);
	}

	void updateCameraPosition() {
		cameraPos = glm::vec3(xCell + xPos, cameraPos[1], yCell + yPos);
	}
	
	void toggleModelRotate()
	{ modelRotating = !modelRotating; }
	
	void toggleLightRotate()
	{ lightRotating = !lightRotating; }

	void setMovingForward(bool f)
	{
		movingForward = f;
	}

	bool isMovingForward() const
	{
		return movingForward;
	}

	void setMovingBackward(bool b)
	{
		movingBackward = b;
	}

	bool isMovingBackward() const
	{
		return movingBackward;
	}

	void setRotatingLeft(bool l)
	{
		rotatingLeft = l;
	}

	bool isRotatingLeft() const
	{
		return rotatingLeft;
	}

	void setRotatingRight(bool r)
	{
		rotatingRight = r;
	}

	bool isRotatingRight() const
	{
		return rotatingRight;
	}
};

#endif
