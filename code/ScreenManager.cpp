#include "ScreenManager.h"
#include "Debug.h"
#include <DDSTextureLoader.h>
#include "Graphics.h"

using namespace DirectX;

namespace Engine
{

	typedef std::vector<GameScreen*>::iterator screenIter;
	
	bool ScreenManager::isInitialized = false;

	ScreenManager::ScreenManager(Engine::Graphics* graphics) :
		font(0),spriteBatch(0),quit(false),traceEnabled(false),
		m_graphics(graphics),m_script(0),input(0)
	{
		
	}


	ScreenManager::~ScreenManager(void)
	{
		screensToUpdate.clear();

		while(!screens.empty())
		{
			GameScreen* screen = screens.back();
			screens.pop_back();
			if(screen)
			{
				screen->UnLoad();
				delete screen;
				screen = 0;
			}
		}

		while(!screens.empty())
		{
			GameScreen* screen = screens.back();
			screens.pop_back();
			if(screen)
			{
				screen->UnLoad();
				delete screen;
				screen = 0;
			}
		}

		if (font)
		{
			delete font;
		}
		if(m_script)
		{
			delete m_script;
		}

		if(spriteBatch)
		{
			delete spriteBatch;
			spriteBatch = 0;
		}

		if(blankTexture)
		{
			blankTexture->Release();
			blankTexture = 0;
		}

		if (input)
		{
			delete input;
			input = 0;
		}


	}


	void ScreenManager::Load()
	{

		isInitialized = true;

		spriteBatch = new DirectX::SpriteBatch(m_graphics->GetImmediateContex());

		font = new SpriteFont(m_graphics->GetDevice(),L"Content/Font/menuFont.spritefont");


		D3D11_VIEWPORT viewport;
		unsigned int numViews = 1;
		m_graphics->GetImmediateContex()->RSGetViewports(&numViews,&viewport);

		CreateDDSTextureFromFile(m_graphics->GetDevice(),L"Content/Textures/blank.dds",0,&blankTexture);

		for(screenIter screen = screens.begin(); screen != screens.end(); screen++)
		{
			((GameScreen*)(*screen))->Load();
		}

		//
		DXGI_SWAP_CHAIN_DESC sd;
		m_graphics->GetSwapChain()->GetDesc(&sd);
		HWND hwnd = sd.OutputWindow;
		input = new InputState(hwnd);
		
		// initialize lua
		m_script = new Script();
		InputState::Bind(m_script->GetState());
	}

	void ScreenManager::Unload()
	{
		for (screenIter screen = screens.begin(); screen != screens.end(); screen++)
			((GameScreen*)(*screen))->UnLoad();
	}

	void ScreenManager::Update(float deltaTime)
	{
		input->Update();
		// make a copy of the master screen list, to avoid confusion if
		// the process of updating one screen adds or removes others
		screensToUpdate.clear();

		for(screenIter screen = screens.begin(); screen != screens.end(); screen++)
			screensToUpdate.push_back(*screen);

		HWND window = GetForegroundWindow();

		DXGI_SWAP_CHAIN_DESC sd;
		m_graphics->GetSwapChain()->GetDesc(&sd);
		HWND thisWindow = sd.OutputWindow;

		bool otherScreenHasFocus = true;
		if(window == thisWindow)
			otherScreenHasFocus = false;

		bool coveredByOtherScreen = false;

		// Loop as long as there are screens waiting to be updated
		while(!screensToUpdate.empty())
		{
			// pop the topmost screen off the waiting list
			GameScreen* screen = screensToUpdate.back();

			screensToUpdate.pop_back();

			// update the screen
			screen->Update(deltaTime,otherScreenHasFocus,coveredByOtherScreen);

			if(screen->ScreenState() == ScreenState::TransitionOn ||
				screen->ScreenState() == ScreenState::Active)
			{
				// if this is the first active screen we came across,
				// give it a chance to handle input.
				if (!otherScreenHasFocus)
				{
					screen->HandleInput(input);

					otherScreenHasFocus = true;
				}

				// If this is an active non-popup, inform any subsequent
				// screens that they are covered by it.
				if(!screen->IsPopup())
					coveredByOtherScreen = true;
			}
		}

		// Print debug trace?
		if(traceEnabled)
			TraceScreens();
	}

