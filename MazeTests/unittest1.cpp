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

	};
}