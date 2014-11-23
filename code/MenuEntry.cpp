#include "MenuEntry.h"
#include "MenuScreen.h"
#include "ScreenManager.h"
#include "MathHelper.h"
#include "Debug.h"
using namespace DirectX;
namespace Engine
{

	MenuEntry::MenuEntry(std::string text)
	{
		Selected = nullptr;
		this->m_Text = text;
		std::string debugText = "MenuEntry - " + m_Text + " Loaded";
		debug.Print((char*)debugText.c_str());
	}



	MenuEntry::~MenuEntry(void)
	{

	}

	void MenuEntry::OnSelectedEntry(PlayerIndex playerIndex)
	{
		if (Selected != nullptr)
			Selected(this,playerIndex);
	}

	void MenuEntry::Update(MenuScreen* screen,bool isSelected,float deltaTime)
	{
		// When the menu selection changes, entries gradually fade between
		// their selected and deselected appearance, rather than instantly
		// popping to the new state.
		float fadeSpeed = timeGetTime() / 1000 * 4.0f;

		if (isSelected)
			m_SelectionFade = MathHelper::Min(m_SelectionFade + fadeSpeed,1.0f);
		else
			m_SelectionFade =  MathHelper::Max(m_SelectionFade - fadeSpeed, 0.0f);
	}

	void MenuEntry::Draw(MenuScreen* screen,bool isSelected,float deltaTime)
	{

		// pulsate the size of the selected menu entry
		double time = (timeGetTime() / 1000.0f);

		float pulsate = sinf(time * 6) + 1.0f;

		// Modify the alpha to fade m_Text out during transitions
		float transitionAlpha = screen->TransitionAlpha();
		// draw the selected entry in yellow, otherwise white	
		float b = isSelected ? 0.0f : 1.0f;
		XMVECTOR color = { 1.0f * transitionAlpha,1.0f * transitionAlpha,
			b * transitionAlpha,1.0f * transitionAlpha };

		// draw the m_Text
		ScreenManager* screenManager = screen->ScreenManager();
		SpriteBatch* spriteBatch = screenManager->SpriteBatch();
		SpriteFont* font = screenManager->Font();

		float scale = 1 + pulsate * 0.05f * m_SelectionFade;

		// adjust x position
		std::wstring s(m_Text.begin(),m_Text.end());
		m_Position.x = (m_Position.x - (font->MeasureString(s.c_str()).m128_f32[0] ) / 2);
		XMFLOAT2 origin = XMFLOAT2((font->MeasureString(s.c_str()).m128_f32[0] ) / 3, font->GetLineSpacing() / 2);
		font->DrawString(spriteBatch,s.c_str(),m_Position,color,0.0f,origin,scale);
	}

	int MenuEntry::GetHeight(MenuScreen* screen)
	{
		return (int)screen->ScreenManager()->Font()->GetLineSpacing();
	}

	int MenuEntry::GetWidth(MenuScreen* screen)
	{
		std::wstring s(m_Text.begin(),m_Text.end());
		return screen->ScreenManager()->Font()->MeasureString(s.c_str()).m128_f32[0] / 2.0f;
	}

}