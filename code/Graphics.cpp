#include "Graphics.h"
#include "HelperUtilities.h"

using namespace DirectX;
namespace Engine
{

	Graphics::Graphics()
	{
		m_swapChain = 0;
		m_device = 0;
		m_deviceContext = 0;
		m_renderTargetView = 0;
		m_depthStencilBuffer = 0;
		m_depthStencilState = 0;
		m_depthStencilView = 0;
		m_rasterState = 0;
		m_depthDisabledStencilState = 0;
	}
	Graphics::~Graphics()
	{

	}

	bool Graphics::Init(HWND hwnd, int width, int height, bool windowed)
	{
		/***********************************************\
		|	Initialize Direct3D 11						|
		\***********************************************/
		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator = 60, denominator = 1, stringLength;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		int error;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevel;

		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

		// Store the vsync setting.
		m_vsync_enabled = true;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if(FAILED(result))
		{
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if(FAILED(result))
		{
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if(FAILED(result))
		{
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if(FAILED(result))
		{
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		if(!displayModeList)
		{
			return false;
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if(FAILED(result))
		{
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for(i=0; i<numModes; i++)
		{
			if(displayModeList[i].Width == (unsigned int)width)
			{
				if(displayModeList[i].Height == (unsigned int)height)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		if(FAILED(result))
		{
			return false;
		}

		// Store the dedicated video card memory in megabytes.
		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
		if(error != 0)
		{
			return false;
		}

		// Release the display mode list.
		delete [] displayModeList;
		displayModeList = 0;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = 0;

		// Release the adapter.
		adapter->Release();
		adapter = 0;

		// Release the factory.
		factory->Release();
		factory = 0;

		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if(m_vsync_enabled)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = hwnd;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if(!windowed)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
			D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
		if(FAILED(result))
		{
			return false;
		}




#pragma endregion

		return OnResize(width,height);

	}

	bool Graphics::OnResize(int width, int height)
	{
		ReleaseCOM(m_renderTargetView);
		ReleaseCOM(m_depthStencilView);
		ReleaseCOM(m_depthStencilBuffer);

		ID3D11Texture2D* backBufferPtr;

		// Get the pointer to the back buffer.
		HRESULT result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if(FAILED(result))
		{
			return false;
		}

		// Create the render target view with the back buffer pointer.
		result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
		if(FAILED(result))
		{
			return false;
		}

		// Release pointer to the back buffer as we no longer need it.
		backBufferPtr->Release();
		backBufferPtr = 0;

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = width;
		depthBufferDesc.Height = height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
		if(FAILED(result))
		{
			return false;
		}

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
		if(FAILED(result))
		{
			return false;
		}

		// Set the depth stencil state.
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		// Initailze the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
		if(FAILED(result))
		{
			return false;
		}

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

		D3D11_RASTERIZER_DESC rasterDesc;
		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
		if(FAILED(result))
		{
			return false;
		}

		// Now set the rasterizer state.
		m_deviceContext->RSSetState(m_rasterState);

		D3D11_RASTERIZER_DESC outlineRasterDesc;
		// Setup the raster description which will determine how and what polygons will be drawn.
		outlineRasterDesc.AntialiasedLineEnable = false;
		outlineRasterDesc.CullMode = D3D11_CULL_FRONT;
		outlineRasterDesc.DepthBias = 0;
		outlineRasterDesc.DepthBiasClamp = 0.0f;
		outlineRasterDesc.DepthClipEnable = true;
		outlineRasterDesc.FillMode = D3D11_FILL_SOLID;
		outlineRasterDesc.FrontCounterClockwise = true;
		outlineRasterDesc.MultisampleEnable = false;
		outlineRasterDesc.ScissorEnable = false;
		outlineRasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = m_device->CreateRasterizerState(&outlineRasterDesc, &m_outLineRasterState);
		if(FAILED(result))
		{
			return false;
		}


		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
		// Clear the second depth stencil state before setting the parameters.
		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
		// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
		// Setup the viewport for rendering.
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;

		// Create the viewport.
		m_deviceContext->RSSetViewports(1, &m_viewport);

		// Setup the projection matrix.
		float fieldOfView = (float)XM_PI / 4.0f;
		float screenAspect = (float)width / (float)height;

		// Create the projection matrix for 3D rendering.
		m_screenDepth = 1000.0f;
		m_projectionMatrix = XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, 0.1f, m_screenDepth );

		// Initialize the world matrix to the identity matrix.
		m_worldMatrix =XMMatrixIdentity();

		// Create an orthographic projection matrix for 2D rendering.
		m_orthoMatrix = XMMatrixOrthographicLH( ( float )width, ( float )height, 0.1f, m_screenDepth );

		return true;
	}

	void Graphics::Release()
	{
		if(m_depthDisabledStencilState)
		{
			m_depthDisabledStencilState->Release();
			m_depthDisabledStencilState = 0;
		}
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		if(m_swapChain)
		{
			m_swapChain->SetFullscreenState(false, NULL);
		}

		if(m_rasterState)
		{
			m_rasterState->Release();
			m_rasterState = 0;
		}

		if(m_depthStencilView)
		{
			m_depthStencilView->Release();
			m_depthStencilView = 0;
		}

		if(m_depthStencilState)
		{
			m_depthStencilState->Release();
			m_depthStencilState = 0;
		}

		if(m_depthStencilBuffer)
		{
			m_depthStencilBuffer->Release();
			m_depthStencilBuffer = 0;
		}

		if(m_renderTargetView)
		{
			m_renderTargetView->Release();
			m_renderTargetView = 0;
		}

		if(m_deviceContext)
		{
			m_deviceContext->Release();
			m_deviceContext = 0;
		}

		if(m_device)
		{
			m_device->Release();
			m_device = 0;
		}

		if(m_swapChain)
		{
			m_swapChain->Release();
			m_swapChain = 0;
		}

		delete this;
	}

	void Graphics::Clear(const float* color,UINT clearFlags,float depth,UINT8 stencil)
	{
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

		// Clear the depth buffer.
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, clearFlags,depth,stencil);

	}
	void Graphics::Present()
	{
		// Present the back buffer to the screen since rendering is complete.
		if(m_vsync_enabled)
		{
			// Lock to screen refresh rate.
			m_swapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			m_swapChain->Present(0, 0);
		}
	}


	void Graphics::TurnZBufferOn()
	{
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
		return;
	}


	void Graphics::TurnZBufferOff()
	{
		m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
		return;
	} 
}