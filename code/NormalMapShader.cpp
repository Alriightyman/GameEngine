#include "NormalMapShader.h"
#include "Texture.h"


namespace Engine
{
	NormalMapShader::NormalMapShader() : m_sampleState(0)
	{
		using namespace DirectX::SimpleMath;
		m_vertexShader = 0;
		m_pixelShader = 0;
		m_diffuseColor = Color::Color(1,1,1,1);
		m_lightDirection = Vector3(0.0f);
	}

	NormalMapShader::~NormalMapShader()
	{
		Shutdown();
	}

	bool NormalMapShader::Initialize(Graphics* graphics)
	{
		if(InitializeShader(graphics,L"Content/Shaders/NormalMapVS.cso",L"Content/Shaders/NormalMapPS.cso"))
			return false;

		return true;
	}

	void NormalMapShader::Shutdown()
	{
		ShutdownShader();
	}

	void NormalMapShader::Render(Graphics* graphics)
	{
		SetShaderParameters(graphics);

		RenderShader(graphics);
	}

	bool NormalMapShader::InitializeShader(Graphics* graphics,std::wstring vsFilename, std::wstring psFilename)
	{
		struct ShaderBuffer
		{
			unsigned int Size;
			void* Buffer;
			ShaderBuffer() { Size = 0; Buffer = 0; }
		};

		ShaderBuffer vertexShaderBuffer, pixelShaderBuffer;
		
		D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
		unsigned int numElements;

		D3D11_SAMPLER_DESC samplerDesc;
		ID3D11Device* device = graphics->GetDevice();

		vertexShaderBuffer.Buffer = LoadCompiledShader(vsFilename,vertexShaderBuffer.Size);
		pixelShaderBuffer.Buffer = LoadCompiledShader(psFilename,pixelShaderBuffer.Size);

		if(FAILED(device->CreateVertexShader(vertexShaderBuffer.Buffer,vertexShaderBuffer.Size,0,&m_vertexShader)))
			return false;

		if(FAILED(device->CreatePixelShader(pixelShaderBuffer.Buffer,pixelShaderBuffer.Size,0,&m_pixelShader)))
			return false;

		// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
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

		polygonLayout[2].SemanticName = "NORMAL";
		polygonLayout[2].SemanticIndex = 0;
		polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[2].InputSlot = 0;
		polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[2].InstanceDataStepRate = 0;

		polygonLayout[3].SemanticName = "TANGENT";
		polygonLayout[3].SemanticIndex = 0;
		polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[3].InputSlot = 0;
		polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[3].InstanceDataStepRate = 0;

		polygonLayout[4].SemanticName = "BINORMAL";
		polygonLayout[4].SemanticIndex = 0;
		polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[4].InputSlot = 0;
		polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[4].InstanceDataStepRate = 0;

		// Get a count of the elements in the layout.
		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		// Create the vertex input layout.
		if(FAILED( device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer.Buffer, vertexShaderBuffer.Size, &m_layout)))
			return false;

		delete [] vertexShaderBuffer.Buffer;
		vertexShaderBuffer.Buffer = 0;
		delete [] pixelShaderBuffer.Buffer;
		pixelShaderBuffer.Buffer = 0;

		m_matrixBuffer.Initialize(device);

		m_lightBuffer.Initialize(device);

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

	void NormalMapShader::ShutdownShader()
	{
		if(m_sampleState)
		{
			m_sampleState->Release();
			m_sampleState = 0;
		}
	}

	void NormalMapShader::SetShaderParameters(Graphics* graphics)
	{
		using namespace DirectX::SimpleMath;
		unsigned int bufferNumber = 0;
		ID3D11DeviceContext* context = graphics->GetImmediateContex();
		ID3D11Buffer* buffer = 0;

		Matrix world = m_worldMatrix.Transpose();
		Matrix view = m_viewMatrix.Transpose();
		Matrix proj = m_projectionMatrix.Transpose();
		
		// setup matrix constant buffer
		m_matrixBuffer.Data.world = world;
		m_matrixBuffer.Data.view = view;
		m_matrixBuffer.Data.projection = proj;
		m_matrixBuffer.ApplyChanges(context);

		// set position of the matrix constant buffer in the vertex shader 
		bufferNumber = 0;
		buffer = m_matrixBuffer.Buffer();

		context->VSSetConstantBuffers(bufferNumber,1,&buffer);
		// set shader texture array resource in the pixel shader
		context->PSSetShaderResources(0,2,m_textures->GetTextures());

		// setup light constant buffer
		m_lightBuffer.Data.diffuseColor = m_diffuseColor;
		m_lightBuffer.Data.lightDirection = m_lightDirection;
		m_lightBuffer.ApplyChanges(context);

		bufferNumber = 0;
		buffer = m_lightBuffer.Buffer();

		context->PSSetConstantBuffers(bufferNumber,1,&buffer);
	}

	void NormalMapShader::RenderShader(Graphics* graphics)
	{
		ID3D11DeviceContext* context = graphics->GetImmediateContex();

		// set the input layout
		context->IASetInputLayout(m_layout);
		
		// set the vertex and pixel shaders that will be used to render this triangle
		context->VSSetShader(m_vertexShader,0,0);
		context->PSSetShader(m_pixelShader,0,0);

		context->PSSetSamplers(0,1,&m_sampleState);
	}
	
	void NormalMapShader::SetDiffuseColor(DirectX::SimpleMath::Color color)
	{
		m_diffuseColor = color;
	}
	void NormalMapShader::SetLightDiretion(DirectX::SimpleMath::Vector3 lightDir)
	{
		m_lightDirection = lightDir;
	}
}