#pragma once
#include "Graphics.h"
#include "ConstantBuffer.h"
#include <SimpleMath.h>
#include <fstream>
namespace Engine
{

	class MultiTextureShader
	{
	private:
		struct MatrixBuffer
		{
			Matrix World;
			Matrix View;
			Matrix Projection;
		};
	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		ID3D11SamplerState* m_sampleState;
		ConstantBuffer<MatrixBuffer> m_matrixBuffer;

	public:
		MultiTextureShader();
		~MultiTextureShader();

		bool Initialize(Graphics* graphics);
		void Shutdown();
		void Render(Graphics* graphics,int indexCount, Matrix worldMatrix,Matrix viewMatrix,
			Matrix projMatrix, ID3D11ShaderResourceView** textureArray);
	private:
		bool InitializeShader(Graphics* graphics,std::wstring vertexShaderFilename,std::wstring pixelShaderFilename);
		void ShutdownShader();

		void SetShaderParameters(Graphics* graphics,Matrix worldMatrix,Matrix viewMatrix,
			Matrix projMatrix, ID3D11ShaderResourceView** textureArray);
		void RenderShader(Graphics* graphics,int indexCount);
		void* LoadCompiledShader(std::wstring filename,unsigned int& size);
	}; 
}