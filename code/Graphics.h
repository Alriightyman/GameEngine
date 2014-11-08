#pragma once
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <map>
#include <memory>


using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace Engine
{
	class Texture;
	class Model;
	class Shader;
	class LightShader;
	class LightMapShader;
	class ToonShader;
	class TextureShader;
	class MultiTextureShader;

	enum ShaderType { LIGHT, TEXTURE,MULTITEXTURE, TOON, LIGHTMAP };

	class Graphics
	{
	private: // fields
		float m_screenDepth;
		bool m_vsync_enabled;
		int m_videoCardMemory;
		char m_videoCardDescription[128];
		D3D11_VIEWPORT m_viewport;
		IDXGISwapChain* m_swapChain;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilState* m_depthDisabledStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterState;
		ID3D11RasterizerState* m_outLineRasterState;

		XMMATRIX m_projectionMatrix;
		XMMATRIX m_worldMatrix;
		XMMATRIX m_orthoMatrix;

		// shader class
		std::map<ShaderType,std::unique_ptr<Shader> > shaders;

	public: // properties
		float GetScreenDepth() const { return m_screenDepth; }
		bool IsVsyncEnabled() const { return m_vsync_enabled; }
		void EnableVsync() { m_vsync_enabled = true; }
		void DisableVysnc() { m_vsync_enabled = false; }

		char* GetVideoCardDescritption() { return m_videoCardDescription; }
		int GetVideoCardMemory() const { return m_videoCardMemory; }
		IDXGISwapChain* GetSwapChain() const { return m_swapChain; }
		ID3D11Device* GetDevice() const { return m_device; }
		ID3D11DeviceContext* GetImmediateContex() { return m_deviceContext; }
		ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView; }
		ID3D11Texture2D* GetDepthStencilBuffer() const { return m_depthStencilBuffer; }
		ID3D11DepthStencilState* GetDepthStencilState() const { return m_depthStencilState; }
		ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView; }
		ID3D11RasterizerState* GetRasterState() const { return m_rasterState; }
		ID3D11RasterizerState* GetOutlineRasterState() const { return m_outLineRasterState; }
		Matrix GetProjectionMatrix() const { return m_projectionMatrix; }
		Matrix GetWorldMatrix() const { return m_worldMatrix; }
		Matrix GetOrthoMatrix() const { return m_orthoMatrix; }
		D3D11_VIEWPORT* GetViewPort() { return &m_viewport; }

		LightShader* GetLightShader(); 
		TextureShader* GetTextureShader();
		MultiTextureShader* GetMultiTextureShader();
		ToonShader* GetToonShader();
		LightMapShader* GetLightMapShader();
	public:
		Graphics();
		~Graphics();

		bool Init(HWND hwnd, int width, int height, bool windowed);
		void Release();

		void Clear(const float* color,UINT clearFlags = D3D11_CLEAR_DEPTH,float depth = 1.0f,UINT8 stencil = 0);
		void Present();

		bool OnResize(int width,int height);

		void TurnZBufferOn();
		void TurnZBufferOff();

		Texture* CreateTexture(std::wstring filename1, std::wstring filename2);
		Model* CreateModel(std::wstring filename, Texture* texure);

		void Render(Model* model, ShaderType shaderType = ShaderType::LIGHT);
	}; 
}