#pragma once
#include "playerIndex.h"
namespace Engine
{

	class PlayerIndexEventArgs
	{
	private:
		int m_playerIndex;
	public:
		int Getint() const { return m_playerIndex; }
	public:
		PlayerIndexEventArgs(int playerIndex);
	};


}