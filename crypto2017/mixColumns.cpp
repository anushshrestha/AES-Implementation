#include "stdafx.h"
#include "cipher.h"
#include <iostream>

#include <bitset>
using namespace std;
// Constant matrix/polynomial (AES standard)
int mulMatrix[4] = { 2,3,1,1 };
// Invertible polynomial (AES standard)
int modVal = 0x11b;

// returns the position of 1 in the number (calculates from MSB)
int position(int value) {
	int count = 8;
	bitset <9> x = value;
	while (count >= 0) {
		if (x[count] == 1) {
			return count;
		}
		count--;
	}
	// The compiler reaches here iff value is 0
	return (count);
}

// find mod invertible polynomial (x^8+x^4+x^3+x+1)
int FindMod(int prod) {
	if (modVal > prod) {
		return prod;
	}
	int posMod = position(modVal);
	int posProd = position(prod);
	int temp = 0;
	while (posProd >= posMod) {
		temp = prod ^ (modVal << (posProd - posMod));
		posProd = position(temp);
	}
	// After finding mod
	return temp;
}

// multiplication of two numbers in binary and finite field
int ProductInFiniteField(int num1, int num2) {
	bitset<8> num_1 = num1;
	bitset<16> num_2 = num2;
	bitset<16> prod = 0;
	int i = 7;
	while (i >= 0) {
		// Binary multiplication
		if (num_1[i] == 1) {
			bitset<16> temp = num2 << i;
			prod = prod ^ temp;
		}
		i--;
	}
	return (int)(prod.to_ulong());
}

// Mix Columns methods
void mixColumns(int state[][4]) {
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int temp = 0;
			for (int k = 0; k < 4; k++) {
				// multiply the matrices and xor-ing them to find one state
				temp = temp ^ ProductInFiniteField(state[k][j], mulMatrix[count]);
				// rotation to prevent out of bounds error.
				count = (count + 1) % 4;
			}
			state[i][j] = FindMod(temp);
			// increment count by 1 for the next round
			count = (count + 1) % 4;
		}
	}
}

//int main() {
//	mixColumns();
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++) {
//
//			state[i][j] = FindMod(state[i][j]);
//			std::cout <<hex<< state[i][j] << " ";
//		}
//		std::cout << endl;
//	}
//	int m;
//	cin >> m;
//	return 0;
//}
