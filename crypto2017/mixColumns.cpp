#include "stdafx.h"
#include "cipher.h"
#include <iostream>

#include <bitset>
using namespace std;
//Constant matrix/polynomial (AES standard)
int mulMatrix[4][4] = { { 2,3,1,1 },{ 1,2,3,1, },{ 1,1,2,3 },{ 3,1,1,2 } }; 
// Invertible polynomial (AES standard)
int modVal = 0x11b; 
//returns the position of 1 in the number (calculates from MSB)
int position(int value) {
	int count = 8;
	bitset <9> x = value;
	while (count >= 0) {
		if (x[count] == 1) {
			return count;
		}
		count--;
	}
	//The compiler reaches here iff value is 0
	return (count);
}
//This finds mod invertible polynomial (x^8+x^4+x^3+x+1)
int FindMod(int prod) {
	if (prod<256) {
		return prod;
	}
	int posMod = position(modVal);
	int posProd = position(prod);
	int temp = 0;
	while (posProd >= posMod) {
		temp = prod ^ (modVal << (posProd - posMod));
		posProd = position(temp);
	}
	//returns after finding mod
	return temp;
}
int ProductInFiniteField(int num1, int num2) {//multiplication of two numbers in binary and finite field
	bitset<8> num_1 = num1;
	bitset<16> num_2 = num2;
	bitset<16> prod = 0;
	int i = 7;
	while (i >= 0) {
		if (num_1[i] == 1) {//this is binary multiplication
			bitset<16> temp = num2 << i;
			prod = prod ^ temp;
		}
		i--;
	}
	return (int)(prod.to_ulong());
}
//Mix Columns methods
void mixColumns(int state[][4]) {
	int tempState[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int temp = 0;
			for (int k = 0; k < 4; k++) {
				//multiplying the matrices and xor-ing them to find one state
				temp = temp ^ ProductInFiniteField(mulMatrix[i][k], state[k][j]);
			}			
			tempState[i][j] = FindMod(temp);
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = tempState[i][j];
		}
	}
}

void AddRoundKey(int state[][4], int * roundKey, int round) {

	for (int i = 0; i < Nb; ++i) {
		for (int j = 0; j < Nb; ++j) {
			state[j][i] ^= roundKey[round * Nb * 4 + i * Nb + j];
		}
	}
}
