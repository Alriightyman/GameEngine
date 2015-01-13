#pragma once
#include "gamescreen.h"
#include <SpriteFont.h>
#include <SimpleMath.h>
#include <d3d11.h>
#include "Camera.h"
#include "Light.h"
#include "Frustum.h"
#include "ObjectManager.h"

using namespace DirectX::SimpleMath;
namespace Engine
{

	class GameplayScreen :
		public GameScreen
	{
	private:
		DirectX::SpriteFont* m_Font;

		float m_PauseAlpha;
		std::unique_ptr<Camera> m_camera;
		
		Light m_light;
		Frustum m_frustum;
		
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