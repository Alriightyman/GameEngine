#include "Debug.h"
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

std::ofstream out("debug.txt");

Debug::Debug(void)
{
	// only run the console if in debug mode
#ifdef _DEBUG
	InitailizeConsole();
#endif
}


Debug::~Debug(void)
{
	if(out.good() && &out != nullptr)
		out.close();
}

void Debug::InitailizeConsole()
{
	AllocConsole();

	// set up for output, we don't need input
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt,"w");
	setvbuf(hf_out, 0,_IONBF,1);
	*stdout = *hf_out;
}

void Debug::Print(char c[])
{
	out << c << std::endl;
}

std::ofstream& Debug::operator<<(char c[]) { out << c; return out; }
std::ofstream& Debug::operator<<(int i) { out << i; return out;}
std::ofstream& Debug::operator<<(float f) {out << f; return out; }
std::ofstream& Debug::operator<<(bool b) { if (b) out << "True"; else out << "False"; return out; }

void Debug::Endl(int nr) { for (int i = 0; i < nr; i++) out << std::endl; }
