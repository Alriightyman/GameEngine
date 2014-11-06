#include "Script.h"
#include <Windows.h>
#include <iostream>

Script::Script(void)
{
	m_luaState = luaL_newstate();
	luaL_openlibs(m_luaState);

	lua_getglobal( m_luaState, "package" );
    lua_getfield( m_luaState, -1, "path" ); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring( m_luaState, -1 ); // grab path string from top of stack
    cur_path.append( ";" ); // do your path magic here
	char path[265];
	GetCurrentDirectory(256,path);
	cur_path.append(path);
    cur_path.append( "\\Content\\Scripts\\?;\\Content\\Scripts\\?.lua" );
    lua_pop( m_luaState, 1 ); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring( m_luaState, cur_path.c_str() ); // push the new one
    lua_setfield( m_luaState, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( m_luaState, 1 ); // get rid of package table from top of stack
}

Script::Script(const char* scriptFile)
{
	Script();
	LoadScript(scriptFile);
}

Script::~Script(void)
{
	lua_close(m_luaState);
}

bool Script::LoadScript(const char* scriptFile)
{
	bool value = true;

	try
	{
		int state = luaL_dofile(m_luaState,scriptFile);
		OutputError(state);
	}
	catch(...)
	{
		value = false;
	}

	return value;
}

std::string Script::GetGlobalString(const char* name)
{
	std::string value = "";
	try
	{
		lua_getglobal(m_luaState,name);
		value = lua_tostring(m_luaState,-1);
		lua_pop(m_luaState,1);
	}
	catch (...) { }

	return value;
}


void Script::SetGlobalString(const char* name,const char* value)
{
	lua_pushstring(m_luaState,value);
	lua_setglobal(m_luaState,name);
}

double Script::GetGlobalNumber(const char* name)
{
	double value = 0.0;
	try
	{
		lua_getglobal(m_luaState,name);
		value = lua_tonumber(m_luaState, -1);
		lua_pop(m_luaState,1);
	}
	catch(...) { } 

	return value;
}

void Script::SetGlobalNumber(const char* name, double value)
{
	lua_pushnumber(m_luaState,(float)value);
	lua_setglobal(m_luaState,name);
}

bool Script::GetGlobalBoolean(const char* name)
{
	bool value = 0;
	try 
	{
		lua_getglobal(m_luaState, name);
		value = static_cast<bool>(lua_toboolean(m_luaState, -1));
		lua_pop(m_luaState, 1);
	}
	catch(...) { }

	return value;
}   
	
void Script::SetGlobalBoolean(const char* name, bool value)
{
	lua_pushboolean(m_luaState, (int)value);
	lua_setglobal(m_luaState, name);
}
	
void Script::RunFunction(const char* name, int numAgrs, int isReturn)
{
	//call script function, 0 args, 0 retvals
	lua_getglobal(m_luaState, name);
	lua_call(m_luaState, numAgrs, isReturn);
}
void Script::RunFunction(const char* name,const char* arg, int numAgrs, int isReturn)
{
	lua_getglobal(m_luaState, name);
	lua_pushstring(m_luaState,arg);
	lua_call(m_luaState, numAgrs, isReturn);
}

void Script::RunFunction(const char* name,double arg, int numAgrs, int isReturn)
{
	lua_getglobal(m_luaState, name);
	lua_pushnumber(m_luaState,arg);
	lua_call(m_luaState, numAgrs, isReturn);
}

void Script::OutputError( int state )
{
    if(state != 0)
    {
        std::cerr << "ERR: " << lua_tostring(m_luaState, state) << std::endl;
        lua_pop(m_luaState, 1); //remove error
    }
}