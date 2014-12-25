#include "BackgroundScreen.h"
#include "ScreenManager.h"
#include "Debug.h"
#include <DDSTextureLoader.h>
#include "Graphics.h"
using namespace DirectX;
namespace Engine
{

	BackgroundScreen::BackgroundScreen(void) : GameScreen(), m_BackgroundTexture(0)
	{
		m_TransitionOnTime =  0.5; // TimeSpan.FromSeconds(0.5);
		m_TransitionOffTime = 0.5; // TimeSpan.FromSeconds(0.5);
		debug.Print("BackgroundScreen Loaded!");

	}


	BackgroundScreen::~BackgroundScreen(void)
	{
		UnLoad();
	}

	void BackgroundScreen::Load()
	{
		ID3D11Device* device = m_ScreenManager->GetGraphicsDevice()->GetDevice();
		ID3D11DeviceContext* deviceContext = m_ScreenManager->GetGraphicsDevice()->GetImmediateContex();

		DirectX::CreateDDSTextureFromFile(device,deviceContext,L"Content/Textures/background.dds",
			0,&m_BackgroundTexture);
	}

	void BackgroundScreen::UnLoad()
	{
		if(m_BackgroundTexture)
		{
			m_BackgroundTexture->Release();
			m_BackgroundTexture = 0;
		}
	}

	void BackgroundScreen::Update(float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
	{
		GameScreen::Update(deltaTime,otherScreenHasFocus,false);
	}

	void BackgroundScreen::Draw(float deltaTime)
	{
		DirectX::SpriteBatch* spriteBatch = m_ScreenManager->SpriteBatch();
		D3D11_VIEWPORT viewport;
		UINT count = 1;
		m_ScreenManager->GetGraphicsDevice()->GetImmediateContex()->RSGetViewports(&count,&viewport);

		RECT fullscreen = {0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height)};

		spriteBatch->Begin();
		float colorValue = TransitionAlpha();
		SimpleMath::Color color(colorValue, colorValue, colorValue,1.0f);

		spriteBatch->Draw(m_BackgroundTexture, fullscreen,0, color);
		spriteBatch->End();
	}

}