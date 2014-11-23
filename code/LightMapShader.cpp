#include "LightMapShader.h"
#include "Texture.h"

namespace Engine
{
	LightMapShader::LightMapShader() : m_sampleState(0)
	{
		m_vertexShader = 0;
		m_pixelShader = 0;
		m_layout = 0;
	}

	LightMapShader::~LightMapShader() {}

	bool LightMapShader::Initialize(Graphics* graphics)
	{
		// initialize the vertex and pixel shaders
		if(!InitializeShader(graphics,L"Content/Shaders/LightMapVS.cso",L"Content/Shaders/LightMapPS.cso"))
			return false;

		return true;
	}

	void LightMapShader::Shutdown()
	{
		ShutdownShader();
	}

	void LightMapShader::Render(Graphics* graphics)
	{
		// set the shader parameters it will use for rendering
		SetShaderParameters(graphics);
		// render the prepared buffers with the shader
		RenderShader(graphics);
	}

	bool LightMapShader::InitializeShader(Graphics* graphics,std::wstring vertexShaderFilename, std::wstring pixelShaderFilename)
	{
		struct ShaderBuffer
		{
			unsigned int Size;
			void* Buffer;

			ShaderBuffer() { Buffer = 0; Size = 0; }
		};

		ShaderBuffer vertexShaderBuffer, pixelShaderBuffer;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
		unsigned int numElements;
		D3D11_SAMPLER_DESC samplerDesc;
		ID3D11Device* device = graphics->GetDevice();

		vertexShaderBuffer.Buffer = LoadCompiledShader(vertexShaderFilename,vertexShaderBuffer.Size);
		pixelShaderBuffer.Buffer = LoadCompiledShader(pixelShaderFilename, pixelShaderBuffer.Size);

		if(FAILED(device->CreateVertexShader(vertexShaderBuffer.Buffer,vertexShaderBuffer.Size,0,&m_vertexShader)))
			return false;

		if(FAILED(device->CreatePixelShader(pixelShaderBuffer.Buffer,pixelShaderBuffer.Size,0,&m_pixelShader)))
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
		if(FAILED(device->CreateInputLayout(polygonLayout,numElements,vertexShaderBuffer.Buffer,vertexShaderBuffer.Size, &m_layout)))
			return false;

		delete vertexShaderBuffer.Buffer;
		vertexShaderBuffer.Buffer = 0;
		delete pixelShaderBuffer.Buffer;
		pixelShaderBuffer.Buffer = 0;

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

		if(FAILED(device->CreateSamplerState(&samplerDesc,&m_sampleState)))
			return false;

		return true;
	}

	void LightMapShader::ShutdownShader()
	{
		if(m_sampleState)
		{
			m_sampleState->Release();
			m_sampleState = 0;
		}
	}

	void LightMapShader::SetShaderParameters(Graphics* graphics)
	{
		unsigned int bufferNumber;
		ID3D11DeviceContext* deviceContext = graphics->GetImmediateContex();

		Matrix worldCopy,viewCopy,projCopy;
		worldCopy = m_worldMatrix.Transpose();
		viewCopy = m_viewMatrix.Transpose();
		projCopy = m_projectionMatrix.Transpose();

		m_matrixBuffer.Data.world = worldCopy;
		m_matrixBuffer.Data.view = viewCopy;
		m_matrixBuffer.Data.projection = projCopy;
		m_matrixBuffer.ApplyChanges(deviceContext);

		bufferNumber = 0;
		ID3D11Buffer* buffer = m_matrixBuffer.Buffer();
		deviceContext->VSSetConstantBuffers(bufferNumber,1,&buffer);

		deviceContext->PSSetShaderResources(0,2,m_textures->GetTextures());
	}

	void LightMapShader::RenderShader(Graphics* graphics)
	{
		ID3D11DeviceContext* context = graphics->GetImmediateContex();
		context->IASetInputLayout(m_layout);

		// set the vertex and pixel shaders that will be used to render this triangle
		context->VSSetShader(m_vertexShader,0,0);
		context->PSSetShader(m_pixelShader,0,0);

		//
		context->PSSetSamplers(0,1,&m_sampleState);
	}

}