////////////////////////////////////////////////////////////////////////////////
// Filename: Model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "model.h"
#include <cmath>
#include <string>
#include "Script.h"

namespace Engine
{

	#pragma region Mesh

	Model::Mesh::Mesh()
		: Textures(0)
	{
	}

	Model::Mesh::~Mesh()
	{
		if(Textures)
		{
			Textures->Shutdown();
			delete Textures;
			Textures = 0;
		}

	}
#pragma endregion


	#pragma region Model

	Model::Model()
	{
	
	}


	Model::Model(const Model& other)
	{

	}


	Model::~Model()
	{
		Shutdown();
	}

	void Model::Bind(Script* script)
	{
		using namespace luabridge;
		if (!isInitialized)
		{
			isInitialized = true;
			getGlobalNamespace(script->GetState())
				.beginClass<Model>("Model")
				.endClass();
		}
	}

	bool Model::isInitialized = false;

	bool Model::Initialize(Graphics* graphics,std::wstring modelFilename)
	{
		bool result;
		// load the model data
		result = LoadModel(graphics, modelFilename);

		if(!result)
			return false;

		// Calculate the tangent, binormal, and normals
		CalculateModelVectors();

		// Initialize the vertex and index buffers.
		result = InitializeBuffers(graphics);
		if(!result)
			return false;

		return true;
	}

	bool Model::LoadModel(Graphics* graphics, std::wstring filename)
	{
		std::ifstream fin;
		std::string ignore;
		// open the model file
		fin.open(filename);

		// if it could not open the file then exit
		if(fin.fail())
			return false;

		// read the vertex count
		fin >> ignore >> ignore >> m_vertexCount;
		// get the mesh count
		int meshCount = 0;
		fin >> ignore >> ignore >> meshCount;
		this->m_shaderType =  ShaderType::NORMALMAP; ;
		// create the mesh data
		for (int m = 0; m < meshCount; m++)
		{
			Mesh* mesh = new Mesh();
			// get the vertex start location
			fin >> ignore >> mesh->Start;
			// get the vertex count
			fin >> ignore >> mesh->Count;

			// get the diffuse/normal/specular maps
			std::string diffuse,normal,spec;

			fin >> ignore >> diffuse;
			fin >> ignore >> normal;
			fin >> ignore >> spec;
			std::string path = "Content/Textures/";
			diffuse = path + diffuse;
			normal = path + normal;
			spec = path + spec;
			// create and set the texture
			mesh->Textures = graphics->CreateTexture(std::wstring(diffuse.begin(),diffuse.end()),std::wstring(normal.begin(),normal.end()),std::wstring(spec.begin(),spec.end()));
			// set a default shader type
			m_meshes.push_back(mesh);
		}

		// set the number of indices to be the same as the vertex count
		m_indexCount = m_vertexCount;

		// create the model using the vertex count that was read in
		m_model = new ModelData[m_vertexCount];

		if(!m_model)
			return false;

		// read the vertex data
		for(int i = 0; i <  m_vertexCount; i++)
		{
			fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
			fin >> m_model[i].tu >> m_model[i].tv;
			fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
		}


		fin.close();

		return true;

	}

#pragma region Vertex Calculation Methods
	void Model::CalculateModelVectors()
	{

		int faceCount, index;
		// calculate the number of faces
		faceCount = m_vertexCount / 3;

		// initializee the index to the model data
		index = 0;
		// go through all the faces and calculate the tangent, binormal, and normal vectors
		for(int i = 0; i < faceCount; i++)
		{
			TempVertex v1,v2,v3;
			Vector3 tangent,binormal,normal;
			// get the three vertices for this face
			v1.x = m_model[index].x;
			v1.y = m_model[index].y;
			v1.z = m_model[index].z;
			v1.tu = m_model[index].tu;
			v1.tv = m_model[index].tv;
			v1.nx = m_model[index].nx;
			v1.ny = m_model[index].ny;
			v1.nz = m_model[index].nz;
			index++;

			v2.x = m_model[index].x;
			v2.y = m_model[index].y;
			v2.z = m_model[index].z;
			v2.tu = m_model[index].tu;
			v2.tv = m_model[index].tv;
			v2.nx = m_model[index].nx;
			v2.ny = m_model[index].ny;
			v2.nz = m_model[index].nz;
			index++;

			v3.x = m_model[index].x;
			v3.y = m_model[index].y;
			v3.z = m_model[index].z;
			v3.tu = m_model[index].tu;
			v3.tv = m_model[index].tv;
			v3.nx = m_model[index].nx;
			v3.ny = m_model[index].ny;
			v3.nz = m_model[index].nz;
			index++;

			// calculate the tangent and binormals
			CalculateTangentBinormal(v1,v2,v3,tangent,binormal);

			// calculate the normals based on the tangents and binormals
			CalculateNormal(tangent,binormal,normal);

			m_model[index - 1].nx = normal.x;
			m_model[index - 1].ny = normal.y;
			m_model[index - 1].nz = normal.z;
			m_model[index - 1].tx = tangent.x;
			m_model[index - 1].ty = tangent.y;
			m_model[index - 1].tz = tangent.z;
			m_model[index - 1].bx = binormal.x;
			m_model[index - 1].by = binormal.y;
			m_model[index - 1].bz = binormal.z;

			m_model[index - 2].nx = normal.x;
			m_model[index - 2].ny = normal.y;
			m_model[index - 2].nz = normal.z;
			m_model[index - 2].tx = tangent.x;
			m_model[index - 2].ty = tangent.y;
			m_model[index - 2].tz = tangent.z;
			m_model[index - 2].bx = binormal.x;
			m_model[index - 2].by = binormal.y;
			m_model[index - 2].bz = binormal.z;

			m_model[index - 3].nx = normal.x;
			m_model[index - 3].ny = normal.y;
			m_model[index - 3].nz = normal.z;
			m_model[index - 3].tx = tangent.x;
			m_model[index - 3].ty = tangent.y;
			m_model[index - 3].tz = tangent.z;
			m_model[index - 3].bx = binormal.x;
			m_model[index - 3].by = binormal.y;
			m_model[index - 3].bz = binormal.z;
		}
	}

