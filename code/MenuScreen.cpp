#include "MenuScreen.h"
#include "ScreenManager.h"
#include "Debug.h"
#include "MathHelper.h"
#include "Graphics.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace Engine
{

	MenuScreen::MenuScreen(std::string m_MenuTitle) : m_SelectedEntry(0)
	{
		this->m_MenuTitle = m_MenuTitle;
		m_TransitionOffTime = 0.5f;
		m_TransitionOnTime = 0.5f;
	}


	MenuScreen::~MenuScreen(void)
	{
		while(!m_MenuEntries.empty())
		{
			MenuEntry* entry = m_MenuEntries.back();
			m_MenuEntries.pop_back();
			delete entry;
			entry = 0;
		}
	}

	void MenuScreen::HandleInput(InputState* input)
	{
		if (input->IsMenuUp(m_controllingPlayer))
		{
			m_SelectedEntry--;
			if (m_SelectedEntry < 0)
				m_SelectedEntry = m_MenuEntries.size() - 1;
		}

		if (input->IsMenuDown(m_controllingPlayer))
		{
			m_SelectedEntry++;
			if (m_SelectedEntry >(int) m_MenuEntries.size()-1)
				m_SelectedEntry = 0;
		}
		// Accept or cancel the menu? We pass in our ControllingPlayer, which may
		// either be null (to accept input from any player) or a specific index.
		// If we pass a null controlling player, the InputState helper returns to
		// us which player actually provided the input. We pass that through to
		// OnSelectEntry and OnCancel, so they can tell which player triggered them.
		int playerIndex = PlayerIndex::Null;

		if (input->IsMenuSelect(m_controllingPlayer))
		{
			OnSelectedEntry(m_SelectedEntry,playerIndex);
		}
		else if(input->IsMenuCancel(m_controllingPlayer))
		{
			OnCancel(this,playerIndex);
		}
	}

	void MenuScreen::OnSelectedEntry(int entryIndex,int playerIndex)
	{
		std::vector<MenuEntry*>::iterator it = m_MenuEntries.begin();
		std::advance(it,entryIndex);
		(*it)->OnSelectedEntry(playerIndex);
	}


	void MenuScreen::OnCancel(void* sender,int playerIndex)
	{
		ExitScreen();
	}

	void MenuScreen::UpdateMenuEntryLocations()
	{
		// Make the menu slide into place during transitions, using a
		// power curve to make things look more interesting (this makes
		// the movement slow down as it nears the end).
		float transitionOffset = pow(m_TransitionPosition,2);

		// start at Y = 175; each X value is generated per entry
		Vector2 position = Vector2(0.0f,175.0f);

		// update each menu entry's location in turn
		for (std::vector<MenuEntry*>::iterator i = m_MenuEntries.begin(); i != m_MenuEntries.end(); i++)
		{
			MenuEntry* menuEntry = *i;

			// each entry is to be centered horizontally
			D3D11_VIEWPORT viewport;
			UINT count = 1;
			m_ScreenManager->GetGraphicsDevice()->GetImmediateContex()->RSGetViewports(&count,&viewport);
			position.x = (float)(viewport.Width / 2 + menuEntry->GetWidth(this) / 2);

			if(m_ScreenState == ScreenState::TransitionOn)
				position.x -= transitionOffset * 256;
			else 
				position.x += transitionOffset * 512;

			// set the entry's position
			menuEntry->SetPosition(position);

			// move down for the next entry the size of this entry
			position.y += menuEntry->GetHeight(this);
		}

	}

	void MenuScreen::Update(float deltaTime, bool otherScreenHasFocus,bool coveredByOtherScreen)
	{
		// update each nested MenuEntry object
		for (std::vector<MenuEntry*>::iterator it = m_MenuEntries.begin(); it != m_MenuEntries.end(); it++)
		{
			int i = std::distance(m_MenuEntries.begin(),it);
			bool isSelected = IsActive() && ( i == m_SelectedEntry);

			((MenuEntry*)(*it))->Update(this,isSelected,deltaTime);
		}

		GameScreen::Update(deltaTime,otherScreenHasFocus,coveredByOtherScreen);
	}

	void MenuScreen::Draw(float deltaTime)
	{
		// make sure out entries are in the right place before we draw them
		UpdateMenuEntryLocations();

		ID3D11Device* device = m_ScreenManager->GetGraphicsDevice()->GetDevice();
		SpriteBatch* spriteBatch = m_ScreenManager->SpriteBatch();
		SpriteFont* font = m_ScreenManager->Font();

		spriteBatch->Begin();

		// draw each menu entry in turn
		for(std::vector<MenuEntry*>::iterator it = m_MenuEntries.begin(); it != m_MenuEntries.end(); it++)
		{
			int i = std::distance(m_MenuEntries.begin(),it);
			MenuEntry* entry = *it;
			bool isSelected = IsActive() && (i == m_SelectedEntry);
			entry->Draw(this,isSelected,deltaTime);
		}

		// Make the menu slide into place during transitions, using a
		// power curve to make things look more interesting (this makes
		// the movement slow down as it nears the end).
		float transitionOffset = pow(m_TransitionPosition,2);

		// Draw the menu title centered on the screen
		D3D11_VIEWPORT viewport;
		UINT count = 1;
		m_ScreenManager->GetGraphicsDevice()->GetImmediateContex()->RSGetViewports(&count,&viewport);
		std::wstring s(m_MenuTitle.begin(),m_MenuTitle.end());
		XMFLOAT2 titlePosition = XMFLOAT2((float)(viewport.Width / 2) - (font->MeasureString(s.c_str()).m128_f32[0] / 2), 80);
		XMFLOAT2 origin = XMFLOAT2(font->MeasureString(s.c_str()).m128_f32[0] / 3, font->GetLineSpacing() / 2);

		float transitionAplpha = TransitionAlpha();
		XMVECTOR titleColor = {0.75f * transitionAplpha,0.75f * transitionAplpha,0.75f * transitionAplpha,1.0f * transitionAplpha};
		float titleScale = 1.25f;

		titlePosition.y -= transitionOffset * 100;
		font->DrawString(spriteBatch,s.c_str(),titlePosition,titleColor,0.0f,origin,titleScale);
		spriteBatch->End();
	}



}