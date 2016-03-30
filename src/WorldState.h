#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#pragma once
#include "ModelManager.h"

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
	WorldState();

	void init();

	void updateFrameTime(float timeAsSeconds);

	void printFPS() const;

	void printMotionState() const;

	void setRunning(bool r);

	bool isRunning();

	float getCurrentTime();

	void timeStep(float t);

	ModelManager & getModels();

	glm::mat4 getModelTranslate() const;

	glm::mat4 getModelRotate() const;

	glm::mat4 getLightRotate() const;

	glm::vec4 getLightPos() const;

	glm::vec3 getLightIntensity() const;

	glm::mat4 getCameraMatrix() const;

	void setShadingMode(int m);

	int getShadingMode() const;

	glm::vec4 getCameraPos() const;

	glm::vec4 getCameraLook() const;

	float getCameraAngle();

	void step(bool forward);

	void turnRight();

	void turnLeft();

	void checkModel();

	void foundModel(unsigned i);

	void checkEndGame();

	void riseUp();

	void fallDown();

	void updateCameraAngle();

	void updateCamera();

	void toggleModelRotate();

	void toggleLightRotate();

	void setMovingForward(bool f);

	bool isMovingForward() const;

	void setMovingBackward(bool b);

	bool isMovingBackward() const;

	void setRotatingLeft(bool l);

	bool isRotatingLeft() const;

	void setRotatingRight(bool r);

	bool isRotatingRight() const;

	void setRise(bool r);

	bool isRising();

	void setFall(bool f);

	bool isFalling();

	float getXPos();

	float getYPos();

	int getXCell();

	int getYCell();

	float getXPosComp();

	float getYPosComp();

	float getAnimationTime();
};

#endif
