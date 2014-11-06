#include "LoadingScreen.h"
#include "ScreenManager.h"
#include "Debug.h"
#include "Graphics.h"
using namespace DirectX;
namespace Engine
{

	LoadingScreen::LoadingScreen(Engine::ScreenManager* m,bool loadingIsSlow,std::vector<GameScreen*> screensToLoad) : m_OtherScreensAreGone(false)
	{
		debug.Print("LoadingScreen Loaded!");
		this->m_LoadingIsSlow = loadingIsSlow;
		this->m_ScreensToLoad = screensToLoad;

		m_TransitionOnTime = 0.7f;
	}

	LoadingScreen::~LoadingScreen(void)
	{
		debug.Print("LoadingScreen deleted");
	}

	void LoadingScreen::Load(Engine::ScreenManager* screenManager,bool loadingIsSlow,PlayerIndex& playerIndex,std::vector<GameScreen*> screensToLoad)
	{
		// tell each current screens to transition off
		std::vector<GameScreen*> screens = screenManager->GetScreens();
		for (std::vector<GameScreen*>::iterator screen = screens.begin(); screen != screens.end(); screen++)
			(*screen)->ExitScreen();

		// create and activate the loading screen
		LoadingScreen* loadingScreen = new LoadingScreen(screenManager,loadingIsSlow,screensToLoad);

		screenManager->AddScreen(loadingScreen,playerIndex);
	}

	void LoadingScreen::Update(float deltaTime,bool otherScreenHasFocus,bool coveredByOtherScreen)
	{
		GameScreen::Update(deltaTime,otherScreenHasFocus,coveredByOtherScreen);

		// if all the previous screens have finished transitioning
		// off, it is time to actually perform the load.
		if (m_OtherScreensAreGone)
		{
			for (std::vector<GameScreen*>::iterator screen = m_ScreensToLoad.begin(); screen != m_ScreensToLoad.end(); screen++)
			{
				if((*screen) != nullptr)
					m_ScreenManager->AddScreen((*screen),m_controllingPlayer);
			}
			m_ScreenManager->RemoveScreen(this);
		}
	}

	void LoadingScreen::Draw(float deltaTime)
	{
		// If we are the only active screen, that means all the previous screens
		// must have finished transitioning off. We check for this in the Draw
		// method, rather than in Update, because it isn't enough just for the
		// screens to be gone: in order for the transition to look good we must
		// have actually drawn a frame without them before we perform the load.
		if((m_ScreenState == ScreenState::Active) &&
			(m_ScreenManager->GetScreens().size() == 1))
		{
			m_OtherScreensAreGone = true;
		}
		// The gameplay screen takes a while to load, so we display a loading
		// message while that is going on, but the menus load very quickly, and
		// it would look silly if we flashed this up for just a fraction of a
		// second while returning from the game to the menus. This parameter
		// tells us how long the loading is going to take, so we know whether
		// to bother drawing the message.
		if (m_LoadingIsSlow)
		{
			debug.Print("Draw LoadingScreen...slow");
			SpriteBatch* spriteBatch = m_ScreenManager->SpriteBatch();
			//		ID3DXFont* font = m_ScreenManager->Font(FONT24);
			SpriteFont* font = m_ScreenManager->Font();
			std::wstring message = L"Loading...";

			// center the text in the viewport
			//D3DVIEWPORT9 viewport;
			//m_ScreenManager->Device()->GetViewport(&viewport);
			D3D11_VIEWPORT viewport;
			UINT count = 1;
			m_ScreenManager->GetGraphicsDevice()->GetImmediateContex()->RSGetViewports(&count,&viewport);

			XMFLOAT2 point = XMFLOAT2( font->MeasureString(message.c_str()).m128_f32[0],font->GetLineSpacing());
			XMFLOAT2 position =XMFLOAT2((viewport.Width - point.x) / 2, (viewport.Height - point.y)/2);
			float transitionAlpha = TransitionAlpha();
			XMFLOAT4 color = XMFLOAT4(0xFF * transitionAlpha,0xFF * transitionAlpha,
				0xFF * transitionAlpha,0xFF * transitionAlpha);

			spriteBatch->Begin();
			//font->DrawText(spriteBatch,message,-1,&r,DT_NOCLIP,color);
			//font->Draw(spriteBatch,position.x,position.y,message,1.0f,color);
			font->DrawString(spriteBatch,message.c_str(),position,XMLoadFloat4(&color));
			spriteBatch->End();
		}
	}

}
