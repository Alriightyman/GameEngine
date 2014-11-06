////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.h
#pragma once

//////////////
// INCLUDES //
//////////////
#include "Graphics.h"
#include <DDSTextureLoader.h>
using namespace DirectX;


namespace Engine
{

	class Texture
	{
	public:
		Texture();
		Texture(const Texture&);
		~Texture();

		bool Initialize(Graphics* graphics, std::wstring filename1,std::wstring filename2);
		void Shutdown();

		ID3D11ShaderResourceView** GetTextures();

	private:
		ID3D11ShaderResourceView* m_textures[2];
	};


}