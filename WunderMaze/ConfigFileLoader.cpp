#include "ConfigFileLoader.h";

void ConfigFileLoader::loadConfig() {
	string line;
	ifstream myfile("config.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line.find("size") == 0) {
				X = atoi(line.substr(5).c_str());
			}
		}
		myfile.close();
	}
	else cout << "Unable to open file";
}

int ConfigFileLoader::getX() {
	return X;
}