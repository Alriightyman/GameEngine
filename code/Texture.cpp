////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "texture.h"

namespace Engine
{

	Texture::Texture() : m_count(3)
	{
		for(int i = 0; i < m_count; i++)
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

	int Texture::GetHeight(int index) const
	{
		ID3D11Resource* resource;
		ID3D11Texture2D* texture2d;
		D3D11_TEXTURE2D_DESC desc;
		if (index >= m_count || !m_textures[index])
			return 0;

		m_textures[index]->GetResource(&resource);
		resource->QueryInterface(__uuidof(ID3D11Texture2D),(void**)&texture2d);
		texture2d->GetDesc(&desc);

		return desc.Height;
	}

	int Texture::GetWidth(int index) const
	{
		ID3D11Resource* resource;
		ID3D11Texture2D* texture2d;
		D3D11_TEXTURE2D_DESC desc;
		if (index >= m_count || !m_textures[index])
			return 0;

		m_textures[index]->GetResource(&resource);
		resource->QueryInterface(__uuidof(ID3D11Texture2D),(void**)&texture2d);
		texture2d->GetDesc(&desc);

		return desc.Width;
	}
}