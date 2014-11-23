#pragma once
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include <SimpleMath.h>
#include <fstream>
namespace Engine
{
	class Texture;

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

	public:
		MultiTextureShader();
		~MultiTextureShader() override;

		bool Initialize(Graphics* graphics) override;
		void Shutdown() override;
		void Render(Graphics* graphics) override;
	private:
		bool InitializeShader(Graphics* graphics,std::wstring vertexShaderFilename,std::wstring pixelShaderFilename) override;
		void ShutdownShader();

		void SetShaderParameters(Graphics* graphics) override;;
		void RenderShader(Graphics* graphics) override;;
	}; 
}