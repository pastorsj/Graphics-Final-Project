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
			mg.makeMaze(expectedXSize, expectedYSize, -1);
			Assert::AreEqual(expectedXSize, mg.getXSize());
			Assert::AreEqual(expectedYSize, mg.getYSize());
		}

		TEST_METHOD(TestMazeBorders) {
			MazeGenerator mg;
			int expectedXSize = 20;
			int expectedYSize = 20;
			Assert::AreEqual(0,mg.makeMaze(expectedXSize, expectedYSize, -1));
			// Confirm left walls
			for (int i = 1; i < expectedYSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(expectedXSize - 1, i).left);
			}
			// Confirm top walls
			for (int i = 1; i < expectedXSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(i, expectedYSize - 1).up);
			}
			// Confirm inner top walls
			for (int i = 1; i < expectedXSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(i, 1).up);
			}
			// Confirm inner left walls
			for (int i = 1; i < expectedYSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(1, i).left);
			}
		}

		TEST_METHOD(TestNonsquareMazeBorders) {
			MazeGenerator mg;
			int expectedXSize = 10;
			int expectedYSize = 20;
			Assert::AreEqual(0, mg.makeMaze(expectedXSize, expectedYSize, -1));
			// Confirm left walls
			for (int i = 1; i < expectedYSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(expectedXSize - 1, i).left);
			}
			// Confirm top walls
			for (int i = 1; i < expectedXSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(i, expectedYSize - 1).up);
			}
			// Confirm inner top walls
			for (int i = 1; i < expectedXSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(i, 1).up);
			}
			// Confirm inner left walls
			for (int i = 1; i < expectedYSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(1, i).left);
			}
		}

		TEST_METHOD(TestMinMazeSize) {
			MazeGenerator mg;
			int expectedXSize = 5;
			int expectedYSize = 5;
			Assert::AreEqual(0, mg.makeMaze(expectedXSize, expectedYSize, -1));
			// Confirm left walls
			for (int i = 1; i < expectedYSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(expectedXSize - 1, i).left);
			}
			// Confirm top walls
			for (int i = 1; i < expectedXSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(i, expectedYSize - 1).up);
			}
			// Confirm inner top walls
			for (int i = 1; i < expectedXSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(i, 1).up);
			}
			// Confirm inner left walls
			for (int i = 1; i < expectedYSize - 1; ++i) {
				Assert::AreEqual(true, mg.getCell(1, i).left);
			}
		}

		TEST_METHOD(TestMazeXSizeTooSmall) {
			MazeGenerator mg;
			int expectedXSize = 4;
			int expectedYSize = 5;
			Assert::AreEqual(1, mg.makeMaze(expectedXSize, expectedYSize, -1));
		}

		TEST_METHOD(TestMazeYSizeTooSmall) {
			MazeGenerator mg;
			int expectedXSize = 5;
			int expectedYSize = 4;
			Assert::AreEqual(1, mg.makeMaze(expectedXSize, expectedYSize, -1));
		}

	};
}