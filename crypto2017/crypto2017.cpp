/**
* AES implementation,
* Author : Anush Shrestha, Vaishanvi Batni, Arindam Mitra
* This is an implementation of FIPS 197 (csrc.nist.gov/publications/fips/fips197/fips-197.pdf)
* This has security optimisations and prevention from side channel attack.
*/

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include "cipher.h"

using namespace std;
#define Nr 10

// Cipher is the main function that encrypts the PlainText.
// update the state to cipher text
void Cipher(int state[][4], int * cipherKey)
{	
	int * expandedKeys = KeyExpansion(cipherKey, 128);
	uint8_t round = 0;

	// Add the First round key to the state before starting the rounds.
	AddRoundKey(state, expandedKeys, round);

	// There will be Nr rounds.
	// The first Nr-1 rounds are identical.
	// These Nr-1 rounds are executed in the loop below.
	for (round = 1; round < Nr; ++round)
	{
		SubBytes(state);
		ShiftRows(state);
		mixColumns(state);
		AddRoundKey(state, expandedKeys, round);
	}

	// The last round is given below.
	// The MixColumns function is not here in the last round.
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, expandedKeys, Nr);
}

int main()
{
	// Test cipher key from FIPS 197 Appendix A Page 27
	int cipherKey[16] = {
		0x2b,0x7e,0x15,0x16,
		0x28,0xae,0xd2,0xa6,
		0xab,0xf7,0x15,0x88,
		0x09,0xcf,0x4f,0x3c
	};

	int * expandedKeys = KeyExpansion(cipherKey, 128);

	//Test state value from FIPS 197 Appendix B Page 33
	int state[4][4] = {
		{0x32,0x88,0x31,0xe0},
		{0x43,0x5a,0x31,0x37},
		{0xf6,0x30,0x98,0x07},
		{0xa8,0x8d,0xa2,0x34}
	};

	SubBytes(state);
	ShiftRows(state);

	// This code is just to check output of entire program
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			cout << hex << state[row][column] << ' ';
		}
		cout << endl;
	}
	cout << dec;
	system("pause");
	return 0;
}
