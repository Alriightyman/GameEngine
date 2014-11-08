#pragma once
#include "Graphics.h"
#include "ConstantBuffer.h"
#include <SimpleMath.h>
#include <fstream>


using namespace DirectX::SimpleMath;


namespace Engine
{
	class Shader
	{

	protected:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		Matrix m_worldMatrix, m_viewMatrix,m_projectionMatrix;

	public:
		Shader();
		virtual ~Shader();
		virtual bool Initialize(Graphics* graphics) = 0;
		virtual void Shutdown();

		virtual void Render(Graphics* graphics, int indexCount) = 0;
	public:
		void SetWorldViewProjMatrices(Matrix world,Matrix view, Matrix projection);
	protected:
		virtual bool InitializeShader(Graphics* graphics, std::wstring vertShaderfilename,std::wstring pixelShaderFilename) = 0;
		virtual void ShutdownShader() = 0;
		virtual void SetShaderParameters(Graphics* graphics) = 0;
		virtual void RenderShader(Graphics* graphics, int indexCount) = 0;
		char* LoadCompiledShader(std::wstring filename, unsigned int& size);

	};
}