	void Model::CalculateTangentBinormal(TempVertex vertex1,TempVertex vertex2,TempVertex vertex3, Vector3& tangent,Vector3& binormal)
	{
		Vector3 vector1,vector2;
		Vector3 tuVector, tvVector;
		float den;
		//float length;


		// Calculate the two vectors for this face.
		vector1.x = vertex2.x - vertex1.x;
		vector1.y = vertex2.y - vertex1.y;
		vector1.z = vertex2.z - vertex1.z;

		vector2.x = vertex3.x - vertex1.x;
		vector2.y = vertex3.y - vertex1.y;
		vector2.z = vertex3.z - vertex1.z;

		// Calculate the tu and tv texture space vectors.
		tuVector.x = vertex2.tu - vertex1.tu;
		tvVector.x = vertex2.tv - vertex1.tv;

		tuVector.y = vertex3.tu - vertex1.tu;
		tvVector.y = vertex3.tv - vertex1.tv;

		tuVector.z = 0;
		tvVector.z = 0;

		// Calculate the denominator of the tangent/binormal equation.
		den = 1.0f / (tuVector.x * tvVector.y - tuVector.y * tvVector.x);

		// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
		tangent.x = (tvVector.y * vector1.x - tvVector.x * vector2.x) * den;
		tangent.y = (tvVector.y * vector1.y - tvVector.x * vector2.y) * den;
		tangent.z = (tvVector.y * vector1.z - tvVector.x * vector2.z) * den;

		binormal.x = (tuVector.x * vector2.x - tuVector.y * vector1.x) * den;
		binormal.y = (tuVector.x * vector2.y - tuVector.y * vector1.y) * den;
		binormal.z = (tuVector.x * vector2.z - tuVector.y * vector1.z) * den;

		tangent.Normalize();
		
		binormal.Normalize();
		
	}

	void Model::CalculateNormal(Vector3 tangent, Vector3 binormal, Vector3& normal)
	{
		// calculate the cross product of the tangent and binormal to get normal
		normal = tangent.Cross(binormal);

		// normalize the normal
		normal.Normalize();
	}
#pragma endregion
	
	void Model::Shutdown()
	{
		while(!m_meshes.empty())
		{
			Mesh* m = m_meshes.back();
			m_meshes.pop_back();
			delete m;
			m = 0;
		}
		// Shutdown the vertex and index buffers.
		ShutdownBuffers();
	}


