#include "WorldState.h"


WorldState::WorldState()
{

}

void WorldState::init(ModelManager * mods, ControlState * conState)
{
	for (size_t i = 0; i < NUM_TRACKED_FRAMES; i++)
		frameTimes[i] = 0.0f;
	models = mods;
	models->init();

	moveState = conState;
	conState->init(models);

	TRANSLATION_SENSITIVITY = 0.07f;
	modelCapture = false;
	moveState->setRise(false);;
	moveState->setFall(false);
	animationTime = -1;
	this->overlayId = 1;

	modelCount = 0;

	cameraRotate = glm::mat4(1);
	cameraIncrement = glm::rotate(glm::mat4(1), 0.02f, glm::vec3(0, 1, 0));

	modelIncrement = glm::rotate(glm::mat4(1.0f), 0.02f, glm::vec3(0, 1, 0));

	shadingMode = 0;
	running = true;
	

	//cameraPos = glm::vec3(center[0],camDistance*std::max(xsize,ysize),center[2]);
	cameraPos = glm::vec3(0, 0, 0);
	//cameraPos = glm::vec3(4, 10, 4);
	cameraLook = glm::vec3(1, 0, 0);
	cameraUp = glm::vec3(0, 1, 0);

	MazeGenerator mg = models->getMazeGenerator();

	for (int i = 0; i < mg.getXSize(); i++) {
		for (int j = 0; j < mg.getYSize(); j++) {
			printf("x: %i, y: %i", i, j);
			printf(mg.getCell(i, j).up ? "up" : "no up");
			printf(mg.getCell(i, j).left ? "left\n" : "no left\n");
		}
	}
}

ControlState * WorldState::getControlState()
{
	return moveState;
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
	//printf(movingForward ? "W" : "w");
	//printf(movingBackward ? "S" : "s");
	//printf(rotatingLeft ? "A" : "a");
	//printf(rotatingRight ? "D" : "d");
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
	if (getControlState()->isMovingForward() && !modelCapture) {
		getControlState()->step(true);
		checkModel();
	}

	if (getControlState()->isMovingBackward() && !modelCapture){
		getControlState()->step(false);
		checkModel();
	}

	if (getControlState()->isRotatingLeft() && !modelCapture)
		getControlState()->turnLeft();

	if (getControlState()->isRotatingRight() && !modelCapture)
		getControlState()->turnRight();

	if (getControlState()->isRising() && !modelCapture)
		riseUp();

	if (getControlState()->isFalling() && !modelCapture)
		fallDown();

	if (modelCapture) {
		animationTime += 1;
		if (animationTime > 60) {
			modelCapture = false;
			animationTime = -1;
			models->getRawModels().at(lastModelFound)->setTexNum(3);
		}
	}

	updateCamera();

	this->currentTime = t;
}

ModelManager & WorldState::getModels()
{
	return *models;
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

void WorldState::checkModel()
{
	for (unsigned i = 2; i < models->getRawModels().size(); i++) {
		Model* m = models->getRawModels().at(i);
		if (m->getXcell() == getControlState()->getXCell() && m->getYcell() == getControlState()->getYCell() && !m->hasBeenFound()) {
			foundModel(i);
		}
	}

}

void WorldState::foundModel(unsigned i)
{

	modelCount++;
	lastModelFound = i;
	models->getRawModels().at(i)->find();
	modelCapture = true;
	checkEndGame();
}

void WorldState::checkEndGame() {
	if (modelCount == models->getNumObjects()) {
		printf("You Win!");
		moveState->setMovingForward(false);
		moveState->setMovingBackward(false);
		moveState->setRotatingLeft(false);
		moveState->setRotatingRight(false);
		moveState->setRise(false);
		moveState->setFall(false);
		this->setOverlayId(2);
	}
}

void WorldState::riseUp()
{
	cameraPos = glm::vec3(getControlState()->getXPos(), cameraPos[1] += TRANSLATION_SENSITIVITY, getControlState()->getYPos());
	cameraLook[1] += TRANSLATION_SENSITIVITY;
}

void WorldState::fallDown()
{
	cameraPos = glm::vec3(getControlState()->getXPos(), 0, getControlState()->getYPos());
	cameraLook[1] = 0;
}

void WorldState::updateCameraAngle() {
	//glm::mat3 rotation = glm::mat3(cos(cameraAngle), 0, sin(cameraAngle), 0, 1, 0, -sin(cameraAngle), 0, cos(cameraAngle));
	//glm::vec3 baseVec = glm::vec3(1, 0, 0);
	cameraLook = glm::vec3(cos(getControlState()->getCameraAngle()) * 2 * PI, 0, sin(getControlState()->getCameraAngle()) * 2 * PI);
	//cameraLook = rotation * baseVec;
	//glm::vec4 rot = cameraRotate * glm::vec4(1, 0, 0, 0);
	//cameraLook = glm::vec3(rot[0], rot[1], rot[2]);
}

void WorldState::updateCamera() {
	cameraPos = glm::vec3(getControlState()->getXPos(), cameraPos[1], getControlState()->getYPos());
	cameraLook = glm::vec3(getControlState()->getXPos() + cos(getControlState()->getCameraAngle()), cameraLook[1], getControlState()->getYPos() + sin(getControlState()->getCameraAngle()));
}

void WorldState::toggleModelRotate()
{
	modelRotating = !modelRotating;
}

void WorldState::toggleLightRotate()
{
	lightRotating = !lightRotating;
}

float WorldState::getAnimationTime() {
	return animationTime;
}

int WorldState::getOverlayId() {
	return this->overlayId;
}

void WorldState::setOverlayId(int nextId) {
	this->overlayId = nextId;
}

void WorldState::handleButtonPress() {
	if (this->overlayId == 1) {
		this->overlayId = 0;
	}
	else if (this->overlayId == 2) {
		this->setRunning(false);
	}
}