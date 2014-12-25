#pragma once
#include "gamescreen.h"
#include "MenuEntry.h"
#include <vector>

namespace Engine
{
	class MenuScreen :
		public GameScreen
	{
#pragma region Fields
	protected:
		std::vector<MenuEntry*> m_MenuEntries;
		int m_SelectedEntry;
		std::string m_MenuTitle;
#pragma endregion

#pragma region Properties
		std::vector<MenuEntry*> MenuEntries() { return m_MenuEntries; }
#pragma endregion

#pragma region Initialization
	public:
		MenuScreen(std::string m_MenuTitle);
		~MenuScreen(void);
#pragma endregion

#pragma region Handle Input
		/// <summary>
		/// Responds to user input, changing the selected entry and accepting
		/// or cancelling the menu.
		/// </summary>
		void HandleInput(InputState* input);
	protected:
		/// <summary>
		/// Handler for when the user has chosen a menu entry.
		/// </summary>
		virtual void OnSelectedEntry(int entryIndex,int playerIndex);
		virtual void OnCancel(void* sender,int playerIndex);

#pragma endregion

#pragma region Update and Draw
	protected:
		/// <summary>
		/// Allows the screen the chance to position the menu entries. By default
		/// all menu entries are lined up in a vertical list, centered on the screen.
		/// </summary>
		virtual void UpdateMenuEntryLocations();
	public:
		/// <summary>
		/// Updates the menu.
		/// </summary>
		void Update(float deltaTime, bool otherScreenHasFocus,bool coveredByOtherScreen) override;
		/// <summary>
		/// Draws the menu.
		/// </summary>
		void Draw(float deltaTime) override;
#pragma endregion
	};



}