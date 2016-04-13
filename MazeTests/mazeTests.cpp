#include "stdafx.h"
#include "CppUnitTest.h"
#include "../WunderMaze/MazeGenerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MazeTests
{		
	TEST_CLASS(MazeGenerator1)
	{
	public:
		
		TEST_METHOD(TestInitialization)
		{
			MazeGenerator mg;
		}

		TEST_METHOD(TestMazeSizeGetters)
		{
			MazeGenerator mg;
			int expectedXSize = 20;
			int expectedYSize = 20;
			mg.makeMaze(expectedXSize, expectedYSize);
			Assert::AreEqual(expectedXSize, mg.getXSize());
			Assert::AreEqual(expectedYSize, mg.getYSize());
		}

	};
}