	void ScreenManager::TraceScreens()
	{
		for(screenIter screen = screens.begin(); screen != screens.end(); screen++)
			debug.Print((char*)((GameScreen*)(*screen))->ToString().c_str());
	}

	/// <summary>
	/// Tells each screen to draw itself.
	/// </summary>
	void ScreenManager::Draw(float deltaTime)
	{
		for(std::vector<GameScreen*>::iterator screen = screens.begin(); screen != screens.end(); screen++)
		{
			if(((GameScreen*)(*screen))->ScreenState() == ScreenState::Hidden)
				continue;

			// draw screen
			((GameScreen*)(*screen))->Draw(deltaTime);

		}
	}

	void ScreenManager::AddScreen(GameScreen* screen, PlayerIndex playerIndex)
	{
		screen->ControllingPlayer(playerIndex);
		screen->ScreenManager(this);
		screen->IsExiting(false);

		// If we have a graphics device, tell the screen to load
		if(isInitialized)
		{
			screen->Load();
		}

		screens.push_back(screen);
	}

	/// <summary>
	/// Removes a screen from the screen manager. You should normally
	/// use GameScreen.ExitScreen instead of calling this directly, so
	/// the screen can gradually transition off rather than just being
	/// instantly removed.
	/// </summary>

	void ScreenManager::RemoveScreen(GameScreen* screen)
	{
		// if we have a graphics device, tell the screen to unload
		if (isInitialized)
		{
			screen->UnLoad();
		}

		screens.erase(std::remove(screens.begin(),screens.end(),screen),screens.end());
		//	screens.erase(std::remove(screens.begin(),screens.end(),screen),screens.end());

		//delete screen;
		screensToDelete.push_back(screen);
	}

	std::vector<GameScreen*> ScreenManager:: GetScreens()
	{
		return screens;
	}

	void ScreenManager::FadeBackBufferToBlack(float alpha)
	{
		D3D11_VIEWPORT viewport;
		UINT viewCount = 1;
		m_graphics->GetImmediateContex()->RSGetViewports(&viewCount,&viewport);

		spriteBatch->Begin();
		RECT r = { 0,0,viewport.Width,viewport.Height };

		XMVECTORF32 color = { 0.0f,0.0f,0.0f,alpha };
		spriteBatch->Draw(blankTexture,r,0,color);

		spriteBatch->End();
	}

	void ScreenManager::CleanUpScreens()
	{
		while(!screensToDelete.empty())
		{
			GameScreen* s = screensToDelete.back();
			screensToDelete.pop_back();
			delete s;
			s = 0;
		}
	}

	// binds data types of directXtoolkit
	void ScreenManager::LuaBindings(luabridge::lua_State* L)
	{
		using namespace luabridge;
		using namespace DirectX::SimpleMath;
		// Vector3
		getGlobalNamespace(L)
			.beginClass<Vector3>("Vector3")
				.addConstructor<void(*)(void)>()
				.addConstructor<void(*)(float x,float y,float z)>()
				.addData<float>("x",&Vector3::x)
				.addData<float>("y",&Vector3::y)
				.addData<float>("z",&Vector3::z)
			.endClass();

		// Vector2
		getGlobalNamespace(L)
			.beginClass<Vector2>("Vector2")
				.addConstructor<void(*)(void)>()
				.addConstructor<void(*)(float x,float y)>()
				.addData<float>("x",&Vector2::x)
				.addData<float>("y",&Vector2::y)
			.endClass();
		// Quat
		getGlobalNamespace(L)
			.beginClass<Quaternion>("Quaternion")
				.addConstructor<void(*)(void)>()
				.addConstructor<void(*)(float x,float y,float z, float w)>()
				.addConstructor<void(*)(const Vector3& v, float scalar)>()
				.addData<float>("x",&Quaternion::x)
				.addData<float>("y",&Quaternion::y)
			.endClass();
		// color
		getGlobalNamespace(L)
			.beginClass<Color>("Color")
				.addConstructor<void(*)(void)>()
				.addConstructor<void(*)(float r,float g,float b, float a)>()
				.addConstructor<void(*)(float r,float g,float b)>()
				.addProperty("r",&Color::R,&Color::R)
				.addProperty("g",&Color::G,&Color::G)
				.addProperty("b",&Color::B,&Color::B)
				.addProperty("a",&Color::A,&Color::A)
			.endClass();
	}
}