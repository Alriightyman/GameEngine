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
		ID3D11ShaderResourceView* m_texture;

	public:
		void SetTexture(Texture* texture);
	public:
		TextureShader();
		~TextureShader() override;

		bool Initialize(Graphics* graphics) override;
		void Shutdown() override;
		void Render(Graphics* graphics,int indexCount) override;
	private:
		bool InitializeShader(Graphics* graphics,std::wstring vertexShaderFilename,std::wstring pixelShaderFilename) override;
		void ShutdownShader();
		
		void SetShaderParameters(Graphics* graphics) override;
		void RenderShader(Graphics* graphics,int indexCount) override;


	}; 
}