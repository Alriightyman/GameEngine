#pragma once


#include <d3d11.h>
#include <SimpleMath.h>
#include <fstream>
#include "texture.h"
#include "Shader.h"
using namespace DirectX::SimpleMath;

/* 
|
*/
namespace Engine
{

	class Model
	{
		// make friends with the Graphics class
		friend class Graphics;
	private:
		// static vertex buffer
		struct StaticVertex
		{
			Vector3 position;
			Vector2 texture;
			Vector3 normal;

			StaticVertex()
			{
				position = Vector3(0,0,0);
				texture = Vector2(0,0);
				normal = Vector3(0,0,0);
			}
		};

		// has a little skinned data
		struct Vertex
		{
			Vector3 position;
			Vector2 texture;
			Vector3 normal;
			Vector4 weight;
			Vector4 weightIndex;

			Vertex()
			{
				position = Vector3(0,0,0);
				texture = Vector2(0,0);
				normal = Vector3(0,0,0);
			}
		};


		struct ModelData
		{
			float x,y,z;
			float tu,tv;
			float nx,ny,nz;
		};

	private:
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
		int m_vertexCount, m_indexCount;
		Texture* m_Textures;
		ModelData* m_model;

	public:
		Model();
		Model(const Model&);
		~Model();

		void Shutdown();
		int GetIndexCount();
		Texture* GetTextures();
	protected:
		bool Initialize(Graphics* graphics,std::wstring modelFilename, Texture* texture);
		void Render(Graphics* graphics, Shader* shader);
	private:
		bool InitializeBuffers(Graphics*);
		void ShutdownBuffers();
		void RenderBuffers(Graphics*);

		bool LoadModel(std::wstring filename);
		void ReleaseModel();

		void ReleaseTextures();
	};






	//////////////
	// INCLUDES //
	//////////////
	//#include <SimpleMath.h>
#include <stdlib.h>
#include <time.h>


	///////////////////////////////////////////////////////////////////////////////
	// Class name: ModelListClass
	///////////////////////////////////////////////////////////////////////////////
	class ModelListClass
	{
	private:
		struct ModelInfoType
		{
			Color color;
			float positionX, positionY, positionZ;
		};

	public:
		ModelListClass();
		ModelListClass(const ModelListClass&);
		~ModelListClass();

		bool Initialize(int);
		void Shutdown();

		int GetModelCount();
		void GetData(int, float&, float&, float&, Color&);

	private:
		int m_modelCount;
		ModelInfoType* m_ModelInfoList;
	};




}