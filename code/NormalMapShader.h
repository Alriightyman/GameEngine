#pragma once
#include "Shader.h"

namespace Engine
{
	class NormalMapShader : public Shader
	{
	private:
		struct MatrixBuffer
		{
			Matrix world;
			Matrix view;
			Matrix projection;
		};

		struct LightBuffer
		{
			DirectX::SimpleMath::Color diffuseColor;
			DirectX::SimpleMath::Vector3 lightDirection;
			float padding;
		};

	private:
		ID3D11SamplerState* m_sampleState;
		ConstantBuffer<MatrixBuffer> m_matrixBuffer;
		ConstantBuffer<LightBuffer> m_lightBuffer;

		DirectX::SimpleMath::Color m_diffuseColor;
		DirectX::SimpleMath::Vector3 m_lightDirection;
		Texture* m_textures;
	public:
		void SetDiffuseColor(DirectX::SimpleMath::Color color);
		void SetLightDiretion(DirectX::SimpleMath::Vector3 lightDir);
		void SetTexture(Texture* texture);
	public:
		NormalMapShader();
		~NormalMapShader();

		bool Initialize(Graphics* graphics) override;
		void Shutdown() override;
		void Render(Graphics* graphics,int indexCount) override;

	private:
		bool InitializeShader(Graphics* graphics,std::wstring vsFilename, std::wstring psFilename) override;
		void ShutdownShader() override;
		void SetShaderParameters(Graphics* graphics) override;
		void RenderShader(Graphics* graphics,int indexCount) override;
	};

}