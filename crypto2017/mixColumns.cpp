#include "stdafx.h"
#include <iostream>

#include <bitset>
using namespace std;
int state[4][4] = { { 1, 2,3,4 },{ 1, 2,3,4 },{ 1, 3,3,4 },{ 1, 2,3,4 } };
int mulMatrix[4] = { 2,3,1,1 };
int modVal = 0x11b;
int position(int value) {
	int count = 8;
	bitset <9> x = value;
	while (count >= 0) {
		if (x[count] == 1) {
			return count;
		}
		count--;
	}
	return (count);
}
int FindMod(int prod) {
	if (modVal > prod) {
		return prod;
	}
	if (modVal == prod) {
		return 0;
	}
	int posMod = position(modVal);
	int posProd = position(prod);
	int temp = 0;
	while (posProd >= posMod) {
		temp = prod ^ (modVal << (posProd - posMod));
		posProd = position(temp);
	}
	return temp;
}
int ProductInFiniteField(int num1, int num2) {
	bitset<8> num_1 = num1;
	bitset<8> num_2 = num2;
	bitset<16> prod = 0;
	bitset<16> temp = num2;
	int i = 7;
	while (i >= 0) {
		if (num_1[i] == 1) {
			temp = num2 << i;
			prod = prod ^ temp;
		}
		i--;
	}
	return (int)(prod.to_ulong());
}

void mixColumns() {
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int temp = 0;
			for (int k = 0; k < 4; k++) {
				temp = temp ^ ProductInFiniteField(state[k][j], mulMatrix[count]);
				count = (count + 1) % 4;
			}
			state[i][j] = FindMod(temp);
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
