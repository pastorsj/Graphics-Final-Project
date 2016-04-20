#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#pragma once
#include "ModelManager.h"
#include "ControlState.h"

#define NUM_TRACKED_FRAMES 10
#define COLLISION_TOLERANCE 0.2f

class WorldState
{
private:
	ControlState *moveState;

	float frameTimes[NUM_TRACKED_FRAMES];
	float currentTime;
	bool running;
	ModelManager * models;
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

	bool modelCapture;
	float animationTime;

	int lastModelFound;
	int modelCount;

	int overlayId;

	GLfloat TRANSLATION_SENSITIVITY;

public:
	WorldState();

	void init();

	ControlState * getControlState();

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

	void checkModel();

	void foundModel(unsigned i);

	void checkEndGame();

	void riseUp();

	void fallDown();

	void updateCameraAngle();

	void updateCamera();

	void toggleModelRotate();

	void toggleLightRotate();

	float getAnimationTime();

	int getOverlayId();

	void setOverlayId(int nextId);

	void handleButtonPress();
};

#endif
