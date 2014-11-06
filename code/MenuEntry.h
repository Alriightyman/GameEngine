#pragma once
#include <string>
#include <d3d11.h>
#include <functional>
#include <vector>
#include <SimpleMath.h>
#include "PlayerIndex.h"
using namespace DirectX::SimpleMath;


namespace Engine
{

	class MenuScreen;

	class MenuEntry
	{
	protected:
		/// <summary>
		/// The m_Text rendered for this entry.
		/// </summary>
		std::string m_Text;
		/// <summary>
		/// Tracks a fading selection effect on the entry.
		/// </summary>
		/// <remarks>
		/// The entries transition out of the selection effect when they are deselected.
		/// </remarks>
		float m_SelectionFade;
		/// <summary>
		/// The m_Position at which the entry is drawn. This is set by the MenuScreen
		/// each frame in Update.
		/// </summary>
		Vector2 m_Position;
	public:
		std::string Text() { return m_Text; }
		void Text(std::string value) { m_Text = value; }

		Vector2 Position() { return m_Position; }
		void Position(Vector2 value) { m_Position = value; }
#pragma region Events
	public:
		/// <summary>
		/// Event raised when the menu entry is selected.
		/// </summary>
		std::function<void (void*,PlayerIndex)> Selected;
		//	void (*Selected)(void*);
		/// <summary>
		/// Method for raising the Selected event.
		/// </summary>
		virtual void OnSelectedEntry(PlayerIndex playerIndex);
#pragma endregion

	public:
		MenuEntry(std::string m_Text);
		~MenuEntry(void);

		virtual void Update(MenuScreen* screen, bool isSelected, float deltaTime);
		virtual void Draw(MenuScreen* screen, bool isSelected, float deltaTime);
		virtual int GetHeight(MenuScreen* screen);
		virtual int GetWidth(MenuScreen* screen);

	};


}