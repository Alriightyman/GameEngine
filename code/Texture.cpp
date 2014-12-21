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
	//	Shutdown();
	}


	bool Texture::Initialize(Graphics* graphics,std::wstring filename1, std::wstring filename2,std::wstring filename3)
	{
		ID3D11Device* device = graphics->GetDevice();
		
		// Load the texture in.
		HRESULT hr = CreateDDSTextureFromFile( device, filename1.c_str(), nullptr, &m_textures[0] );
		if(FAILED(hr))
			return false;
		std::wstring path = L"Content/Textures/";
		if(filename2.size() > path.size() + 1)
		{
			if(FAILED(CreateDDSTextureFromFile( device, filename2.c_str(), nullptr, &m_textures[1] )))
				return false;
		
			if(filename3.size() > path.size() + 1)
				if(FAILED(CreateDDSTextureFromFile(device,filename3.c_str(),nullptr,&m_textures[2])))
					return false;

		}
		return true;
	}


	void Texture::Shutdown()
	{

	}


	ID3D11ShaderResourceView** Texture::GetTextures()
	{
		return m_textures;
	} 
}