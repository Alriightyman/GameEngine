#include "Shader.h"

namespace Engine
{
	Shader::Shader()
	{

	}
	Shader::~Shader()
	{

	}

	void Shader::Shutdown()
	{

	}

	char* Shader::LoadCompiledShader(std::wstring filename, unsigned int& size)
	{
		// open the file
		std::ifstream ifs(filename, std::ios::binary);
		// make sure it is good
		if (ifs.bad() || ifs.fail())
		{
			std::wstring failmsg = L"Failed to load shader from ";
			failmsg.append(filename);
			return 0;
		}
		// get the size of the file
		ifs.seekg(0, std::ios::end);
		size = (UINT)ifs.tellg();
		// create some memory
		char* buffer = new char[size];
		// set position to the begining of the file
		ifs.seekg(0, std::ios::beg);
		// read the file
		ifs.read(buffer, size);
		// close the file
		ifs.close();
		// return the buffer
		return buffer;
	} 
}