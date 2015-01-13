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
	// a texture class that holds a 3 textures
	class Texture
	{
	public:
		Texture();
		Texture(const Texture&);
		~Texture();

		bool Initialize(Graphics* graphics, std::wstring filename1,std::wstring filename2,std::wstring filename3);
		void Shutdown();

		ID3D11ShaderResourceView** GetTextures();
		int GetHeight(int index) const;
		int GetWidth(int index) const;

	private:
		int m_count;
		ID3D11ShaderResourceView* m_textures[3];
	};

}