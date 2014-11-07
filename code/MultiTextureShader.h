#pragma once
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include <SimpleMath.h>
#include <fstream>
namespace Engine
{

	class MultiTextureShader : public Shader
	{
	private:
		struct MatrixBuffer
		{
			Matrix World;
			Matrix View;
			Matrix Projection;
		};
	private:
		ID3D11SamplerState* m_sampleState;
		ConstantBuffer<MatrixBuffer> m_matrixBuffer;
		ID3D11ShaderResourceView** m_textureArray;
	public:
		void SetTextureArray(ID3D11ShaderResourceView** texArray);
	public:
		MultiTextureShader();
		~MultiTextureShader() override;

		bool Initialize(Graphics* graphics) override;
		void Shutdown() override;
		void Render(Graphics* graphics,int indexCount) override;
	private:
		bool InitializeShader(Graphics* graphics,std::wstring vertexShaderFilename,std::wstring pixelShaderFilename) override;
		void ShutdownShader();

		void SetShaderParameters(Graphics* graphics) override;;
		void RenderShader(Graphics* graphics,int indexCount) override;;
	}; 
}