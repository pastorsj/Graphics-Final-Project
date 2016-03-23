#include "WorldState.h"


WorldState::WorldState()
{
	for (size_t i = 0; i < NUM_TRACKED_FRAMES; i++)
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
	models.init();

	cameraAngle = 0;
	//cameraPos = glm::vec3(center[0],camDistance*std::max(xsize,ysize),center[2]);
	cameraPos = glm::vec3(1, 0, 1);
	//cameraPos = glm::vec3(4, 10, 4);
	cameraLook = glm::vec3(1, 0, 0);
	cameraUp = glm::vec3(0, 1, 0);

	MazeGenerator * mg = MazeGenerator::instance();

	for (int i = 0; i < mg->getXSize(); i++) {
		for (int j = 0; j < mg->getYSize(); j++) {
			printf("x: %i, y: %i", i, j);
			MazeGenerator * mg = MazeGenerator::instance();
			printf(mg->getCell(i, j).up ? "up" : "no up");
			printf(mg->getCell(i, j).left ? "left\n" : "no left\n");
		}
	}
}

void WorldState::updateFrameTime(float timeAsSeconds)
{
	for (size_t i = 1; i < NUM_TRACKED_FRAMES; i++)
		frameTimes[i] = frameTimes[i - 1];
	frameTimes[0] = timeAsSeconds;
}

void WorldState::printFPS() const
{
	float sum = 0.0f;
	float avg = 0.0f;
	float fps = 0.0f;

	for (size_t i = 0; i < NUM_TRACKED_FRAMES; i++)
		sum += frameTimes[i];

	avg = sum / NUM_TRACKED_FRAMES;
	fps = 1.0f / avg;
	printf("fps %f\n", fps);
}

void WorldState::printMotionState() const
{
	printf(movingForward ? "W" : "w");
	printf(movingBackward ? "S" : "s");
	printf(rotatingLeft ? "A" : "a");
	printf(rotatingRight ? "D" : "d");
}

void WorldState::setRunning(bool r)
{
	running = r;
}

bool WorldState::isRunning()
{
	return running;
}

float WorldState::getCurrentTime()
{
	return this->currentTime;
}

void WorldState::timeStep(float t)
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

ModelManager & WorldState::getModels()
{
	return models;
}

glm::mat4 WorldState::getModelTranslate() const
{
	return modelTranslate;
}

glm::mat4 WorldState::getModelRotate() const
{
	return modelRotate;
}

glm::mat4 WorldState::getLightRotate() const
{
	return lightRotate;
}

glm::vec4 WorldState::getLightPos() const
{
	return this->lightPos;
}

glm::vec3 WorldState::getLightIntensity() const
{
	return this->lightIntensity;
}

glm::mat4 WorldState::getCameraMatrix() const
{
	return glm::lookAt(cameraPos, cameraLook, cameraUp);;
}

void WorldState::setShadingMode(int m)
{
	this->shadingMode = m;
}

int WorldState::getShadingMode() const
{
	return this->shadingMode;
}

glm::vec4 WorldState::getCameraPos() const
{
	return glm::vec4(this->cameraPos, 1);
}

glm::vec4 WorldState::getCameraLook() const
{
	return glm::vec4(this->cameraLook, 1);
}
float WorldState::getCameraAngle()
{
	return cameraAngle;
}

void WorldState::step(bool forward) {
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

void WorldState::turnRight()
{
	cameraAngle += ROTATION_SENSITIVITY;
	if (cameraAngle > 2 * PI)
		cameraAngle = 0;
	//cameraRotate = -cameraIncrement * cameraRotate;
}

void WorldState::turnLeft()
{
	cameraAngle -= ROTATION_SENSITIVITY;
	if (cameraAngle < 0)
		cameraAngle = 2 * PI;
	//cameraRotate = cameraIncrement * cameraRotate;
}

void WorldState::checkModel()
{
	for (unsigned i = 2; i < models.getRawModels().size(); i++) {
		Model m = models.getRawModels().at(i);
		if (m.getXcell() == xCell && m.getYcell() == yCell && !m.hasBeenFound()) {
			foundModel(i);
		}
	}

}

void WorldState::foundModel(unsigned i)
{

	modelCount++;
	lastModelFound = i;
	models.getRawModels().at(i).find();
	modelCapture = true;
	checkEndGame();
}

void WorldState::checkEndGame(){
	if (modelCount == models.getNumObjects()) {
		printf("You Win!");

	}
}

void WorldState::riseUp()
{
	cameraPos = glm::vec3(getXPos(), cameraPos[1] += TRANSLATION_SENSITIVITY, getYPos());
	cameraLook[1] += TRANSLATION_SENSITIVITY;
}

void WorldState::fallDown()
{
	cameraPos = glm::vec3(getXPos(), 0, getYPos());
	cameraLook[1] = 0;
}

void WorldState::updateCameraAngle(){
	//glm::mat3 rotation = glm::mat3(cos(cameraAngle), 0, sin(cameraAngle), 0, 1, 0, -sin(cameraAngle), 0, cos(cameraAngle));
	//glm::vec3 baseVec = glm::vec3(1, 0, 0);
	cameraLook = glm::vec3(cos(cameraAngle) * 2 * PI, 0, sin(cameraAngle) * 2 * PI);
	//cameraLook = rotation * baseVec;
	//glm::vec4 rot = cameraRotate * glm::vec4(1, 0, 0, 0);
	//cameraLook = glm::vec3(rot[0], rot[1], rot[2]);
}

void WorldState::updateCamera() {
	cameraPos = glm::vec3(getXPos(), cameraPos[1], getYPos());
	cameraLook = glm::vec3(getXPos() + cos(cameraAngle), cameraLook[1], getYPos() + sin(cameraAngle));
}

void WorldState::toggleModelRotate()
{
	modelRotating = !modelRotating;
}

void WorldState::toggleLightRotate()
{
	lightRotating = !lightRotating;
}

void WorldState::setMovingForward(bool f)
{
	movingForward = f;
}

bool WorldState::isMovingForward() const
{
	return movingForward;
}

void WorldState::setMovingBackward(bool b)
{
	movingBackward = b;
}

bool WorldState::isMovingBackward() const
{
	return movingBackward;
}

void WorldState::setRotatingLeft(bool l)
{
	rotatingLeft = l;
}

bool WorldState::isRotatingLeft() const
{
	return rotatingLeft;
}

void WorldState::setRotatingRight(bool r)
{
	rotatingRight = r;
}

bool WorldState::isRotatingRight() const
{
	return rotatingRight;
}

void WorldState::setRise(bool r)
{
	rise = r;
}

bool WorldState::isRising()
{
	return rise;
}

void WorldState::setFall(bool f)
{
	fall = f;
}

bool WorldState::isFalling()
{
	return fall;
}

float WorldState::getXPos() {
	return xCell + xPos;
}

float WorldState::getYPos() {
	return yCell + yPos;
}

int WorldState::getXCell() {
	return xCell;
}

int WorldState::getYCell() {
	return yCell;
}

float WorldState::getXPosComp(){
	return xPos;
}

float WorldState::getYPosComp(){
	return yPos;
}

float WorldState::getAnimationTime() {
	return animationTime;
}