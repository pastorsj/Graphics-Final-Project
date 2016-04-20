#ifndef __CONTROLSTATE
#define __CONTROLSTATE
#pragma once
#include "ModelManager.h"

#define NUM_TRACKED_FRAMES 10
#define COLLISION_TOLERANCE 0.2f

class ControlState {

private:
	ModelManager * models;
	MazeGenerator mg;

	bool movingForward;
	bool movingBackward;
	bool rotatingLeft;
	bool rotatingRight;
	bool rise;
	bool fall;

	GLfloat TRANSLATION_SENSITIVITY;
	float ROTATION_SENSITIVITY;

	float cameraAngle;

	// The cell within the maze that the player is currently in
	int xCell;
	int yCell;
	// The x and y position of the player/camera within a cell. This assumes that 0,0 corresponds to the top left corner of a cell, while 1,1 is the bottom right
	float xPos;
	float yPos;

public:
	void init(ModelManager * mods);

	float getCameraAngle();

	void step(bool forward);

	void turnRight();

	void turnLeft();

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
};
#endif
