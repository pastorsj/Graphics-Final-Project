#define printusers() printf("Program by pastorsj+persinme+porterjc+robinsat\n");

#define _CRT_SECURE_NO_WARNINGS
#define RESOLUTION 1000	//IF you change this, there is one in RenderEngine.cpp that must be changed as well
#define TARGET_FPS 30                // controls spin update rate
#define TIME_BETWEEN_UPDATES 0.015   // seconds between motion updates
#define PRINT_FPS_INTERVAL 5.0f


#ifdef __APPLE__
#include <fcntl.h>
#include <unistd.h>
#endif

#define GL3W
#ifdef GLEW
#define GLEW_STATIC
#include "glew/glew.h"
#endif
#ifdef GL3W
#include "gl3w/gl3w.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#define _USE_MATH_DEFINES
//#include <math.h>
#define PI 3.14159f
#include "ShaderManager.h"
#include "GLHelper.h"
#include "WorldState.h"
#include "ControlState.h"
#include "RenderEngine.h"


class Program4
{
public:
	void runGameLoop()
	{
		while (this->isProgramRunning) {
			getWindowContext();

			WorldState state;
			initializeStates(state);
			sf::Clock c;
			float lastFrame = c.restart().asSeconds();
			float lastPrint = lastFrame;
			float targetFrameTime = 1.0f / (float)TARGET_FPS;

			while (state.isRunning() && this->isProgramRunning)
			{
				App->setActive();
				float currentTime = c.getElapsedTime().asSeconds();
				float sinceLastFrame = currentTime - lastFrame;
				float sleepTime = targetFrameTime - sinceLastFrame;
				if (sleepTime > 0)
					sf::sleep(sf::seconds(sleepTime));

				currentTime = c.getElapsedTime().asSeconds();
				lastFrame = currentTime;
				float sinceLastPrint = currentTime - lastPrint;

				handleEvents(state, render);
				state.timeStep(currentTime);

				if (sinceLastPrint > PRINT_FPS_INTERVAL) {
					lastPrint = currentTime;
					//state.printFPS();
					//state.printMotionState();
				}

				render.display(state);
				
				App->display();
			}
			App->close();
			delete App;
		}
	}

private:
	sf::Window * App;
	RenderEngine render;

	sf::Clock timer;
	float lastUpdate;
	float motionTime;
	bool buttonDown[3];

	bool isProgramRunning = true;

	void initializeStates(WorldState & state) {
		state.init();
		render.init(state);
		render.buildRenderBuffers(RESOLUTION, RESOLUTION);

		buttonDown[0] = false;
		buttonDown[1] = false;
		buttonDown[2] = false;
		state.getControlState()->setMovingForward(false);
		state.getControlState()->setMovingBackward(false);
		state.getControlState()->setRotatingLeft(false);
		state.getControlState()->setRotatingRight(false);
	}

	void setIsProgramRunning(bool running)
	{
		isProgramRunning = running;
	}

	void handleEvents(WorldState & state, RenderEngine & render)
	{
		sf::Event event;

		while (App->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				setIsProgramRunning(false);
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				setIsProgramRunning(false);

			if ((event.type == sf::Event::TextEntered) && (event.text.unicode == 'q'))
				state.setShadingMode(0);
			if ((event.type == sf::Event::TextEntered) && (event.text.unicode == 'w'))
				state.setShadingMode(1);
			if ((event.type == sf::Event::TextEntered) && (event.text.unicode == 'e'))
				state.setShadingMode(2);
			if ((event.type == sf::Event::TextEntered) && (event.text.unicode == 'r'))
				state.toggleModelRotate();
			if ((event.type == sf::Event::TextEntered) && (event.text.unicode == 't'))
				state.toggleLightRotate();

			if ((event.type == sf::Event::MouseButtonPressed)) {
				cout << "Mouse pressed!!" << endl;
				int xpos = event.mouseButton.x;
				int ypos = event.mouseButton.y;
				cout << "Mouse position: " << xpos << ", " << ypos << endl;
				if (state.getOverlayId() == 1) {
					if (xpos > 350 && xpos < 650 && ypos > 550 && ypos < 650) {
						cout << "Mouse press in bounds!" << endl;
						state.handleButtonPress();
					}
				}
				else if (state.getOverlayId() == 2) {
					if (xpos > 350 && xpos < 650 && ypos > 650 && ypos < 750) {
						cout << "Mouse press in bounds!" << endl;
						state.handleButtonPress();
					}
				}
			}

			//Key events to move the player around
			if (state.getOverlayId() == 0) {
				if ((event.type == sf::Event::KeyPressed)) {
					if ((event.key.code == sf::Keyboard::W))
						state.getControlState()->setMovingForward(true);
					if ((event.key.code == sf::Keyboard::S))
						state.getControlState()->setMovingBackward(true);
					if ((event.key.code == sf::Keyboard::A))
						state.getControlState()->setRotatingLeft(true);
					if ((event.key.code == sf::Keyboard::D))
						state.getControlState()->setRotatingRight(true);
					if ((event.key.code == sf::Keyboard::R))
						state.getControlState()->setRise(true);
					if ((event.key.code == sf::Keyboard::F))
						state.getControlState()->setFall(true);
				}

				if ((event.type == sf::Event::KeyReleased)) {
					if ((event.key.code == sf::Keyboard::W))
						state.getControlState()->setMovingForward(false);
					if ((event.key.code == sf::Keyboard::S))
						state.getControlState()->setMovingBackward(false);
					if ((event.key.code == sf::Keyboard::A))
						state.getControlState()->setRotatingLeft(false);
					if ((event.key.code == sf::Keyboard::D))
						state.getControlState()->setRotatingRight(false);
					if ((event.key.code == sf::Keyboard::R))
						state.getControlState()->setRise(false);
					if ((event.key.code == sf::Keyboard::F))
						state.getControlState()->setFall(false);
				}
			}
		}

		/*lastUpdate = timer.getElapsedTime().asSeconds();
		bool needsUpdate = lastUpdate > TIME_BETWEEN_UPDATES;
		if (needsUpdate)
		{
		//glm::ivec2 newPos = glm::ivec2(sf::Mouse::getPosition(*App).x, sf::Mouse::getPosition(*App).y);

		if (state.isMovingForward())
		state.stepForward();

		lastUpdate = timer.restart().asSeconds();
		//previousPos = newPos;
		} */
	}

	void getWindowContext()
	{
		sf::err().rdbuf(NULL); //hide errors

#ifdef __APPLE__
		int nullFD = open("/dev/null", O_WRONLY);
		int oldFD = dup(2); // Duplicate the old file descriptor, so it can be restored
		dup2(nullFD, 2); // Redirect
#endif

		sf::VideoMode mode(RESOLUTION, RESOLUTION, 32);
#ifdef __linux__
		sf::ContextSettings settings(32, 0, 0, 3, 3);
#else
		sf::ContextSettings settings(32, 0, 0, 3, 3, sf::ContextSettings::Core);
#endif
		App = new sf::Window(mode, "WonderMaize", sf::Style::Default, settings);

#ifdef __APPLE__
		dup2(oldFD, 2); // Redirect back
		close(oldFD); // Not needed anymore
#endif
	}
};

int main()
{
	printusers();
	Program4 prog;
	prog.runGameLoop();

	return EXIT_SUCCESS;
}