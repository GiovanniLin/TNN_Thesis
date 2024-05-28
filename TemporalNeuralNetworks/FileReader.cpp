#include "FileReader.h"

FileReader::FileReader(std::string filePath)
{
	file_.open(filePath, std::ios::in);
	if (!file_.is_open()) {
		throw std::runtime_error("Unable to open file: " + filePath + " \n");
	}
}

void FileReader::closeFile()
{
	file_.close();
}

bool FileReader::isNextLine() {
	if (!file_.is_open()) {
		throw std::runtime_error("File is not open \n");
	}

	return file_.peek() != std::ifstream::traits_type::eof();
}

std::string FileReader::readNextLine()
{
	if (!file_.is_open()) {
		throw std::runtime_error("File is not open \n");
	}

	std::string line;
	if (std::getline(file_, line)) {
		return line;
	}
	else {
		throw std::runtime_error("End of file reached, use isNextLine() for safe use of readNextLine() \n");
	}
}

std::vector<std::string> FileReader::readNextLineSplit(std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string s = readNextLine();
	std::string token;
	std::vector<std::string> res;

	if (s.empty()) {
		return res;
	}

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}
