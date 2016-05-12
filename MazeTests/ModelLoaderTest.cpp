#include "stdafx.h"
#include "CppUnitTest.h"
#include "../WunderMaze/Model.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ModelLoaderTest
{
	TEST_CLASS(ModelLoader1)
	{
	public:
		TEST_METHOD(TestModelInitialization)
		{
			Model model;
		}

		TEST_METHOD(TestModelLoadNotFail)
		{
			Model model;
			model.init(string("resources/testBox.obj"), 2, 0, 0, true);
		}

		TEST_METHOD(TestModelCorrectVertexOrder)
		{
			Model model;
			model.init(string("resources/testBox.obj"), 2, 0, 0, true);
			vector<glm::vec3> verts = model.getPosition();

			//Check if the first face is correct
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(1, -1, 1), verts[0])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(-1, -1, -1), verts[1])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(1, -1, -1), verts[2])));

			//Check if the second face is correct
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(1, 1, -1), verts[6])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(1, -1, 1), verts[7])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(1, -1, -1), verts[8])));

			//Check if the last face is correct
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(1, -1, -1), verts[33])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(-1, -1, -1), verts[34])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(-1, 1, -1), verts[35])));
		}

		TEST_METHOD(TestModelCorrecetNormalOrder)
		{
			Model model;
			model.init(string("resources/testBox.obj"), 2, 0, 0, true);
			vector<glm::vec3> norms = model.getNormal();

			//Check if normal for first face is correct
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(0, -1, 0), norms[0])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(0, -1, 0), norms[1])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(0, -1, 0), norms[2])));

			//Check if normal for second face is correct
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(0, 1, 0), norms[3])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(0, 1, 0), norms[4])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(0, 1, 0), norms[5])));

			//Check if normal for last face is correct
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(0, 0, -1), norms[33])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(0, 0, -1), norms[34])));
			Assert::IsTrue(glm::all(glm::equal(glm::vec3(0, 0, -1), norms[35])));
		}
	};
}