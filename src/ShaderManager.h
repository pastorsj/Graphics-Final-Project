#ifndef __SHADERMANAGER_
#define __SHADERMANAGER_

#include <SFML/Window.hpp>
#include <stdio.h>

class ShaderManager
{
public:
	
	static GLuint shaderFromFile(char const * const * vertShaderPath, char const * const * fragShaderPath, int const vertShaderCount, int const fragShaderCount);
	
	static GLuint shaderFromString(char const * const * vertShaderSource, char const * const * fragShaderSource, int const vertShaderCount, int const fragShaderCount);
	
private:
	static GLuint readAndBuildProgram(char const * const * vertShaderPath, char const * const * fragShaderPath, int const vertShaderCount, int const fragShaderCount);
	
	static GLuint createShaderProgram(char const * const * vertSource, char const * const * fragSource, int const vertShaderCount, int const fragShaderCount);
	
	static void printShaderInfoLog(GLuint obj, int const shaderCount);
	
	static void printProgramInfoLog(GLuint obj, int const vertShaderCount, int const fragShaderCount);
	
	static FILE* sfopen(const char* name, const char *mode);
	
	static char* readFile(char const * fileName);
};
#endif
