#pragma once
#include "PlayerIndex.h"
namespace Engine
{

	class PlayerIndexEventArgs
	{
	private:
		PlayerIndex m_playerIndex;
	public:
		PlayerIndex GetPlayerIndex() const { return m_playerIndex; }
	public:
		PlayerIndexEventArgs(PlayerIndex playerIndex);
	};


}