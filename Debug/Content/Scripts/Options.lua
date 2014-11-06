
-- globals

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

