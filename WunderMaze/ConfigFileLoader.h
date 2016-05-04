#ifndef __CONFIGFILELOADER
#define __CONFIGFILELOADER
#pragma once

#include <iostream>;
#include <fstream>;
#include <sstream>
#include <string>
using namespace std;

class ConfigFileLoader {
private:
	//ofstream file;
	int X;

public:
	void loadConfig();
	int getX();
};
#endif