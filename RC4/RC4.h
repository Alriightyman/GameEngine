#pragma once
#include <string>

class RC4
{
private:
	int S[256];
	int K[256];
	int i;
	int j;

public:
	RC4(void);
	~RC4(void);

	void KeySchedulingAlgorithm(std::string key);
	char PseudoRandomGenerationAlgorithm();
	std::string Encrypt(std::string plainText);
};

