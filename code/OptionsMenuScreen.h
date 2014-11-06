#pragma once
#include "menuscreen.h"
namespace Engine
{
	class OptionsMenuScreen :
		public MenuScreen
	{
	private:
		MenuEntry* m_UngulateMenuEntry;
		MenuEntry* m_LanguageMenuEntry;
		MenuEntry* m_FrobnicateMenuEntry;
		MenuEntry* m_ElfMenuEntry;
		enum Ungulate { BactrianCamel = 1, Dromedary, Llama };
		Ungulate m_CurrentUngulate;
		std::string m_Languages[3];
		int m_CurrentLanguage;
		bool m_Frobnicate;
		int m_Elf;

	public:
		OptionsMenuScreen(void);
		~OptionsMenuScreen(void);
		void Load();
		void SetMenuEntryText();
		void UngulateMenuEntrySelected(void* sender,PlayerIndex playerIndex);
		void LanguageMenuEntrySelected(void* sender,PlayerIndex playerIndex);
		void FrobnicateMenuEntrySelected(void* sender,PlayerIndex playerIndex);
		void ElfMenuEntrySelected(void* sender,PlayerIndex playerIndex);
	};

}
