#pragma once

#include <fstream>

class Debug
{
public:
	Debug(void);
	~Debug(void);
	void Print(char c[]);
	std::ofstream& operator<<(char c[]);
	std::ofstream& operator<<(int i);
	std::ofstream& operator<<(float f);
	std::ofstream& operator<<(bool b);
	void Endl(int nr);
	private:
	void InitailizeConsole();
};

static Debug debug;

