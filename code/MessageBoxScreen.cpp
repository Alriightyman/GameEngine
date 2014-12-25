#include "MessageBoxScreen.h"
#include "ScreenManager.h"
#include "Debug.h"
#include <DDSTextureLoader.h>
#include "Graphics.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace Engine
{

	MessageBoxScreen::MessageBoxScreen(std::string message,bool includeUsageText)
	{
		debug.Print("MessageBoxScreen Loaded");
		Accepted = nullptr;
		Cancelled = nullptr;

		const std::string usageText = "\nA button, Space, Enter = Ok\nB button, Esc = Cancel";

		if(includeUsageText)
			m_Message = message + usageText;
		else
			m_Message = message;

		m_IsPopup = true;

		m_TransitionOnTime = 0.2f;
		m_TransitionOffTime = 0.2f;
	}


	MessageBoxScreen::~MessageBoxScreen(void)
	{
		if(m_GradientTexture)
		{
			m_GradientTexture->Release();
			m_GradientTexture = 0;
		}
	}

	void MessageBoxScreen::Load()
	{
		CreateDDSTextureFromFile(m_ScreenManager->GetGraphicsDevice()->GetDevice(),m_ScreenManager->GetGraphicsDevice()->GetImmediateContex(),L"Content/Textures/gradient.dds",0,&m_GradientTexture);
	}

	void MessageBoxScreen::HandleInput(InputState* input)
	{
		int index;

		if(input->IsMenuSelect(m_controllingPlayer))
		{
			if(Accepted != nullptr)
				Accepted(this,index);

			ExitScreen();
		}
		else if(input->IsMenuCancel(m_controllingPlayer))
		{
			if(Cancelled != nullptr)
				Cancelled(this,index);

			ExitScreen();
		}
	}

	void MessageBoxScreen::Draw(float deltaTime)
	{

		SpriteBatch* spriteBatch = m_ScreenManager->SpriteBatch();
		SpriteFont* font = m_ScreenManager->Font();

		m_ScreenManager->FadeBackBufferToBlack(TransitionAlpha() * 2 / 3);

		D3D11_VIEWPORT viewport;
		UINT count = 1;
		m_ScreenManager->GetGraphicsDevice()->GetImmediateContex()->RSGetViewports(&count,&viewport);

		Vector2 viewportSize(viewport.Width,viewport.Height);

		std::wstring s(m_Message.begin(),m_Message.end());
		Vector2 textSize = font->MeasureString(s.c_str());
		Vector2 textPosition = (viewportSize - textSize) / 2;

		const int hPad = 32;
		const int vPad = 16;

		RECT backgroundRectangle = {
			(LONG)textPosition.x - hPad, // left
			(LONG)textPosition.y - vPad, // top 
			(LONG)(textSize.x + textPosition.x )+ hPad * 2, // right 
			(LONG)(textSize.y + textPosition.y )+ vPad * 2}; // bottom

		Color color = DirectX::Colors::White * TransitionAlpha();

		spriteBatch->Begin();

		spriteBatch->Draw(m_GradientTexture,backgroundRectangle,NULL,color,0.0f,Vector2::Zero);
		font->DrawString(spriteBatch,s.c_str(),textPosition,color);
		spriteBatch->End();
	} 
}