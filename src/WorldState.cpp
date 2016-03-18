#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#include "WorldState.h"
#include "ModelManager.h"



#define GL3W
#ifdef GLEW
#define GLEW_STATIC
#include "glew/glew.h"
#endif
#ifdef GL3W
#include "gl3w/gl3w.h"
#endif

#define NUM_TRACKED_FRAMES 10
#define COLLISION_TOLERANCE 0.2f

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	float currentTime;
	float cameraAngle;
	bool running;
	ModelManager models;
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
	bool rise;
	bool fall;
	bool modelCapture;
	float animationTime;

	int lastModelFound;
	int modelCount;

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
		for (size_t i = 0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;

		xCell = 1;
		yCell = 1;
		xPos = 0;
		yPos = 0.5;
		TRANSLATION_SENSITIVITY = 0.07f;
		ROTATION_SENSITIVITY = 0.07f;
		modelCapture = false;
		rise = false;
		fall = false;
		animationTime = -1;

		modelCount = 0;

		cameraRotate = glm::mat4(1);
		cameraIncrement = glm::rotate(glm::mat4(1), 0.02f, glm::vec3(0, 1, 0));

		modelIncrement = glm::rotate(glm::mat4(1.0f), 0.02f, glm::vec3(0, 1, 0));

		shadingMode = 0;
		running = true;
		models = ModelManager();
		models.init();

		cameraAngle = 0;
		//cameraPos = glm::vec3(center[0],camDistance*std::max(xsize,ysize),center[2]);
		cameraPos = glm::vec3(1, 0, 1);
		//cameraPos = glm::vec3(4, 10, 4);
		cameraLook = glm::vec3(1, 0, 0);
		cameraUp = glm::vec3(0, 1, 0);

		for (int i = 0; i < xsize; i++) {
			for (int j = 0; j < ysize; j++) {
				printf("x: %i, y: %i", i, j);
				MazeGenerator * mg = MazeGenerator::instance();
				printf(mg->getCell(i, j).up ? "up" : "no up");
				printf(mg->getCell(i, j).left ? "left\n" : "no left\n");
			}
		}
	}

	void updateFrameTime(float timeAsSeconds)
	{
		for (size_t i = 1; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = frameTimes[i - 1];
		frameTimes[0] = timeAsSeconds;
	}

	void printFPS() const
	{
		float sum = 0.0f;
		float avg = 0.0f;
		float fps = 0.0f;

		for (size_t i = 0; i<NUM_TRACKED_FRAMES; i++)
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

	void setRunning(bool r)
	{
		running = r;
	}

	bool isRunning() const
	{
		return running;
	}

	float getCurrentTime() const
	{
		return this->currentTime;
	}

	void timeStep(float t)
	{
		float elapsed = t - this->currentTime;
		this->updateFrameTime(elapsed);

		//spin light
		lightRotate = lightIncrement * lightRotate;

		//spin model
		modelRotate = modelIncrement * modelRotate;

		//move camera
		if (movingForward && !modelCapture)
			step(true);

		if (movingBackward && !modelCapture)
			step(false);

		if (rotatingLeft && !modelCapture)
			turnLeft();

		if (rotatingRight && !modelCapture)
			turnRight();

		if (rise && !modelCapture)
			riseUp();

		if (fall && !modelCapture)
			fallDown();

		if (modelCapture) {
			animationTime += 1;
			if (animationTime > 60) {
				modelCapture = false;
				animationTime = -1;
				models.getRawModels().at(lastModelFound).setTexNum(3);
			}
		}

		updateCamera();

		this->currentTime = t;
	}

	ModelManager & getModels()
	{
		return models;
	}

	glm::mat4 getModelTranslate() const
	{
		return modelTranslate;
	}

	glm::mat4 getModelRotate() const
	{
		return modelRotate;
	}

	glm::mat4 getLightRotate() const
	{
		return lightRotate;
	}

	glm::vec4 getLightPos() const
	{
		return this->lightPos;
	}

	glm::vec3 getLightIntensity() const
	{
		return this->lightIntensity;
	}

	glm::mat4 getCameraMatrix() const
	{
		return glm::lookAt(cameraPos, cameraLook, cameraUp);;
	}

	void setShadingMode(int m)
	{
		this->shadingMode = m;
	}

	int getShadingMode() const
	{
		return this->shadingMode;
	}

	glm::vec4 getCameraPos() const
	{
		return glm::vec4(this->cameraPos, 1);
	}

	glm::vec4 getCameraLook() const
	{
		return glm::vec4(this->cameraLook, 1);
	}
	float getCameraAngle() const
	{
		return cameraAngle;
	}

	void step(bool forward) {
		float xInterval = cos(cameraAngle) * TRANSLATION_SENSITIVITY;
		float yInterval = sin(cameraAngle) * TRANSLATION_SENSITIVITY;

		if (!forward) {
			xInterval = -xInterval;
			yInterval = -yInterval;
		}

		//printf("xCell: %i, yCell: %i\n", xCell, yCell);
		//printf("xPos: %f, yPos: %f\n", xPos, yPos);
		MazeGenerator * mg = MazeGenerator::instance();

		if (xInterval > 0) {
			if (!mg->getCell(xCell + 1, yCell).left || xPos < 0.5 - COLLISION_TOLERANCE){
				xPos += xInterval;
			}
		}
		if (xInterval < 0) {
			if (!mg->getCell(xCell, yCell).left || xPos > COLLISION_TOLERANCE - 0.5) {
				xPos += xInterval;
			}
		}

		if (xPos <= -0.5) {
			xPos = 0.5;
			xCell--;
			checkModel();
		}
		else
			if (xPos >= 0.5) {
			xPos = -0.5;
			xCell++;
			checkModel();
			}
		if (yInterval > 0 && (!mg->getCell(xCell, yCell + 1).up || yPos < 1 - COLLISION_TOLERANCE)) {
			yPos += yInterval;
		}
		if (yInterval < 0 && (!mg->getCell(xCell, yCell).up || yPos > COLLISION_TOLERANCE)) {
			yPos += yInterval;
		}

		if (yPos <= 0) {
			yPos = 1;
			yCell--;
			checkModel();
		}
		else
			if (yPos >= 1) {
			yPos = -0;
			yCell++;
			checkModel();
			}

		//printf("xPos: %f, yPos: %f\n", getXPos(), getYPos());
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

	void checkModel()
	{
		for (unsigned i = 2; i < models.getRawModels().size(); i++) {
			Model m = models.getRawModels().at(i);
			if (m.getXcell() == xCell && m.getYcell() == yCell && !m.hasBeenFound()) {
				foundModel(i);
			}
		}

	}

	void foundModel(unsigned i)
	{

		modelCount++;
		lastModelFound = i;
		models.getRawModels().at(i).find();
		modelCapture = true;
		checkEndGame();
	}

	void checkEndGame(){
		if (modelCount == models.getNumObjects()) {
			printf("You Win!");

		}
	}

	void riseUp()
	{
		cameraPos = glm::vec3(getXPos(), cameraPos[1] += TRANSLATION_SENSITIVITY, getYPos());
		cameraLook[1] += TRANSLATION_SENSITIVITY;
	}

	void fallDown()
	{
		cameraPos = glm::vec3(getXPos(), 0, getYPos());
		cameraLook[1] = 0;
	}

	void updateCameraAngle(){
		//glm::mat3 rotation = glm::mat3(cos(cameraAngle), 0, sin(cameraAngle), 0, 1, 0, -sin(cameraAngle), 0, cos(cameraAngle));
		//glm::vec3 baseVec = glm::vec3(1, 0, 0);
		cameraLook = glm::vec3(cos(cameraAngle) * 2 * PI, 0, sin(cameraAngle) * 2 * PI);
		//cameraLook = rotation * baseVec;
		//glm::vec4 rot = cameraRotate * glm::vec4(1, 0, 0, 0);
		//cameraLook = glm::vec3(rot[0], rot[1], rot[2]);
	}

	void updateCamera() {
		cameraPos = glm::vec3(getXPos(), cameraPos[1], getYPos());
		cameraLook = glm::vec3(getXPos() + cos(cameraAngle), cameraLook[1], getYPos() + sin(cameraAngle));
	}

	void toggleModelRotate()
	{
		modelRotating = !modelRotating;
	}

	void toggleLightRotate()
	{
		lightRotating = !lightRotating;
	}

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

	void setRise(bool r)
	{
		rise = r;
	}

	bool isRising() const
	{
		return rise;
	}

	void setFall(bool f)
	{
		fall = f;
	}

	bool isFalling() const
	{
		return fall;
	}

	float getXPos() {
		return xCell + xPos;
	}

	float getYPos() {
		return yCell + yPos;
	}

	int getXCell() {
		return xCell;
	}

	int getYCell() {
		return yCell;
	}

	float getXPosComp(){
		return xPos;
	}

	float getYPosComp(){
		return yPos;
	}

	float getAnimationTime() {
		return animationTime;
	}
};

#endif