#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <fstream>
#include "Graphics.h"
#include "ConstantBuffer.h"
using namespace DirectX::SimpleMath;

namespace Engine
{

	class ToonShader
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
		ToonShader();
		~ToonShader();

		bool Initialize(ID3D11Device* device);
		void Shutdown();
		bool Render(Graphics* graphics, int indexCount, const Matrix& worldMatrix, const Matrix& viewMatrix,
			const Matrix& projectionMatrix, ID3D11ShaderResourceView* texture, const Vector3& lightDirection, 
			const Color& diffuseColor,bool drawLine = true );
	private:
		bool InitializeShader(ID3D11Device*, std::wstring, std::wstring,
			std::wstring, std::wstring);
		void ShutdownShader();
		void* LoadCompiledShader(const wchar_t* filename,unsigned int& size);
		bool SetShaderParameters(ID3D11DeviceContext* deviceContext, const Matrix& worldMatrix, const Matrix& viewMatrix,
			const Matrix& projectionMatrix, ID3D11ShaderResourceView* texture, const Vector3& lightDirection, 
			const Color& diffuseColor );
		void RenderShader(Graphics* graphics, int,bool);

	private:
		ID3D11VertexShader* m_mainVertexShader;
		ID3D11PixelShader* m_mainPixelShader;
		ID3D11VertexShader* m_outlineVertexShader;
		ID3D11PixelShader* m_outlinePixelShader;

		ID3D11InputLayout* m_layout;
		ID3D11InputLayout* m_outLineLayout;
		ID3D11SamplerState* m_sampleState;
		//ID3D11Buffer* m_matrixBuffer;
		//ID3D11Buffer* m_lightBuffer;
		ConstantBuffer<MatrixBuffer> m_matrixBuffer;
		ConstantBuffer<LightBuffer> m_lightBuffer;
	}; 
}