#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <fstream>
#include "Graphics.h"
#include "Shader.h"
using namespace DirectX::SimpleMath;

namespace Engine
{
	class Texture;

	class ToonShader : public Shader
	{
	private:
		struct MatrixBuffer
		{
			Matrix World;
			Matrix View;
			Matrix Projection;
			Matrix WorldInverseTranspose;
		};

		struct LightBuffer
		{
			Color DiffuseColor;
			Vector3 Direction;
			float Intensity;
		};
	public:
		void SetTexture(Texture* texture);
		void SetLightDirection(Vector3& lightDirection);
		void SetDiffuseColor(Color& diffuseColor);
		void IsDrawLine(bool isDrawLine);
	public:
		ToonShader();
		~ToonShader() override;

		bool Initialize(Graphics* graphics) override;
		void Shutdown();
		void Render(Graphics* graphics, int indexCount) override;;
	private:
		bool InitializeShader(Graphics* graphics, std::wstring,std::wstring) override {return true;}

		bool InitializeShader(Graphics* graphics, std::wstring, std::wstring,
			std::wstring, std::wstring);
		void ShutdownShader() override;;
		void SetShaderParameters(Graphics* graphics)  override;
		void RenderShader(Graphics*,int) override;


	private:
		ID3D11ShaderResourceView* m_texture;
		Vector3 m_lightDirection;
		Color m_diffuseColor;
		bool m_drawLine;
		ID3D11VertexShader* m_outlineVertexShader;
		ID3D11PixelShader* m_outlinePixelShader;

		ID3D11InputLayout* m_outLineLayout;
		ID3D11SamplerState* m_sampleState;

		ConstantBuffer<MatrixBuffer> m_matrixBuffer;
		ConstantBuffer<LightBuffer> m_lightBuffer;
	}; 
}