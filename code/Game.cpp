#include "Game.h"
#include "BackgroundScreen.h"
#include "MainMenuScreen.h"
#include "GameplayScreen.h"
#include <exception>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>

using namespace DirectX;

namespace
{
	// This is just used to forward Windows messages from a global window
	// procedure to our member function window procedure because we cannot
	// assign a member function to WNDCLASS::lpfnWndProc.

	Engine::Game* g_game = 0;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd,UINT msg, WPARAM wparam,LPARAM lparam)
{
	// Forward hwnd on because we can get messages(e.f, wm_create)
	// before CreateWindow returns, and thus before m_hwnd is valid
	return g_game->MsgProc(hwnd,msg,wparam,lparam);
}

#pragma region Game Implementation
namespace Engine
{

	// constructor
	// sets the global game object and initializes member variables
	Game::Game(void)
	{
		g_game = this;
		m_graphics = 0;
		m_screenManager = 0;
		m_isActive = false;
	}

	Game::~Game(void)
	{
		Cleanup();
	}

	bool Game::Init(HINSTANCE hInst, int width, int height, bool windowed)
	{
#pragma region Windows Stuff
		/***********************************************\
		|	Initialize Windows							|
		\***********************************************/
		// create a new window class structure
		WNDCLASS wc;

		// set all members to zero
		ZeroMemory(&wc,sizeof(WNDCLASS));
		// set windows class options
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)MainWndProc;
		wc.hInstance = hInst;
		wc.lpszClassName = "D3DWND";

		// register class
		RegisterClass(&wc);
		// create the window
		m_hwnd = CreateWindow("D3DWND","GameScreen Manager", WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP , 0,0,width,height,0,0,hInst,0);
		if(!m_hwnd)
			return false;
		// show the window
		ShowWindow(m_hwnd,SW_SHOW);
		// update the window
		UpdateWindow(m_hwnd);
#pragma endregion

		// create and initalize the m_graphics object
		m_graphics = new Graphics();

		if(!m_graphics->Init(m_hwnd,width,height,windowed))
			return false;

		// create the screenmanager object
		m_screenManager = new ScreenManager(m_graphics);
		

		// add the background and mainmenu screens
		m_screenManager->AddScreen( new BackgroundScreen(),PlayerIndex::Null);
		m_screenManager->AddScreen( new MainMenuScreen(),PlayerIndex::Null);
		// initialize the screenmanager
		m_screenManager->Load();
		ScreenManager::LuaBindings(m_screenManager->GetScript()->GetState());
		return true;
	}

	void Game::Cleanup()
	{
		// clean up the screen manager
		// and release the memory
		if(m_screenManager)
		{
			m_screenManager->Unload();
			delete m_screenManager;
			m_screenManager = 0;
		}
		// clean up the m_graphics device
		// and release the memory
		if(m_graphics)
		{
			m_graphics->Release();
			m_graphics = 0;
		}
	}

	void Game::Update(float deltaTime)
	{
		// quit screen manager?
		if(m_screenManager->Quit())
			Quit();
		// clean up dead screens
		m_screenManager->CleanUpScreens();
		// update screen manager
		m_screenManager->Update(deltaTime);
	}

	void Game::Draw(float deltaTime)
	{
		// Setup the color to clear the buffer to.
		float color[] = { 0.0f,0.0f,0.0f,1.0f };
		// Clear the back buffer.
		m_graphics->Clear(color);
		// let screen manager draw screens
		m_screenManager->Draw(deltaTime);
		// flip backbuffer and front buffer
		m_graphics->Present();
	}
	// quit Game
	void Game::Quit()
	{
		DestroyWindow(m_hwnd);
		PostQuitMessage(0);
	}
	// message pump
	LRESULT Game::MsgProc(HWND hwnd,UINT msg, WPARAM wparam,LPARAM lparam)
	{
		switch(msg)
		{
		case WM_ACTIVATE:
			{
				// checks if the app is being used at the moment
				if (LOWORD(wparam) == WA_ACTIVE )
					m_isActive = true;
				else
					m_isActive = false;
			}
			break;
		case WM_DESTROY:
			DestroyWindow(m_hwnd);
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hwnd,msg,wparam,lparam);
	} 
}
#pragma endregion

#pragma region Entry Point
// main entry point of the application
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev,LPSTR cmdLine,int showCmd)
{

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// create the application object
	Engine::Game game;

	// Initialize the application object
	if(!game.Init(hInst,800,600,true))
		return 0;

	// used for windows messages
	MSG msg; 
	memset(&msg,0,sizeof(MSG));

	// set the start time
	int startTime = timeGetTime();

	// start message loop
	while(msg.message != WM_QUIT)
	{
		// check for any messages and handle
		// them
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // otherwise, run game
		{
			
			// delta time between frames in milliseconds
			int t = timeGetTime();
			float deltaTime = (t - startTime) * 0.001f;
			
			// update application
			game.Update(deltaTime);
			
			// render application
			game.Draw(deltaTime);
			// copy the old time
			startTime = t;
			
		}
	}

	// release all resources
	game.Cleanup();
	g_game = 0;

	return msg.wParam;
}


#pragma endregion





