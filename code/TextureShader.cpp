#include "TextureShader.h"
#include <d3dcompiler.h>
#include "HelperUtilities.h"
namespace Engine
{

	TextureShader::TextureShader() 
		: m_vertexShader(0),m_pixelShader(0),
		m_layout(0),m_matrixBuffer(0),m_sampleState(0)
	{
	}

	TextureShader::~TextureShader()
	{
	}

	bool TextureShader::Initialize(Graphics* graphics,HWND hwnd)
	{

		if(!InitializeShader(graphics,hwnd,L"Content/Shaders/TextureVS.hlsl",L"Content/Shaders/TexturePS.hlsl"))
			return false;

		return true;
	}

	void TextureShader::Shutdown()
	{
		ShutdownShader();
	}

	void TextureShader::Render(Graphics* graphics,int indexCount,
		Matrix world,Matrix view, Matrix proj,
		ID3D11ShaderResourceView* texture)
	{
		SetShaderParameters(graphics,world,view,proj,texture);

		RenderShader(graphics,indexCount);
	}


	bool TextureShader::InitializeShader(Graphics* graphics,HWND hwnd, std::wstring vsFilename, std::wstring psFilename)
	{
		ID3D10Blob *errorMessage,*vertexShaderBuffer,*pixelShaderBuffer;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
		unsigned int numElements;
		D3D11_BUFFER_DESC matrixBufferDesc;

		// Initialize the pointers this function will use to null.
		errorMessage = 0;
		vertexShaderBuffer = 0;
		pixelShaderBuffer = 0;

		HRESULT hr = D3DCompileFromFile(vsFilename.c_str(),0,0,"main","vs_5_0",D3D10_SHADER_ENABLE_STRICTNESS,0,
			&vertexShaderBuffer, &errorMessage);
		if(FAILED(hr))
		{
			// if the shader failed to compile it should have written something to the error message
			if(errorMessage)
				OutputShaderErrorMessage(errorMessage,hwnd,vsFilename);
			else
				MessageBox(hwnd,std::string(vsFilename.begin(),vsFilename.end()).c_str(),"Missing Shader File",0);

			return false;
		}

		hr = D3DCompileFromFile(psFilename.c_str(),0,0,"main","ps_5_0",D3D10_SHADER_ENABLE_STRICTNESS,0,
			&pixelShaderBuffer, &errorMessage);
		if(FAILED(hr))
		{
			// if the shader failed to compile it should have written something to the error message
			if(errorMessage)
				OutputShaderErrorMessage(errorMessage,hwnd,psFilename);
			else
				MessageBox(hwnd,std::string(psFilename.begin(),psFilename.end()).c_str(),"Missing Shader File",0);

			return false;
		}

		BHR(graphics->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),vertexShaderBuffer->GetBufferSize(),0,&m_vertexShader));

		BHR(graphics->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),pixelShaderBuffer->GetBufferSize(),0,&m_pixelShader));

		// Create the vertex input layout description.
		// This setup needs to match the VertexType stucture in the Model and in the shader.
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "TEXCOORD";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		// get a count of the elements in the layout
		numElements = sizeof(polygonLayout)/sizeof(polygonLayout[0]);

		// create the vertex input layout
		BHR(graphics->GetDevice()->CreateInputLayout(polygonLayout,numElements,vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(),&m_layout));

		// release the vertex shader buffer and the pixel shader buffer since they are no longer needed
		ReleaseCOM(vertexShaderBuffer);
		ReleaseCOM(pixelShaderBuffer);

		// setup the description of the dynamic matrix constant buffer that is 
		// in the vertex shader
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		// create the constant buffer pointer so we can access the vertex shader
		// constant buffer from with this class
		BHR(graphics->GetDevice()->CreateBuffer(&matrixBufferDesc,0,&m_matrixBuffer));

		// create a texture sampler state description
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

		memset(samplerDesc.BorderColor,0,4);

		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// create the texture sampler state
		BHR(graphics->GetDevice()->CreateSamplerState(&samplerDesc,&m_sampleState));



		return true;

	}

	void TextureShader::ShutdownShader()
	{
		// release sampler state
		ReleaseCOM(m_sampleState);
		// release the matrix constant buffer
		ReleaseCOM(m_matrixBuffer);
		// release the layout
		ReleaseCOM(m_layout);
		// release the pixel shader
		ReleaseCOM(m_pixelShader);
		// release the vertex shader
		ReleaseCOM(m_vertexShader);
	}

	void TextureShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage,HWND hwnd,std::wstring shaderFilename)
	{
		char* compileErrors;
		unsigned long bufferSize;
		std::ofstream fout;
		// get a pointer to the error message text buffer
		compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());

		// get the lenght of the message
		bufferSize = errorMessage->GetBufferSize();

		// open a file to write the error message to
		fout.open("shader-error.txt");

		// write out the error message
		for(int i = 0; i < bufferSize; i++)
			fout << compileErrors[i];

		// close the filfe
		fout.close();

		// release the error message
		ReleaseCOM(errorMessage);

		// pop a message up on the screen to notify the user to check the 
		// text file for compile errors.
		MessageBox(hwnd,"Error compiling shader.  Check shader-error.txt for message",
			std::string(shaderFilename.begin(),shaderFilename.end()).c_str(),MB_OK);
	}

	void TextureShader::SetShaderParameters(Graphics* graphics,Matrix world,
		Matrix view,Matrix proj,ID3D11ShaderResourceView* texture)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBuffer* dataPtr;
		unsigned int bufferNumber;

		Matrix worldCopy,viewCopy,projCopy;
		ID3D11DeviceContext* context = graphics->GetImmediateContex();

		worldCopy.Transpose(world);
		viewCopy.Transpose(view);
		projCopy.Transpose(proj);

		// lock the constant buffer so it can be written to
		HR(context->Map(m_matrixBuffer,0,
			D3D11_MAP_WRITE_DISCARD,0,&mappedResource));

		// get a pointer to the data in the constant buffer
		dataPtr = static_cast<MatrixBuffer*>(mappedResource.pData);

		// copy the matrices into the constant buffer
		dataPtr->world = worldCopy;
		dataPtr->view = viewCopy;
		dataPtr->projection = projCopy;

		// unlock the constant buffer
		context->Unmap(m_matrixBuffer,0);

		// set the position of constant buffer in the vertex shader
		bufferNumber = 0;
		// finally set the constant buffer in the vertex shader with the updated values
		context->VSSetConstantBuffers(bufferNumber,1,&m_matrixBuffer);

		context->PSSetShaderResources(0,1,&texture);
	}

	void TextureShader::RenderShader(Graphics* graphics,int indexCount)
	{
		// set the vertex layout
		ID3D11DeviceContext* context = graphics->GetImmediateContex();

		context->IASetInputLayout(m_layout);
		// set the vertex and pixel shaders that will be used to render this triangle
		context->VSSetShader(m_vertexShader,0,0);
		context->PSSetShader(m_pixelShader,0,0);

		context->PSSetSamplers(0,1,&m_sampleState);

		// render the triangle
		context->DrawIndexed(indexCount,0,0);
	} 
}