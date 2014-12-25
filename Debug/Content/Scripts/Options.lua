
-- globals
-- old test options menu stuff
Ungulate = "Default"
Language = "Default"
CurrentUngulate = 0
LanguageValue = 0

function SetMenuText()
	-- create arrays
	local ungulateArray = { "BactrianCamel", "Dromedary", "Llama" }
	local LanguageArray = { "C++","French", "Deoxyribonucleic" }
	-- set ungulate
	Ungulate = ungulateArray[CurrentUngulate]
	Language = LanguageArray[LanguageValue]
end

-- new options menu stuff
function Load(options)
	local Audio = MenuEntry("Audio");
	local Video = MenuEntry("Video");
	local Gameplay = MenuEntry("Gameplay");
	local back = MenuEntry("Back");

	options:Push(Audio);
	options:Push(Video);
	options:Push(Gameplay);
	options:Push(back);
	
end
