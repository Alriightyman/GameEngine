#pragma

#include "Graphics.h"
#include "Shader.h"
#include <SimpleMath.h>
#include <fstream>
using namespace DirectX::SimpleMath;
namespace Engine
{
	class Texture;

	class TextureShader : public Shader
	{
	private:
		struct MatrixBuffer
		{
			Matrix world;
			Matrix view;
			Matrix projection;
		};
	private:

		ConstantBuffer<MatrixBuffer> m_matrixBuffer;
		ID3D11SamplerState* m_sampleState;

	public:
		TextureShader();
		~TextureShader() override;

		bool Initialize(Graphics* graphics) override;
		void Shutdown() override;
		void Render(Graphics* graphics) override;
	private:
		bool InitializeShader(Graphics* graphics,std::wstring vertexShaderFilename,std::wstring pixelShaderFilename) override;
		void ShutdownShader();
		
		void SetShaderParameters(Graphics* graphics) override;
		void RenderShader(Graphics* graphics) override;


	}; 
}