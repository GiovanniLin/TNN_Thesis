#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class FileReader
{
public:
	FileReader(std::string filePath);
	void closeFile();
	bool isNextLine();
	std::string readNextLine();
	std::vector<std::string> readNextLineSplit(std::string delimiter);
private:
	std::ifstream file_;
};

