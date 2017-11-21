#include "stdafx.h"
#include "cipher.h"
#include <iostream>

#include <bitset>
using namespace std;
int mulMatrix[4] = { 2,3,1,1 }; //Constant matrix/polynomial (AES standard)
int modVal = 0x11b; // Invertible polynomial (AES standard)
int position(int value) {//returns the position of 1 in the number (calculates from MSB)
	int count = 8;
	bitset <9> x = value;
	while (count >= 0) {
		if (x[count] == 1) {
			return count;
		}
		count--;
	}
	return (count);//The compiler reaches here iff value is 0
}
int FindMod(int prod) {//This finds mod invertible polynomial (x^8+x^4+x^3+x+1)
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
	return temp;//returns after finding mod
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

void mixColumns(int state[][4]) {//Mix Columns methods
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int temp = 0;
			for (int k = 0; k < 4; k++) {
				temp = temp ^ ProductInFiniteField(state[k][j], mulMatrix[count]);//multiplying the matrices and xor-ing them to find one state
				count = (count + 1) % 4;// rotation, so that count doesn't go out of bounds.
			}
			state[i][j] = FindMod(temp);
			count = (count + 1) % 4;//increasing the count by 1 for the next round
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