	void Model::Render(Graphics* graphics)
	{
		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers(graphics);

		// render all meshes in the model
		for(unsigned int i = 0; i < m_meshes.size(); i++)
		{
			// get the shader for this mesh
			Shader* shader = graphics->GetShader(m_shaderType);

			if(shader != nullptr)
			{
				// set the texture for the shader
				shader->SetTexture(m_meshes[i]->Textures);
				// render mesh with shader parameters
				shader->Render(graphics);
				// draw indexed mesh
				graphics->GetImmediateContex()->DrawIndexed(m_meshes[i]->Count,m_meshes[i]->Start,0);
			}
			
		}
	}


	bool Model::InitializeBuffers(Graphics* graphics)
	{

		Vertex* vs;		
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;
		ID3D11Device* device = graphics->GetDevice();

		// Create the vertex array.
		int size = m_vertexCount;
		vs = new Vertex[size];
		//vertices.resize(size);
		size = m_indexCount;
		// Create the index array.
		indices = new unsigned long[size];

		// Load the vertex array with data.
		for (int i = 0; i < m_vertexCount; i++)
		{

			vs[i].position = Vector3(m_model[i].x, m_model[i].y, m_model[i].z); 
			vs[i].texture = Vector2(m_model[i].tu, m_model[i].tv);
			vs[i].normal = Vector3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
			vs[i].tangent = Vector3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
			vs[i].binormal = Vector3(m_model[i].bx, m_model[i].by, m_model[i].bz);

			indices[i] = i;

		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vs;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete [] vs;
		delete [] indices;

		return true;
	}


	void Model::ShutdownBuffers()
	{
		while(!m_meshes.empty())
		{
			Mesh* m = m_meshes.back();
			
			if(m->Textures)
			{
				m->Textures->Shutdown();
				delete m->Textures;
				m->Textures = 0;
			}
			delete m;
			m = 0;
			m_meshes.pop_back();
		}
	}


	void Model::RenderBuffers(Graphics* graphics)
	{
		unsigned int stride;
		unsigned int offset;

		ID3D11DeviceContext* deviceContext = graphics->GetImmediateContex();
		// Set vertex buffer stride and offset.
		stride = sizeof(Vertex); 
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
	}

#pragma endregion

/*
	Model::Mesh::Mesh()
		: VertexBuffer(0),IndexBuffer(0),Textures(0)
	{
		this->Model = 0;
	}

	Model::Mesh::~Mesh()
	{
	
		if(this->Model)
		{
			delete [] this->Model;
			this->Model = 0;
		}

		if(VertexBuffer)
		{
			VertexBuffer->Release();
			VertexBuffer = 0;
		}

		if(IndexBuffer)
		{
			IndexBuffer->Release();
			IndexBuffer = 0;
		}

		if(Textures)
		{
			Textures->Shutdown();
			delete Textures;
			Textures = 0;
		}

	}

	Model::Model()
	{
	
	}


	Model::Model(const Model& other)
	{

	}


	Model::~Model()
	{
		Shutdown();
	}


	bool Model::Initialize(Graphics* graphics,std::wstring modelFilename, Texture* texture)
	{
		bool result;
		// load the model data
		result = LoadModel(modelFilename);
		if(!result)
			return false;

		// Calculate the tangent, binormal, and normals
		CalculateModelVectors();

		// Initialize the vertex and index buffers.
		result = InitializeBuffers(graphics);
		if(!result)
		{
			return false;
		}

		for(int i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i]->Textures = texture;

//			delete [] m_meshes[i]->Model;
//			m_meshes[i]->Model = 0;
		}



		// Load the texture for this model.
		// m_Textures = texture;

		return true;
	}

	bool Model::LoadModel(std::wstring filename)
	{
		std::ifstream fin;
		char input;
		std::string ignore;
		// open the model file
		fin.open(filename);

		// if it could not open the file then exit
		if(fin.fail())
			return false;

		//fin.get(input);

		//while(input != ':')
		//	fin.get(input);

		// read the vertex count
		
		fin >> ignore >> ignore >> m_vertexCount;

		int meshCount = 0;

		fin >> ignore >> ignore >> meshCount;
		int offset = 0;
		for (int m = 0; m < meshCount; m++)
		{
			Mesh* mesh = new Mesh();

			fin >> ignore >> mesh->Start;

			fin >> ignore >> mesh->Count;

			m_meshes.push_back(mesh);
		}

		for (int m = 0; m < meshCount; m++)
		{
			//mesh.Vertices.resize(mesh.Count);
			
			// set the number of indices to be the same as the vertex count
			m_meshes[m]->IndexCount = m_meshes[m]->Count;

			// create the model using the vertex count that was read in
			m_meshes[m]->Model = new ModelData[m_meshes[m]->Count];

			if(!m_meshes[m]->Model)
				return false;

			// read the vertex data
			for(int i = 0; i <  m_meshes[m]->Count; i++)
			{
				fin >> m_meshes[m]->Model[i].x >> m_meshes[m]->Model[i].y >> m_meshes[m]->Model[i].z;
				fin >> m_meshes[m]->Model[i].tu >> m_meshes[m]->Model[i].tv;
				fin >> m_meshes[m]->Model[i].nx >> m_meshes[m]->Model[i].ny >> m_meshes[m]->Model[i].nz;
			}
		}

		fin.close();

		return true;

	}

	void Model::CalculateModelVectors()
	{
		for(int m = 0;m < m_meshes.size(); m++)
		{
			int faceCount, index;
			TempVertex v1,v2,v3;
			Vector3 tangent,binormal,normal;

			// calculate the number of faces
			faceCount = m_meshes[m]->Count / 3;

			// initializee the index to the model data
			index = 0;
			// go through all the faces and calculate the tangent, binormal, and normal vectors
			for(int i = 0; i < faceCount; i++)
			{
				// get the three vertices for this face
				v1.x = m_meshes[m]->Model[index].x;
				v1.y =  m_meshes[m]->Model[index].y;
				v1.z =  m_meshes[m]->Model[index].z;
				v1.tu =  m_meshes[m]->Model[index].tu;
				v1.tv =  m_meshes[m]->Model[index].tv;
				v1.nx =  m_meshes[m]->Model[index].nx;
				v1.ny =  m_meshes[m]->Model[index].ny;
				v1.nz =  m_meshes[m]->Model[index].nz;
				index++;

				v2.x =  m_meshes[m]->Model[index].x;
				v2.y =  m_meshes[m]->Model[index].y;
				v2.z =  m_meshes[m]->Model[index].z;
				v2.tu =  m_meshes[m]->Model[index].tu;
				v2.tv =  m_meshes[m]->Model[index].tv;
				v2.nx =  m_meshes[m]->Model[index].nx;
				v2.ny =  m_meshes[m]->Model[index].ny;
				v2.nz =  m_meshes[m]->Model[index].nz;
				index++;

				v3.x =  m_meshes[m]->Model[index].x;
				v3.y =  m_meshes[m]->Model[index].y;
				v3.z =  m_meshes[m]->Model[index].z;
				v3.tu =  m_meshes[m]->Model[index].tu;
				v3.tv =  m_meshes[m]->Model[index].tv;
				v3.nx =  m_meshes[m]->Model[index].nx;
				v3.ny =  m_meshes[m]->Model[index].ny;
				v3.nz =  m_meshes[m]->Model[index].nz;
				index++;

				// calculate the tangent and binormals
				CalculateTangentBinormal(v1,v2,v3,tangent,binormal);

				// calculate the normals based on the tangents and binormals
				CalculateNormal(tangent,binormal,normal);

				 m_meshes[m]->Model[index - 1].nx = normal.x;
				 m_meshes[m]->Model[index - 1].ny = normal.y;
				 m_meshes[m]->Model[index - 1].nz = normal.z;
				 m_meshes[m]->Model[index - 1].tx = tangent.x;
				 m_meshes[m]->Model[index - 1].ty = tangent.y;
				 m_meshes[m]->Model[index - 1].tz = tangent.z;
				 m_meshes[m]->Model[index - 1].bx = binormal.x;
				 m_meshes[m]->Model[index - 1].by = binormal.y;
				 m_meshes[m]->Model[index - 1].bz = binormal.z;

				 m_meshes[m]->Model[index - 2].nx = normal.x;
				m_meshes[m]->Model[index - 2].ny = normal.y;
				m_meshes[m]->Model[index - 2].nz = normal.z;
				m_meshes[m]->Model[index - 2].tx = tangent.x;
				m_meshes[m]->Model[index - 2].ty = tangent.y;
				m_meshes[m]->Model[index - 2].tz = tangent.z;
				m_meshes[m]->Model[index - 2].bx = binormal.x;
				m_meshes[m]->Model[index - 2].by = binormal.y;
				m_meshes[m]->Model[index - 2].bz = binormal.z;

				m_meshes[m]->Model[index - 3].nx = normal.x;
				m_meshes[m]->Model[index - 3].ny = normal.y;
				m_meshes[m]->Model[index - 3].nz = normal.z;
				m_meshes[m]->Model[index - 3].tx = tangent.x;
				m_meshes[m]->Model[index - 3].ty = tangent.y;
				m_meshes[m]->Model[index - 3].tz = tangent.z;
				m_meshes[m]->Model[index - 3].bx = binormal.x;
				m_meshes[m]->Model[index - 3].by = binormal.y;
				m_meshes[m]->Model[index - 3].bz = binormal.z;
			}

		}
	}

	void Model::CalculateTangentBinormal(TempVertex vertex1,TempVertex vertex2,TempVertex vertex3, Vector3& tangent,Vector3& binormal)
	{
		Vector3 vector1,vector2;
		Vector3 tuVector, tvVector;
		float den;
		float length;


		// Calculate the two vectors for this face.
		vector1.x = vertex2.x - vertex1.x;
		vector1.y = vertex2.y - vertex1.y;
		vector1.z = vertex2.z - vertex1.z;

		vector2.x = vertex3.x - vertex1.x;
		vector2.y = vertex3.y - vertex1.y;
		vector2.z = vertex3.z - vertex1.z;

		// Calculate the tu and tv texture space vectors.
		tuVector.x = vertex2.tu - vertex1.tu;
		tvVector.x = vertex2.tv - vertex1.tv;

		tuVector.y = vertex3.tu - vertex1.tu;
		tvVector.y = vertex3.tv - vertex1.tv;

		tuVector.z = 0;
		tvVector.z = 0;

		// Calculate the denominator of the tangent/binormal equation.
		den = 1.0f / (tuVector.x * tvVector.y - tuVector.y * tvVector.x);
		if(den == INFINITY || den == NAN)
			den = 0;

		// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
		tangent.x = (tvVector.y * vector1.x - tvVector.x * vector2.x) * den;
		tangent.y = (tvVector.y * vector1.y - tvVector.x * vector2.y) * den;
		tangent.z = (tvVector.y * vector1.z - tvVector.x * vector2.z) * den;

		binormal.x = (tuVector.x * vector2.x - tuVector.y * vector1.x) * den;
		binormal.y = (tuVector.x * vector2.y - tuVector.y * vector1.y) * den;
		binormal.z = (tuVector.x * vector2.z - tuVector.y * vector1.z) * den;

		tangent.Normalize();
		
		binormal.Normalize();
		
	}

	void Model::CalculateNormal(Vector3 tangent, Vector3 binormal, Vector3& normal)
	{
		// calculate the cross product of the tangent and binormal to get normal
		normal = tangent.Cross(binormal);

		// normalize the normal
		normal.Normalize();
	}

	void Model::Shutdown()
	{
		while(!m_meshes.empty())
		{
			Mesh* m = m_meshes.back();
			m_meshes.pop_back();
			delete m;
			m = 0;
		}
		// Shutdown the vertex and index buffers.
		ShutdownBuffers();
	}


	void Model::Render(Graphics* graphics,Shader* shader)
	{
		for(int i = 0; i < m_meshes.size(); i++)
		{
			// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
			RenderBuffers(graphics,m_meshes[i]);

			shader->SetTexture(m_meshes[i]->Textures);

			if(shader != nullptr)
				shader->Render(graphics);

			graphics->GetImmediateContex()->DrawIndexed(m_meshes[i]->IndexCount,m_meshes[i]->Start,0);
		}
	}


	bool Model::InitializeBuffers(Graphics* graphics)
	{
		for(int m = 0; m < m_meshes.size(); m++)
		{
			Vertex* vs;
			
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;
			ID3D11Device* device = graphics->GetDevice();

			// Create the vertex array.
			int size = m_meshes[m]->Count;
			vs = new Vertex[size];
			//vertices.resize(size);
			size = m_meshes[m]->IndexCount;
			// Create the index array.
			indices = new unsigned long[size];

			// Load the vertex array with data.
			for (int i = 0; i < m_meshes[m]->Count; i++)
			{

				vs[i].position = Vector3(m_meshes[m]->Model[i].x, m_meshes[m]->Model[i].y, m_meshes[m]->Model[i].z); 
				vs[i].texture = Vector2(m_meshes[m]->Model[i].tu, m_meshes[m]->Model[i].tv);
				vs[i].normal = Vector3(m_meshes[m]->Model[i].nx, m_meshes[m]->Model[i].ny, m_meshes[m]->Model[i].nz);
				vs[i].tangent = Vector3(m_meshes[m]->Model[i].tx, m_meshes[m]->Model[i].ty, m_meshes[m]->Model[i].tz);
				vs[i].binormal = Vector3(m_meshes[m]->Model[i].bx, m_meshes[m]->Model[i].by, m_meshes[m]->Model[i].bz);

				indices[i] = i;

			}

			// Set up the description of the static vertex buffer.
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_meshes[m]->Count;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = vs;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			// Now create the vertex buffer.
			result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_meshes[m]->VertexBuffer);
			if(FAILED(result))
			{
				return false;
			}

			// Set up the description of the static index buffer.
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_meshes[m]->IndexCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the index data.
			indexData.pSysMem = indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer.
			result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_meshes[m]->IndexBuffer);
			if(FAILED(result))
			{
				return false;
			}

			// Release the arrays now that the vertex and index buffers have been created and loaded.
			delete [] vs;
			delete [] indices;

		}
		return true;
	}


	void Model::ShutdownBuffers()
	{
		while(!m_meshes.empty())
		{
			m_meshes.pop_back();
		}
	}


	void Model::RenderBuffers(Graphics* graphics, Mesh* mesh)
	{
		unsigned int stride;
		unsigned int offset;

		ID3D11DeviceContext* deviceContext = graphics->GetImmediateContex();
		// Set vertex buffer stride and offset.
		stride = sizeof(Vertex); 
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &mesh->VertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(mesh->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

*/

#pragma region MODEL_LIST
	/////////////////////////////////////////////////////
	/////////// MODEL LIST CLASS ///////////////////////
	///////////////////////////////////////////////////


	ModelListClass::ModelListClass()
	{
		m_ModelInfoList = 0;
	}


	ModelListClass::ModelListClass(const ModelListClass& other)
	{
	}


	ModelListClass::~ModelListClass()
	{
	}


	bool ModelListClass::Initialize(int numModels)
	{
		int i;
		float red, green, blue;

		// Store the number of models.
		m_modelCount = numModels;

		// Create a list array of the model information.
		m_ModelInfoList = new ModelInfoType[m_modelCount];
		if(!m_ModelInfoList)
		{
			return false;
		}
		// Seed the random generator with the current time.
		srand((unsigned int)time(NULL));

		// Go through all the models and randomly generate the model color and position.
		for(i=0; i<m_modelCount; i++)
		{
			// Generate a random color for the model.
			red = (float)rand() / RAND_MAX;
			green = (float)rand() / RAND_MAX;
			blue = (float)rand() / RAND_MAX;

			m_ModelInfoList[i].color = Color(red, green, blue, 1.0f);

			// Generate a random position in front of the viewer for the mode.
			m_ModelInfoList[i].positionX = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
			m_ModelInfoList[i].positionY = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
			m_ModelInfoList[i].positionZ = ((((float)rand()-(float)rand())/RAND_MAX) * 10.0f) + 5.0f;
		}

		return true;
	}

	void ModelListClass::Shutdown()
	{
		// Release the model information list.
		if(m_ModelInfoList)
		{
			delete [] m_ModelInfoList;
			m_ModelInfoList = 0;
		}

		return;
	}

	int ModelListClass::GetModelCount()
	{
		return m_modelCount;
	}

	void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ, Color& color)
	{
		positionX = m_ModelInfoList[index].positionX;
		positionY = m_ModelInfoList[index].positionY;
		positionZ = m_ModelInfoList[index].positionZ;

		color = m_ModelInfoList[index].color;

		return;
	} 
#pragma endregion

}