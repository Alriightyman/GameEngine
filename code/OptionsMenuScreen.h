#pragma once
#include "menuscreen.h"

class Script;

namespace Engine
{
	class OptionsMenuScreen :
		public MenuScreen
	{
	private:
		static bool isInitialized;
	public:
		static void Bind(Script* script);

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
		OptionsMenuScreen(Script* script);
		~OptionsMenuScreen(void);
		void Load();
		void SetMenuEntryText();
		void UngulateMenuEntrySelected(void* sender,int playerIndex);
		void LanguageMenuEntrySelected(void* sender,int playerIndex);
		void FrobnicateMenuEntrySelected(void* sender,int playerIndex);
		void ElfMenuEntrySelected(void* sender,int playerIndex);
	
	private:
		// these functions are to use so lua 
		// can work with std::vector and 
		// std::function, std::bind
		void Push(MenuEntry* entry);
		void AddSelected(MenuEntry* entry);
	};

}
