#include "ptc_file_reader.hpp"
#include "ptc_console.hpp"
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>

std::string FileReader::Read(std::string path)
{

	std::string content;
	std::ifstream fileStream;

	fileStream.exceptions(std::ifstream::badbit);

	try
	{
		fileStream.open(path);
		std::stringstream stringStream;

		stringStream << fileStream.rdbuf();

		fileStream.close();

		content = stringStream.str();
	}
	catch (std::ifstream::failure e)
	{
		Console::WriteLine(Console::FormatString("ERROR::FILEREADER::FILE_READ_ERROR\n%s", e.what()));
	}

	return content;
}
std::vector<uint8_t> FileReader::ReadBytes(std::string path)
{

	std::vector<uint8_t> content;
	std::ifstream fileStream;

	fileStream.exceptions(std::ifstream::badbit);

	try
	{
		fileStream.open(path, std::ios::binary | std::ios::ate);
		size_t size = fileStream.tellg();
		fileStream.seekg(0);
		content.resize(size);
		fileStream.read(reinterpret_cast<char*>(content.data()), size);
		fileStream.close();
	}
	catch (std::ifstream::failure e)
	{
		Console::WriteLine(Console::FormatString("ERROR::FILEREADER::FILE_READ_ERROR\n%s", e.what()));
	}

	return content;
}