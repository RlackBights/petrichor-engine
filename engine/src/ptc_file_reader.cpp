#include "ptc_file_reader.hpp"
#include "ptc_console.hpp"
#include <sstream>
#include <fstream>

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