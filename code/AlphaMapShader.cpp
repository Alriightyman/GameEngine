#include "AlphaMapShader.h"
#include "Texture.h"

namespace Engine
{

	AlphaMapShader::AlphaMapShader()
	{
		m_vertexShader = 0;
		m_pixelShader = 0;
		m_layout = 0;
		m_samplerState = 0;
	}

	AlphaMapShader::~AlphaMapShader()
	{
	}

	bool AlphaMapShader::Initialize(Graphics* graphics)
	{
		if(!InitializeShader(graphics,L"Content/Shaders/AlphaMapVS.cso",L"Content/Shaders/AlphaMapPS.cso"))
			return false;

		return true;
	}

	void AlphaMapShader::Shutdown()
	{
		ShutdownShader();
	}

	void AlphaMapShader::Render(Graphics* graphics)
	{

		SetShaderParameters(graphics);

		RenderShader(graphics);
	}

	bool AlphaMapShader::InitializeShader(Graphics* graphics, std::wstring vsFilename, std::wstring psFilename)
	{
		struct ShaderBuffer
		{
			unsigned int Size;
			void* Buffer;
			ShaderBuffer() { Size = 0; Buffer = 0; }
		};

		ShaderBuffer vertexBuffer, pixelBuffer;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
		unsigned int numElements;
		D3D11_SAMPLER_DESC samplerDesc;
		ID3D11Device* device = graphics->GetDevice();

		// load shaders
		vertexBuffer.Buffer = LoadCompiledShader(vsFilename,vertexBuffer.Size);
		pixelBuffer.Buffer = LoadCompiledShader(psFilename,pixelBuffer.Size);

		// create vertex and pixel shaders
		if(FAILED(device->CreateVertexShader(vertexBuffer.Buffer,vertexBuffer.Size,0,&m_vertexShader)))
			return false;

		if(FAILED(device->CreatePixelShader(pixelBuffer.Buffer,pixelBuffer.Size, 0,&m_pixelShader)))
			return false;

		// create the vertex input layout description
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "TEXCOORD";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;
		// get a count of the elements in the layout
		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		// create the vertex input layout
		if(FAILED(device->CreateInputLayout(polygonLayout,numElements,vertexBuffer.Buffer,vertexBuffer.Size,&m_layout)))
			return false;

		delete vertexBuffer.Buffer;
		vertexBuffer.Buffer = 0;
		delete pixelBuffer.Buffer; 
		pixelBuffer.Buffer = 0;

		// inititalize the matrix constant buffer
		m_matrixBuffer.Initialize(device);

		// Create a texture sampler state description.
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if(FAILED(device->CreateSamplerState(&samplerDesc,&m_samplerState)))
			return false;

		return true;
	}

	void AlphaMapShader::ShutdownShader()
	{
		if(m_samplerState)
		{
			m_samplerState->Release();
			m_samplerState = 0;
		}
	}

	void AlphaMapShader::SetShaderParameters(Graphics* graphics)
	{
		unsigned int bufferNumber = 0;
		ID3D11DeviceContext* context = graphics->GetImmediateContex();

		Matrix world = m_worldMatrix.Transpose();
		Matrix view = m_viewMatrix.Transpose();
		Matrix proj = m_projectionMatrix.Transpose();

		m_matrixBuffer.Data.World = world;
		m_matrixBuffer.Data.View = view;
		m_matrixBuffer.Data.Projection = proj;
		m_matrixBuffer.ApplyChanges(context);

		ID3D11Buffer* buffer = m_matrixBuffer.Buffer();
		context->VSSetConstantBuffers(bufferNumber,1,&buffer);
		context->PSSetShaderResources(0,3,m_textures->GetTextures());
	}

	void AlphaMapShader::RenderShader(Graphics* graphics)
	{
		ID3D11DeviceContext* context= graphics->GetImmediateContex();

		context->IASetInputLayout(m_layout);

		// set the vertex and pixel shaders
		context->VSSetShader(m_vertexShader,0,0);
		context->PSSetShader(m_pixelShader,0,0);
	}

}