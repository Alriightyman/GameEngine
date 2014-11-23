#pragma once
#include "ScreenManager.h"
#include "Graphics.h"

namespace Engine
{
	class Game
	{
	private:
		HWND m_hwnd;
		bool m_isActive;

		Graphics* m_graphics;
		ScreenManager* m_screenManager;

	public:
		Game(void);
		~Game(void);
		bool Init(HINSTANCE hInst, int width, int height, bool windowed);
		void Update(float deltaTime);
		void Draw(float deltaTime); 
		void Cleanup();
		void Quit();

		LRESULT MsgProc(HWND hwnd,UINT msg, WPARAM wparam,LPARAM lparam);


	}; 
}

