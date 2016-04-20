#include "ControlState.h"

void ControlState::init(ModelManager * mods)
{
	models = mods;
	mg = models->getMazeGenerator();
	//models->init();
	TRANSLATION_SENSITIVITY = 0.07f;
	ROTATION_SENSITIVITY = 0.07f;
	cameraAngle = 0;
	xCell = 1;
	yCell = 1;
	xPos = 0;
	yPos = 0.5;
	rise = false;
	fall = false;
}

float ControlState::getCameraAngle()
{
	return cameraAngle;
}

void ControlState::step(bool forward) {
	float xInterval = cos(cameraAngle) * TRANSLATION_SENSITIVITY;
	float yInterval = sin(cameraAngle) * TRANSLATION_SENSITIVITY;

	if (!forward) {
		xInterval = -xInterval;
		yInterval = -yInterval;
	}

	//printf("xCell: %i, yCell: %i\n", xCell, yCell);
	//printf("xPos: %f, yPos: %f\n", xPos, yPos)
	
	if (xInterval > 0) {
		if (!mg.getCell(xCell + 1, yCell).left || xPos < 0.5 - COLLISION_TOLERANCE) {
			xPos += xInterval;
		}
	}
	if (xInterval < 0) {
		if (!mg.getCell(xCell, yCell).left || xPos > COLLISION_TOLERANCE - 0.5) {
			xPos += xInterval;
		}
	}

	if (xPos <= -0.5) {
		xPos = 0.5;
		xCell--;
	}
	else if (xPos >= 0.5) {
		xPos = -0.5;
		xCell++;
	}

	if (yInterval > 0 && (!mg.getCell(xCell, yCell + 1).up || yPos < 1 - COLLISION_TOLERANCE)) {
		yPos += yInterval;
	}

	if (yInterval < 0 && (!mg.getCell(xCell, yCell).up || yPos > COLLISION_TOLERANCE)) {
		yPos += yInterval;
	}

	if (yPos <= 0) {
		yPos = 1;
		yCell--;
	}
	else if (yPos >= 1) {
		yPos = -0;
		yCell++;
	}

	//printf("xPos: %f, yPos: %f\n", getXPos(), getYPos());
}

void ControlState::turnRight()
{
	cameraAngle += ROTATION_SENSITIVITY;
	if (cameraAngle > 2 * PI)
		cameraAngle = 0;
	//cameraRotate = -cameraIncrement * cameraRotate;
}

void ControlState::turnLeft()
{
	cameraAngle -= ROTATION_SENSITIVITY;
	if (cameraAngle < 0)
		cameraAngle = 2 * PI;
	//cameraRotate = cameraIncrement * cameraRotate;
}

void ControlState::setMovingForward(bool f)
{
	movingForward = f;
}

bool ControlState::isMovingForward() const
{
	return movingForward;
}

void ControlState::setMovingBackward(bool b)
{
	movingBackward = b;
}

bool ControlState::isMovingBackward() const
{
	return movingBackward;
}

void ControlState::setRotatingLeft(bool l)
{
	rotatingLeft = l;
}

bool ControlState::isRotatingLeft() const
{
	return rotatingLeft;
}

void ControlState::setRotatingRight(bool r)
{
	rotatingRight = r;
}

bool ControlState::isRotatingRight() const
{
	return rotatingRight;
}

void ControlState::setRise(bool r)
{
	rise = r;
}

bool ControlState::isRising()
{
	return rise;
}

void ControlState::setFall(bool f)
{
	fall = f;
}

bool ControlState::isFalling()
{
	return fall;
}

float ControlState::getXPos() {
	return xCell + xPos;
}

float ControlState::getYPos() {
	return yCell + yPos;
}

int ControlState::getXCell() {
	return xCell;
}

int ControlState::getYCell() {
	return yCell;
}

float ControlState::getXPosComp() {
	return xPos;
}

float ControlState::getYPosComp() {
	return yPos;
}