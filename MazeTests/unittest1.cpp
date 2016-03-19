#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/MazeGenerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MazeTests
{		
	TEST_CLASS(MazeGenerator1)
	{
	public:
		
		TEST_METHOD(TestInitialization)
		{
			MazeGenerator * mg = MazeGenerator::instance();
			Assert::IsNotNull(mg);
		}

		TEST_METHOD(TestMazeSizeGetters)
		{
			MazeGenerator * mg = MazeGenerator::instance();
			int expectedXSize = 20;
			int expectedYSize = 20;
			mg->makeMaze(expectedXSize, expectedYSize);
			Assert::AreEqual(expectedXSize, mg->getXSize());
			Assert::AreEqual(expectedYSize, mg->getYSize());
		}

	};
}