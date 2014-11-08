#pragma once

#include "Shader.h"

namespace Engine
{
	class LightMapShader : public Shader
	{
	private:
		struct MatrixBuffer
		{
			Matrix world;
			Matrix view;
			Matrix projection;
		};
	private:
		ID3D11SamplerState* m_sampleState;
		ConstantBuffer<MatrixBuffer> m_matrixBuffer;
		Texture* m_texture;
	public:
		void SetTexture(Texture* texture);

	public:
		LightMapShader();
		~LightMapShader() override;

		bool Initialize(Graphics* graphics) override;
		void Shutdown() override;
		void Render(Graphics* graphics, int indexCount) override;

	private:
		bool InitializeShader(Graphics* graphics, std::wstring vertexShaderFilename, std::wstring pixelShaderFilename) override;
		void ShutdownShader() override;

		void SetShaderParameters(Graphics* graphics) override;
		void RenderShader(Graphics* graphics, int indexCount) override;

	};

}
