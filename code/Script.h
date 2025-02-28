#pragma once
#include "Lua\LuaBridge\LuaBridge.h"
#include <string>
#include <vector>

class Script
{
private:
	luabridge::lua_State* m_luaState;

public:
	static const std::string ScriptRoot;
	Script(void);
	Script(const char* scriptfile);
	~Script(void);

	bool LoadScript(const char* scriptFile);
	void OutputError(int state);
	// simple sets/gets functions
	luabridge::lua_State* GetState() { return m_luaState; }
	std::string GetGlobalString(const char* name);
	void SetGlobalString(const char* name, const char* value);

	double GetGlobalNumber(const char* name);
	void SetGlobalNumber(const char* name, double value);

	bool GetGlobalBoolean(const char* name);
	void SetGlobalBoolean(const char* name, bool value);
	// run a function
	void RunFunction(const char* name,const char* arg, int numAgrs, int isReturn = 0);
	void RunFunction(const char* name,double arg, int numAgrs, int isReturn = 0);
	void RunFunction(const char* name,int numAgrs = 0, int isReturn = 0);

	double GetResultNumber();
	void Clean();


	std::vector<std::wstring> ReturnArray();
	
};

