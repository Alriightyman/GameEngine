#include "Shader.h"

namespace Engine
{
	Shader::Shader()
	{

	}
	Shader::~Shader()
	{
		if(m_vertexShader)
		{
			m_vertexShader->Release();
			m_vertexShader = 0;
		}

		if(m_pixelShader)
		{
			m_pixelShader->Release();
			m_pixelShader = 0;
		}

		if(m_layout)
		{
			m_layout->Release();
			m_layout = 0;
		}
	}

	void Shader::Shutdown()
	{

	}

	void Shader::SetWorldViewProjMatrices(Matrix world,Matrix view, Matrix projection)
	{
		m_worldMatrix = world;
		m_viewMatrix = view;
		m_projectionMatrix = projection;
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