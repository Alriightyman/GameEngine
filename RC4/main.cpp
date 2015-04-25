#include <string>
#include <fstream>
#include "RC4.h"

int main(int argc, char** argv)
{
	RC4 rc4;
	// open file in binary mode
	std::ifstream fin(argv[1],std::ios_base::binary);

	std::string plainText;
	// get file size
	fin.seekg(0,fin.end);
	int length = fin.tellg();
	fin.seekg(0,fin.beg);
	// set string size, read contents of file and close it
	plainText.resize(length);
	fin.read(&plainText[0],length);
	fin.close();
	// initialize the rc4 with the key "Asset"
	rc4.KeySchedulingAlgorithm("Asset");
	// encrypt the plaintext
	std::string cipherText = rc4.Encrypt(plainText);
	//  write the encrypted file back to the original filename and close it.
	std::ofstream fout(argv[1],std::ios_base::binary);
	fout.write(&cipherText[0],cipherText.size());
	fout.close();

	return 0;
}