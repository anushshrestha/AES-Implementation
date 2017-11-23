#include "stdafx.h"
#include "cipher.h"
#include <string>
#include <vector>
#include <bitset>
#include <iostream>
using namespace std;

int addVar = 198;
bool firstTime = true;
int affineConst = 143;
int irreducable = 0x11B;

int productInFiniteField(int num1, int num2) {

	int i = 7;
	int product = 0;
	bitset<16> prod = 0;
	bitset<8> num_1 = num1;
	bitset<16> num_2 = num2;

	while (i >= 0) {
		if (num_1[i] == 1) {
			prod = prod ^ (num_2 << i);
		}
		i--;
	}
	return (int)(prod.to_ulong());
}


int mulInverse(int init) {
	if (init == 0) {
		return 0;
	}

	int temp = irreducable;
	int q = 0;
	int prev = 0;
	int present = 1;

	while (temp > 1) {
		int q1 = 1;
		// finding position of first 1 in the divisor
		int pos1 = position(init);
		// finding position of first 1 in the divident
		int pos2 = position(temp);

		// if position of dividend(pos2) is greater than the position of the divisor (pos1), 
		// divisor has to be multiplied (or shifted, in case of binary)
		if (pos1 <= pos2) {
			// Calculate quotient for the first xor
			q1 = q1 << (pos2 - pos1);
			int temp2 = init << (pos2 - pos1);
			// xor the values to get the remainer (Here, temp is the remainder)
			temp = temp^temp2;
		}
		else {
			// calculate the intermediate inverse of the function using quotients
			int random = prev;
			// store previous intermediate inverse
			prev = present;
			// product in finite field to get the new intermediate inverse
			present = productInFiniteField(present, q);
			present = random ^ present;
			int temporary = temp;
			temp = init;
			init = temporary;
			q1 = 0;
			q = 0;
		}
		q += q1;
	}

	int random = prev;
	prev = present;
	// final inverse by finding product in finite field and xoring it with the previous intermediate inverse.
	present = productInFiniteField(present, q);
	present = random ^ present;
	return present;
}
int affineTrans(int k) {
	// 01 doesn't have an inverse that can be represented in 8 bits.
	if (k == 0x11a) {
		return 0x7c;
	}

	int i = 0, j = 0;
	bitset<8> prod, finalVal = 0;
	bitset<8> affineConstBit = affineConst;
	bitset<8> value = k;
	prod = value;

	// reversing the bits for easy calculation
	for (int i = 0; i < 8; i++) {
		value[7 - i] = prod[i];
	}

	// Affine transformation done here. Matrix multiplication and addition of the matrix
	while (i < 8) {
		prod = affineConstBit & value;

		while (j < 8) {
			finalVal[i] = finalVal[i] ^ prod[j];
			j++;
		}

		j = 0;
		int temp = affineConstBit[0];
		affineConstBit = affineConstBit >> 1;
		affineConstBit[7] = temp;
		i++;
	}

	prod = finalVal;
	for (int i = 0; i < 8; i++) {
		finalVal[7 - i] = prod[i];
	}

	bitset<8> addVarBit = addVar;
	finalVal = finalVal ^ addVarBit;
	prod = finalVal;

	for (int i = 0; i < 8; i++) {
		finalVal[7 - i] = prod[i];
	}

	return (int)(finalVal.to_ulong());
}

int sBoxGen(int value) {
	return affineTrans(mulInverse(value));
}

void SubBytes(int state[][4]) {
	for (int i = 0; i < 4; i++) {
		/*if (i % 16 == 0) {
			cout << endl;
		}*/
		for (int j = 0; j < 4; j++) {
			// generating state using sbox
			state[i][j] = sBoxGen(state[i][j]);
			//cout << std::hex << state[i][j] << " ";
		}
		//cout << endl;
	}
}

//
//void SubBytes(int state[][4])
//{
//	int k = mulInverse(0x10);
//	bitset<8> kk = k;
//	int r = affineTrans(k);
//	int m;
//	m = 44;
//	byteSub();
//	cin >> m;
//}
