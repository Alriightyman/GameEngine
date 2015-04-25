#include "RC4.h"

// helper function
static void Swap(int* S, int a, int b)
{
	int tmp = S[a];
	S[a] = S[b];
	S[b] = tmp;
}

RC4::RC4(void) : j(0),i(0)
{
}


RC4::~RC4(void)
{
}
// initialize the RC4
void RC4::KeySchedulingAlgorithm(std::string key)
{
	int length = key.size();
	int a = 0;

	for(int x = 0; x < 256; x++)
	{
		S[x] = x;
		K[x] = key[a];
		a++;
		if (a > length-1)
			a = 0;
	}

	for(int x = 0; x < 256; x++)
	{
		a = (a + S[x] + K[x]) % 256;
		Swap(S,x,a);
	}

}
// the Pseudo Random Generation Algorithm with the while loop moved out of it
// to the Encrypt function and changed to a for loop.
char RC4::PseudoRandomGenerationAlgorithm()
{
	i = (i + 1) % 256;
	j = (j + S[i]) % 256;

	Swap(S,i,j);

	int index = (S[i] + S[j]) % 256;
	char c = S[index];
	return c;
}
// encrypts the plain text into a cipher text
std::string RC4::Encrypt(std::string plainText)
{
	int length = plainText.size();
	std::string cipher = plainText;
	char c;
	for(int x = 0; x <= length; x++)
	{
		c = PseudoRandomGenerationAlgorithm();
		cipher[x] = cipher[x] ^ c;
	}

	return cipher;
}


