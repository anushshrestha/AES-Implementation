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
vector <int> KeySubWord(vector <int> word)
{
	for (int row = 0; row < 4; row++) {
		word[row] = sbox[word[row] >> 4][word[row] & 0x0F];
		cout << hex << word[row] << ' ';
	}
	return(word);
}

// Performs the RotWord function for KeyExpansion. The first byte in the integer is rotated
// to the end.
vector <int> KeyRotateWord(vector <int> keyWord)
{
	keyWord.push_back(keyWord[0]);
	keyWord.erase(keyWord.begin());
	return keyWord;
}

vector<vector<int>> KeyExpansion(vector <int> key, int Nb)
{
	const vector <vector <int>> Rcon = {
		{ 0x01, 0x00, 0x00, 0x00 },
		{ 0x02, 0x00, 0x00, 0x00 },
		{ 0x04, 0x00, 0x00, 0x00 },
		{ 0x08, 0x00, 0x00, 0x00 },
		{ 0x10, 0x00, 0x00, 0x00 },
		{ 0x20, 0x00, 0x00, 0x00 },
		{ 0x40, 0x00, 0x00, 0x00 },
		{ 0x80, 0x00, 0x00, 0x00 },
		{ 0x1b, 0x00, 0x00, 0x00 },
		{ 0x36, 0x00, 0x00, 0x00 }
	};

	int Nk = key.size() / 4; // key length (in words): 4/6/8 for 128/192/256-bit keys
	int Nr = Nk + 6;       // no of rounds: 10/12/14 for 128/192/256-bit keys

	vector <vector <int>> expandedKey(Nb, vector <int>(Nb*(Nr + 1), 0));
	vector <int> temp(Nk, 0);
	int counter = 0;

	// initialise first Nk words of expanded key with cipher key
	for (counter = 0; counter < Nk; counter++) {
		vector <int> r = { key[4 * counter], key[4 * counter + 1], key[4 * counter + 2], key[4 * counter + 3] };
		expandedKey[counter] = r;
	}

	// expand the key into the remainder of the schedule
	for (counter = Nk; counter < Nb * (Nr + 1); ++counter) {
		vector <int> word(Nb*(Nr + 1), 0);
		temp = expandedKey[counter - 1];

		// each Nk'th word has extra transformation
		if ((counter % Nk) == 0) {
			temp = KeySubWord(KeyRotateWord(temp));
			for (int t = 0; t < 4; t++) {
				temp[t] ^= Rcon[(counter - 1) / Nk][t];
			}
		}
		// 256-bit key has subWord applied every 4th word
		else if (Nk > 6 && (counter % Nk) == 4) {
			temp = KeySubWord(temp);
		}

		vector <int> newKey;
		for (int t = 0; t < Nk; t++) {
			newKey.push_back(expandedKey[counter - Nk][t] ^ temp[t]);
		}
		expandedKey.push_back(newKey);
	}
	return expandedKey;
}
