#pragma

#include "Graphics.h"
#include <SimpleMath.h>

#include <fstream>
using namespace DirectX::SimpleMath;
namespace Engine
{

	class TextureShader
	{
	private:
		struct MatrixBuffer
		{
			Matrix world;
			Matrix view;
			Matrix projection;
		};
	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		ID3D11Buffer* m_matrixBuffer;
		ID3D11SamplerState* m_sampleState;

	public:
		TextureShader();
		~TextureShader();

		bool Initialize(Graphics* graphics,HWND window);
		void Shutdown();
		void Render(Graphics* graphics,int indexCount,Matrix world,
			Matrix view,Matrix projection,ID3D11ShaderResourceView* texture);
	private:
		bool InitializeShader(Graphics* graphics,HWND window,std::wstring vertexShaderFilename,std::wstring pixelShaderFilename);
		void ShutdownShader();
		void OutputShaderErrorMessage(ID3D10Blob* blob,HWND window,std::wstring message);

		void SetShaderParameters(Graphics* graphics,Matrix world,
			Matrix view, Matrix proj,ID3D11ShaderResourceView* texture);
		void RenderShader(Graphics* graphics,int indexCount);
	}; 
}