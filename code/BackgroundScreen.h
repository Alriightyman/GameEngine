#pragma once
#include "gamescreen.h"
#include <d3d11.h>
namespace Engine
{

	class BackgroundScreen :
		public GameScreen
	{
	private:
		ID3D11ShaderResourceView* m_BackgroundTexture;
	public:
		BackgroundScreen(void);
		~BackgroundScreen(void);
		void Load();
		void UnLoad();
		void Update(float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
		void Draw(float deltaTime);
	};

}
