#include "stdafx.h"
#include <iostream>
#include "stdint.h"
#include <string>
#include <vector>
#include "stdio.h"
#include "keyexpansion.h"
#include "cipher.h"
using namespace std;

// Performs the SubWord substitution for KeyExpansion. 
int KeySubWord(int word)
{
	int result;
	result = (int)sbox[(word >> 4) & 0x0000000F][word & 0x0000000F];
	result += (int)sbox[(word >> 12) & 0x0000000F][(word >> 8) & 0x0000000F] << 8;
	result += (int)sbox[(word >> 20) & 0x0000000F][(word >> 16) & 0x0000000F] << 16;
	result += (int)sbox[(word >> 28) & 0x0000000F][(word >> 24) & 0x0000000F] << 24;

	return(word);
}

// Performs the RotWord function for KeyExpansion. The first byte in the integer is rotated
// to the end.
int KeyRotateWord(int keyWord)
{
	return ((keyWord) << 8) | ((keyWord) >> 24);
}

int * KeyExpansion(int * key, int keysize)
{
	//const vector <vector <int>> Rcon = {
	//	{ 0x01, 0x00, 0x00, 0x00 },
	//	{ 0x02, 0x00, 0x00, 0x00 },
	//	{ 0x04, 0x00, 0x00, 0x00 },
	//	{ 0x08, 0x00, 0x00, 0x00 },
	//	{ 0x10, 0x00, 0x00, 0x00 },
	//	{ 0x20, 0x00, 0x00, 0x00 },
	//	{ 0x40, 0x00, 0x00, 0x00 },
	//	{ 0x80, 0x00, 0x00, 0x00 },
	//	{ 0x1b, 0x00, 0x00, 0x00 },
	//	{ 0x36, 0x00, 0x00, 0x00 }
	//};
	int Nb = 4, Nr, Nk, idx, temp;
	int Rcon[] = {
		0x01,0x02,0x04,0x08,
		0x10,0x20,0x40,0x80,
		0x1b,0x36,0x6c,0xd8,
		0xab,0x4d,0x9a
	};

	switch (keysize) {
	case 128: Nr = 10; Nk = 4; break;
		//case 192: Nr = 12; Nk = 6; break;
		//case 256: Nr = 14; Nk = 8; break;
	default: return;
	}

	int w[44];
	for (idx = 0; idx < Nk; ++idx) {
		w[idx] = ((key[4 * idx]) << 24) | ((key[4 * idx + 1]) << 16) |
			((key[4 * idx + 2]) << 8) | ((key[4 * idx + 3]));
	}

	for (idx = Nk; idx < Nb * (Nr + 1); ++idx) {
		temp = w[idx - 1];
		if ((idx % Nk) == 0)
			temp = KeySubWord(KeyRotateWord(temp)) ^ Rcon[(idx - 1) / Nk];
		else if (Nk > 6 && (idx % Nk) == 4)
			temp = KeySubWord(temp);
		w[idx] = w[idx - Nk] ^ temp;
	}

	//vector <vector <int>> expandedKey(Nb, vector <int>(Nb*(Nr + 1), 0));
	//vector <int> temp(Nk, 0);
	//int counter = 0;

	//// initialise first Nk words of expanded key with cipher key
	//for (counter = 0; counter < Nk; counter++) {
	//	vector <int> r = { key[4 * counter], key[4 * counter + 1], key[4 * counter + 2], key[4 * counter + 3] };
	//	expandedKey[counter] = r;
	//}

	//// expand the key into the remainder of the schedule
	//for (counter = Nk; counter < Nb * (Nr + 1); ++counter) {
	//	vector <int> word(Nb*(Nr + 1), 0);
	//	temp = expandedKey[counter - 1];

	//	// each Nk'th word has extra transformation
	//	if ((counter % Nk) == 0) {
	//		temp = KeySubWord(KeyRotateWord(temp));
	//		for (int t = 0; t < 4; t++) {
	//			temp[t] ^= Rcon[(counter - 1) / Nk][t];
	//		}
	//	}
	//	// 256-bit key has subWord applied every 4th word
	//	else if (Nk > 6 && (counter % Nk) == 4) {
	//		temp = KeySubWord(temp);
	//	}

	//	vector <int> newKey;
	//	for (int t = 0; t < Nk; t++) {
	//		newKey.push_back(expandedKey[counter - Nk][t] ^ temp[t]);
	//	}
	//	expandedKey.push_back(newKey);
	//}

	return w;
}
