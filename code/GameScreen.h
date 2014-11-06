#pragma once
#include "InputState.h"
#include <string>

namespace Engine
{
	class ScreenManager;


	enum ScreenState
	{
		TransitionOn,
		Active,
		TransitionOff,
		Hidden
	};

	class GameScreen
	{
	protected:
		std::string m_Name;
		bool m_IsPopup;
		float m_TransitionOnTime;
		float m_TransitionOffTime;
		float m_TransitionPosition;
		ScreenState m_ScreenState;
		bool m_IsExiting;
		bool m_DeleteScreen;
		bool m_OtherScreenHasFocus;
		ScreenManager* m_ScreenManager;
		PlayerIndex m_controllingPlayer;

	public:
		bool IsPopup() { return m_IsPopup; }
		float TransitionOnTime() { return m_TransitionOnTime; }
		float TransitionOffTime() { return m_TransitionOffTime; }
		float TransitionPosition() { return m_TransitionPosition; }
		float TransitionAlpha() { return 1.0f - m_TransitionPosition; }
		ScreenState ScreenState() { return m_ScreenState; }
		bool IsExiting() { return m_IsExiting; }
		void IsExiting(bool value) { m_IsExiting = value; }

		bool IsActive() { return !m_OtherScreenHasFocus &&
			(m_ScreenState == ScreenState::TransitionOn ||
			m_ScreenState == ScreenState::Active); }

		ScreenManager* ScreenManager() { return m_ScreenManager; }
		void ScreenManager(class ScreenManager* value) { m_ScreenManager = value; }

		std::string ToString() { return m_Name; }
		bool DeleteScreen() { return m_DeleteScreen;}
		PlayerIndex ContollingPlayer() { return m_controllingPlayer; }
		void ControllingPlayer(PlayerIndex value) { m_controllingPlayer = value; }
	public:
		GameScreen(void);
		virtual ~GameScreen(void);
		virtual void Load();
		virtual void UnLoad();

		virtual void Update(float deltaTime, bool OtherScreenHasFocus, bool coveredByOtherScreen);
		virtual void HandleInput(InputState* input);
		virtual void Draw(float deltaTime);
		void ExitScreen();
	private:
		bool UpdateTransition(float deltaTime, float time, int direction);


	};


}