#pragma once
#include "GameScreen.h"
#include <d3d11.h>
#include <functional>

namespace Engine
{
	class MessageBoxScreen :
		public GameScreen
	{
	private:
		std::string m_Message;
		ID3D11ShaderResourceView* m_GradientTexture;

	public:
		std::function<void (void*,int)> Accepted;
		std::function<void (void*,int)> Cancelled;

	public:
		MessageBoxScreen(std::string message,bool includeUsageText = true);
		~MessageBoxScreen(void);
		void Load();

		void HandleInput(InputState* input);

		void Draw(float deltaTime);
	};


}