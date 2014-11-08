////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "texture.h"

namespace Engine
{

	Texture::Texture()
	{
		for(int i = 0; i < 3; i++)
			m_textures[i] = 0;
	}


	Texture::Texture(const Texture& other)
	{
	}


	Texture::~Texture()
	{

	}


	bool Texture::Initialize(Graphics* graphics,std::wstring filename1, std::wstring filename2,std::wstring filename3)
	{
		ID3D11Device* device = graphics->GetDevice();

		// Load the texture in.

		if(FAILED(CreateDDSTextureFromFile( device, filename1.c_str(), nullptr, &m_textures[0] )))
			return false;

		if(filename2.size() > 1)
		{
			if(FAILED(CreateDDSTextureFromFile( device, filename2.c_str(), nullptr, &m_textures[1] )))
				return false;
		
			if(filename3.size() > 1)
				if(FAILED(CreateDDSTextureFromFile(device,filename3.c_str(),nullptr,&m_textures[2])))
					return false;

		}
		return true;
	}


	void Texture::Shutdown()
	{
		// Release the texture resource.
		for(int i = 0; i < 3; i++)
		{
			if(m_textures[i])
			{
				m_textures[i]->Release();
				m_textures[i] = 0;
			}
		}
		return;
	}


	ID3D11ShaderResourceView** Texture::GetTextures()
	{
		return m_textures;
	} 
}