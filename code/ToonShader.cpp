#include "ToonShader.h"
namespace Engine
{

	ToonShader::ToonShader()
		: m_outlinePixelShader(0),m_outlineVertexShader(0),
		m_mainPixelShader(0),m_mainVertexShader(0),
		m_layout(0),m_sampleState(0)
	{ 

	}

	ToonShader::~ToonShader()
	{
	}

	bool ToonShader::Initialize(ID3D11Device* device)
	{
		if(!InitializeShader(device,L"Content/Shaders/ToonVS.cso",
			L"Content/Shaders/ToonPS.cso",
			L"Content/Shaders/ToonOutlineVS.cso",
			L"Content/Shaders/ToonOutlinePS.cso"))
		{
			return false;
		} // end if

		return true;
	}

	void ToonShader::Shutdown()
	{
		ShutdownShader();
	}

	bool ToonShader::Render(Graphics* g, int indexCount, const Matrix& worldMatrix, const Matrix& viewMatrix,
		const Matrix& projectionMatrix, ID3D11ShaderResourceView* texture, const Vector3& lightDirection, 
		const Color& diffuseColor,bool drawLine )
	{

		if(!SetShaderParameters(g->GetImmediateContex(),worldMatrix,viewMatrix,projectionMatrix,texture,lightDirection,diffuseColor))
			return false;

		RenderShader(g,indexCount,drawLine);

		return true;
	}


	bool ToonShader::InitializeShader(ID3D11Device* device, std::wstring vertexShaderFilename, std::wstring pixelShaderFilename,
		std::wstring vertexOutlineShaderFilename, std::wstring pixelOutlineShaderFilename)
	{
		HRESULT result;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
		unsigned int numElements;
		D3D11_SAMPLER_DESC samplerDesc;

		struct ShaderBuffer
		{
			unsigned int size;
			void* data;
		};
		ShaderBuffer VSBuffer;
		ShaderBuffer PSBuffer;
		memset(&VSBuffer,0,sizeof(ShaderBuffer));
		memset(&PSBuffer,0,sizeof(ShaderBuffer));

		VSBuffer.data = LoadCompiledShader(vertexShaderFilename.c_str(),VSBuffer.size);
		PSBuffer.data = LoadCompiledShader(pixelShaderFilename.c_str(),PSBuffer.size);

		// Create the vertex shader from the buffer.
		result = device->CreateVertexShader(VSBuffer.data, VSBuffer.size, NULL, &m_mainVertexShader);
		if(FAILED(result))
		{
			return false;
		}
		// Create the pixel shader from the buffer.
		result = device->CreatePixelShader(PSBuffer.data, PSBuffer.size, NULL, &m_mainPixelShader);
		if(FAILED(result))
		{
			return false;
		}

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

		// Get a count of the elements in the layout.
		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		// Create the vertex input layout.
		result = device->CreateInputLayout(polygonLayout, numElements,VSBuffer.data,VSBuffer.size, &m_layout);
		if(FAILED(result))
		{
			return false;
		}
		delete VSBuffer.data;
		delete PSBuffer.data;
		memset(&VSBuffer,0,sizeof(ShaderBuffer));
		memset(&PSBuffer,0,sizeof(ShaderBuffer));
		// now for the outline shaders....
		VSBuffer.data = LoadCompiledShader(vertexOutlineShaderFilename.c_str(),VSBuffer.size);
		PSBuffer.data = LoadCompiledShader(pixelOutlineShaderFilename.c_str(),PSBuffer.size);
		// Create the vertex shader from the buffer.
		result = device->CreateVertexShader(VSBuffer.data,VSBuffer.size, NULL, &m_outlineVertexShader);
		if(FAILED(result))
		{
			return false;
		}

		// Create the pixel shader from the buffer.
		result = device->CreatePixelShader(PSBuffer.data, PSBuffer.size, NULL, &m_outlinePixelShader);
		if(FAILED(result))
		{
			return false;
		}

		if(FAILED(result))
		{
			return false;
		}

		// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
		delete VSBuffer.data;
		delete PSBuffer.data;
		memset(&VSBuffer,0,sizeof(ShaderBuffer));
		memset(&PSBuffer,0,sizeof(ShaderBuffer));


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

		// Create the texture sampler state.
		result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
		if(FAILED(result))
		{
			return false;
		}

		// initialize the constant buffers
		m_matrixBuffer.Initialize(device);
		m_lightBuffer.Initialize(device);

		return true;

	}

