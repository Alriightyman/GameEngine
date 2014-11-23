#include "GameScreen.h"
#include "ScreenManager.h"
#include "MathHelper.h"
namespace Engine
{

	GameScreen::GameScreen() : m_IsPopup(false), m_TransitionOnTime(0.0f),
		m_TransitionOffTime(0.0f),m_TransitionPosition(1),m_ScreenState(ScreenState::TransitionOn),
		m_IsExiting(false),m_ScreenManager(0),m_DeleteScreen(false),m_controllingPlayer(PlayerIndex::Null)
	{
	}


	GameScreen::~GameScreen(void)
	{

	}

	void GameScreen::Load(){}

	void GameScreen::UnLoad(){}

	/// <summary>
	/// Allows the screen to run logic, such as updating the transition position.
	/// Unlike HandleInput, this method is called regardless of whether the screen
	/// is active, hidden, or in the middle of a transition.
	/// </summary>
	void GameScreen::Update(float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
	{
		m_OtherScreenHasFocus = otherScreenHasFocus;

		if (m_IsExiting)
		{
			// If the screen is going away to die, it should transition off.
			m_ScreenState = ScreenState::TransitionOff;

			if (!UpdateTransition(deltaTime, m_TransitionOffTime, 1))
			{
				// When the transition finishes, remove the screen.
				m_ScreenManager->RemoveScreen(this);
			}
		}
		else if (coveredByOtherScreen)
		{ 
			// If the screen is covered by another, it should transition off.
			if(UpdateTransition(deltaTime,m_TransitionOffTime,1))
			{
				// Still busy transitioning.
				m_ScreenState = ScreenState::TransitionOff;
			}
			else
			{
				// Transition Finished!
				m_ScreenState = ScreenState::Hidden;
			}
		}
		else
		{
			// Otherwise, the screen should transition on and become active.
			if (UpdateTransition(deltaTime, m_TransitionOnTime, -1))
			{
				// Still busy transitioning.
				m_ScreenState = ScreenState::TransitionOn;
			}
			else
			{
				// Transition Finished!
				m_ScreenState = ScreenState::Active;
			}
		}
	}

	/// <summary>
	/// Helper for updating the screen transition position.
	/// </summary>
	bool GameScreen::UpdateTransition(float deltaTime, float time, int direction)
	{
		// How much should we move by?
		float transitionDelta;
		if(time == 0.0f)
			transitionDelta = 1;
		else 
			transitionDelta = (deltaTime ) / time;

		// Update the transition position.
		m_TransitionPosition += transitionDelta * direction;

		// Did we reach the end of the transition?
		if(((direction < 0) && (m_TransitionPosition <= 0)) ||
			((direction > 0) && (m_TransitionPosition >= 1)))
		{
			m_TransitionPosition = MathHelper::Clamp(m_TransitionPosition,0.0f,1.0f);
			return false;
		}

		// otherwise, we are still busy transitioning.
		return true;
	}

	/// <summary>
	/// Allows the screen to handle user input. Unlike Update, this method
	/// is only called when the screen is active, and not when some other
	/// screen has taken the focus.
	/// </summary>
	void GameScreen::HandleInput(InputState* input)
	{
	}
	/// <summary>
	/// This is called when the screen should draw itself.
	/// </summary>
	void GameScreen::Draw(float deltaTime)
	{

	}
	/// <summary>
	/// Tells the screen to go away. Unlike ScreenManager.RemoveScreen, which
	/// instantly kills the screen, this method respects the transition timings
	/// and will give the screen a chance to gradually transition off.
	/// </summary>
	void GameScreen::ExitScreen()
	{
		if (m_TransitionOffTime == 0.0f)
		{
			// If the screen has a zero transition time, remove it immediately.
			m_ScreenManager->RemoveScreen(this);
			m_DeleteScreen = true;
		}
		else
		{
			// otherwise flag that it should transition off and then exit.
			m_IsExiting = true;
		}
	}

}