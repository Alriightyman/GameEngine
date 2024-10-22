#pragma once

#include "Shader.h"


namespace Engine
{
	class AlphaMapShader : public Shader
	{
	private:
		struct MatrixBuffer
		{
			Matrix World;
			Matrix View;
			Matrix Projection;
		};

	private:
		ConstantBuffer<MatrixBuffer> m_matrixBuffer;
		ID3D11SamplerState* m_samplerState;

	public:
		AlphaMapShader();
		AlphaMapShader(const AlphaMapShader&) {}
		~AlphaMapShader();

		bool Initialize(Graphics* graphics) override;
		void Shutdown() override;
		void Render(Graphics* graphics) override;

	private:
		bool InitializeShader(Graphics* graphics, std::wstring vsfilename, std::wstring psFilename) override;
		void ShutdownShader() override ;
		void SetShaderParameters(Graphics* graphics) override;
		void RenderShader(Graphics* graphics) override;
	}; 
}