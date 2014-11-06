#pragma once
#include "gamescreen.h"
#include <SpriteFont.h>
#include <SimpleMath.h>
#include <d3d11.h>
#include "Camera.h"
#include "Model.h"
//#include <Model.h> // dxtk model
#include "LightShader.h"
#include "ToonShader.h"
#include "MultiTextureShader.h"
#include "Light.h"
#include "Frustum.h"

using namespace DirectX::SimpleMath;
namespace Engine
{

	class GameplayScreen :
		public GameScreen
	{
	private:
		DirectX::SpriteFont* m_Font;

		float m_PauseAlpha;
		Camera* m_camera;
		std::unique_ptr<Model> m_model;
		LightShader* m_lightShader;
		ToonShader* m_toonShader;
		Light* m_light;
		Frustum* m_frustum;
		//ModelListClass* m_modelList;
		MultiTextureShader* m_multiTextureShader;
		float rotation; // set in constructor
		float rotationX, rotationY,rotationZ;
	public:
		GameplayScreen(void);
		~GameplayScreen(void);
		void Load();
		void UnLoad();
		void HandleInput(InputState* input);
		void Update(float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
		void Draw(float deltaTime);
	};


}