	void ToonShader::ShutdownShader()
	{

		// Release the sampler state.
		if(m_sampleState)
		{
			m_sampleState->Release();
			m_sampleState = 0;
		}

		// Release the layout.
		if(m_layout)
		{
			m_layout->Release();
			m_layout = 0;
		}

		// Release the pixel shader.
		if(m_mainPixelShader)
		{
			m_mainPixelShader->Release();
			m_mainPixelShader = 0;
		}

		// Release the vertex shader.
		if(m_mainVertexShader)
		{
			m_mainVertexShader->Release();
			m_mainVertexShader = 0;
		}

		// Release the pixel shader.
		if(m_outlinePixelShader)
		{
			m_outlinePixelShader->Release();
			m_outlinePixelShader = 0;
		}

		// Release the vertex shader.
		if(m_outlineVertexShader)
		{
			m_outlineVertexShader->Release();
			m_outlineVertexShader = 0;
		}

	}
	/*
	void ToonShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
	{
	char* compileErrors;
	SIZE_T bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
	fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(0, "Error compiling shader.  Check shader-error.txt for message.", "", MB_OK);

	return;
	}
	*/
	bool ToonShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const Matrix& worldMatrix, const Matrix& viewMatrix,
		const Matrix& projectionMatrix, ID3D11ShaderResourceView* texture, const Vector3& lightDirection, 
		const Color& diffuseColor)
	{
		unsigned int bufferNumber;
		// Transpose the matrices and 		
		// update data in the constant buffer
		m_matrixBuffer.Data.World = worldMatrix.Transpose();
		m_matrixBuffer.Data.WorldInverseTranspose = worldMatrix.Invert().Transpose();
		m_matrixBuffer.Data.View = viewMatrix.Transpose();
		m_matrixBuffer.Data.Projection = projectionMatrix.Transpose();
		// set the constant buufer
		m_matrixBuffer.ApplyChanges(deviceContext);

		// Set the position of the constant buffer in the vertex shader.
		bufferNumber = 0;
		ID3D11Buffer* cBuffer[1] = {m_matrixBuffer.Buffer() };
		// Now set the constant buffer in the vertex shader with the updated values.
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, cBuffer);

		// Set shader texture resource in the pixel shader.
		deviceContext->PSSetShaderResources(0, 1, &texture);

		// Copy the lighting variables into the constant buffer.
		m_lightBuffer.Data.Intensity = 1.0f;
		m_lightBuffer.Data.DiffuseColor = diffuseColor;
		m_lightBuffer.Data.Direction = lightDirection;
		// set the constant buffer
		m_lightBuffer.ApplyChanges(deviceContext);

		// Set the position of the light constant buffer in the pixel shader.
		bufferNumber = 0;
		ID3D11Buffer* cLightBuffer[1] = {m_lightBuffer.Buffer() };
		// Finally set the light constant buffer in the pixel shader with the updated values.
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, cLightBuffer);

		return true;
	}

	void ToonShader::RenderShader(Graphics* graphics, int indexCount, bool drawLine)
	{
		// get our graphics devices
		auto deviceContext = graphics->GetImmediateContex();
		auto device = graphics->GetDevice();
		// set the layout
		deviceContext->IASetInputLayout(m_layout);
		if(drawLine)
		{
			// draw the outline first
			deviceContext->RSSetState(graphics->GetOutlineRasterState());
			deviceContext->VSSetShader(m_outlineVertexShader,0,0);
			deviceContext->PSSetShader(m_outlinePixelShader,0,0);
			deviceContext->DrawIndexed(indexCount,0,0);
		}
		// draw the rest of the model in a toony way
		deviceContext->RSSetState(graphics->GetRasterState()); // set the renderstate
		deviceContext->VSSetShader(m_mainVertexShader,0,0);    // set vertexShader 
		deviceContext->PSSetShader(m_mainPixelShader,0,0);     // set pixelShader
		deviceContext->PSSetSamplers(0,1,&m_sampleState);      // set the sampler State
		deviceContext->DrawIndexed(indexCount,0,0);			   // draw the indexed model
	}

	// loads and returns a pointer to a compiled shader in memory
	void* ToonShader::LoadCompiledShader(const wchar_t* filename, UINT& size)
	{
		// open the file
		std::ifstream ifs(filename, std::ios::binary);
		// make sure it is good
		if (ifs.bad() || ifs.fail())
		{
			std::wstring failmsg = L"Failed to load shader from ";
			failmsg.append(filename);
			return 0;
		}
		// get the size of the file
		ifs.seekg(0, std::ios::end);
		size = (UINT)ifs.tellg();
		// create some memory
		char* buffer = new char[size];
		// set position to the begining of the file
		ifs.seekg(0, std::ios::beg);
		// read the file
		ifs.read(buffer, size);
		// close the file
		ifs.close();
		// return the buffer
		return buffer;
	} 
}