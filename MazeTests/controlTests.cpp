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

			//state.turnRight(); //Angle is by positive rotation_sensitivity

			//state.step(false);

			//Assert::AreEqual(1.07f - cos(0.07f) * 0.07f, state.getXPos());
			//Assert::AreEqual(1.5f - sin(0.07f) * 0.07f, state.getYPos());
		}
	};
}