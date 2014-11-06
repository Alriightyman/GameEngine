#include "OptionsMenuScreen.h"
#include "ScreenManager.h"
#include "Debug.h"
#include <functional>
namespace Engine
{

	OptionsMenuScreen::OptionsMenuScreen(void) : MenuScreen("Options")
	{
		m_CurrentUngulate = Ungulate::Dromedary;
		m_Languages[0] = "C++";
		m_Languages[1] = "French";
		m_Languages[2] = "Deoxyribonucleic";

		m_CurrentLanguage = 0;
		m_Frobnicate = true;
		m_Elf = 23;

		// create our menu entries
		m_UngulateMenuEntry = new MenuEntry("");
		m_LanguageMenuEntry = new MenuEntry("");
		m_FrobnicateMenuEntry = new MenuEntry("");
		m_ElfMenuEntry = new MenuEntry("");

		MenuEntry* back = new MenuEntry("Back");
		using namespace std::placeholders;

		m_UngulateMenuEntry->Selected = std::bind(&OptionsMenuScreen::UngulateMenuEntrySelected,this,_1,_2);
		m_LanguageMenuEntry->Selected = std::bind(&OptionsMenuScreen::LanguageMenuEntrySelected,this,_1,_2);
		m_FrobnicateMenuEntry->Selected = std::bind(&OptionsMenuScreen::FrobnicateMenuEntrySelected,this,_1,_2);
		m_ElfMenuEntry->Selected = std::bind(&OptionsMenuScreen::ElfMenuEntrySelected,this,_1,_2);
		back->Selected = std::bind(&OptionsMenuScreen::OnCancel,this,_1,_2);

		// add entries to the menu
		m_MenuEntries.push_back(m_UngulateMenuEntry);
		m_MenuEntries.push_back(m_LanguageMenuEntry);
		m_MenuEntries.push_back(m_FrobnicateMenuEntry);
		m_MenuEntries.push_back(m_ElfMenuEntry);
		m_MenuEntries.push_back(back);
	}
	void OptionsMenuScreen::Load()
	{
		SetMenuEntryText();
	}

	OptionsMenuScreen::~OptionsMenuScreen(void)
	{

	}

	void OptionsMenuScreen::SetMenuEntryText()
	{
		// get the script manager
		Script* script = m_ScreenManager->GetScript();
		// open the options script...
		script->LoadScript("Content/Scripts/Options.lua");
		// set the Language and Ungulate
		script->SetGlobalNumber("CurrentUngulate",m_CurrentUngulate);
		script->SetGlobalNumber("LanguageValue", m_CurrentLanguage + 1);

		script->RunFunction("SetMenuText");
		// use normal stuff for this for now.
		m_UngulateMenuEntry->Text("Preferred ungulate: " + script->GetGlobalString("Ungulate"));
		m_LanguageMenuEntry->Text("Language: " + script->GetGlobalString("Language"));
		m_FrobnicateMenuEntry->Text("Frobnicate: " + (std::string)(m_Frobnicate ? "On" : "Off"));
		m_ElfMenuEntry->Text("Elf: " + std::to_string(m_Elf));
	}

	void OptionsMenuScreen::UngulateMenuEntrySelected(void* sender,PlayerIndex playerIndex)
	{
		switch(m_CurrentUngulate)
		{
		case Ungulate::BactrianCamel:
			m_CurrentUngulate = Ungulate::Dromedary;
			break;
		case Ungulate::Dromedary:
			m_CurrentUngulate = Ungulate::Llama;
			break;
		case Ungulate::Llama:
			m_CurrentUngulate = Ungulate::BactrianCamel;
			break;
		}

		SetMenuEntryText();
	}

	void OptionsMenuScreen::LanguageMenuEntrySelected(void* sender,PlayerIndex playerIndex)
	{
		m_CurrentLanguage = (m_CurrentLanguage + 1) % 3;

		SetMenuEntryText();
	}

	void OptionsMenuScreen::FrobnicateMenuEntrySelected(void* sender,PlayerIndex playerIndex)
	{
		m_Frobnicate = !m_Frobnicate;

		SetMenuEntryText();
	}

	void OptionsMenuScreen::ElfMenuEntrySelected(void* sender,PlayerIndex playerIndex)
	{
		m_Elf++;

		SetMenuEntryText();

	}

}
