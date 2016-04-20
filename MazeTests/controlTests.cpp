#include "stdafx.h"
#include "CppUnitTest.h"
#include "../WunderMaze/ControlState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace controlTests
{
	TEST_CLASS(ControlState1)
	{
	public:
		TEST_METHOD(TestInitialization)
		{
			ControlState state;
		}
		
		TEST_METHOD(TestStep)
		{
			ModelManager manager;
			manager.init();
			ControlState state;
			state.init(&manager);

			Assert::AreEqual(1.0f, state.getXPos());
			Assert::AreEqual(1.5f, state.getYPos());

			state.step(true);

			Assert::AreEqual(1.07f, state.getXPos());
			Assert::AreEqual(1.5f, state.getYPos());

			

			//state.step(false);

			//Assert::AreEqual(1.07f - cos(0.07f) * 0.07f, state.getXPos());
			//Assert::AreEqual(1.5f - sin(0.07f) * 0.07f, state.getYPos());
		}

		TEST_METHOD(TestStepWithTurning)
		{
			ModelManager manager;
			manager.setSeed(1);
			manager.init();
			ControlState state;
			state.init(&manager);

			Assert::AreEqual(1.0f, state.getXPos());
			Assert::AreEqual(1.5f, state.getYPos());

			state.turnRight(); //Angle is by positive rotation_sensitivity

			state.step(false);

			float xPos1 = 1.0f - cos(0.07f) * 0.07f;
			float ypos1 = 1.5f - sin(0.07f) * 0.07f;

			Assert::AreEqual(xPos1, state.getXPos(), 0.0001f);
			Assert::AreEqual(ypos1, state.getYPos(), 0.0001f);

			state.turnLeft(); //Angle is by positive rotation_sensitivity

			state.step(false);

			Assert::AreEqual(xPos1 - 0.07f, state.getXPos(), 0.0001f);
			Assert::AreEqual(ypos1, state.getYPos(), 0.0001f);
		}

		TEST_METHOD(TestCellLocation)
		{
			ModelManager manager;
			manager.setSeed(1);
			manager.init();
			ControlState state;
			state.init(&manager);

			Assert::AreEqual(1, state.getXCell());
			Assert::AreEqual(1, state.getYCell());

			for (int i = 0; i < 8; i++)
			{
				state.step(true);
			}

			Assert::AreEqual(2, state.getXCell());
			state.step(false);
			Assert::AreEqual(1, state.getXCell());

			for (int i = 0; i < 23; i++)
			{
				state.turnRight();
			}

			for (int i = 0; i < 8; i++)
			{
				state.step(true);
			}

			Assert::AreEqual(1, state.getXCell());
			Assert::AreEqual(2, state.getYCell());
			state.step(false);
			Assert::AreEqual(1, state.getYCell());

			for (int i = 0; i < 23; i++)
			{
				state.turnLeft();
			}

			state.step(true);
			Assert::AreEqual(2, state.getXCell());

			for (int i = 0; i < 22; i++)
			{
				state.turnRight();
			}

			state.step(true);
			Assert::AreEqual(2, state.getXCell());
			Assert::AreEqual(2, state.getYCell());

		}
	};
}