#include "stdafx.h"
#include "cipher.h"
#include <string>
#include <vector>
#include <bitset>
#include <iostream>
using namespace std;

//affineConst:Matrix to be added to the intermediate generated sBox
int addVar = 198;
int affineConst = 143;

int invAffineConst = 0x25;
int invAddVar = 0xa0;
//(AES standard) Irreducable polynomial (x^8+x^4+x^3+x+1)
int irreducable = 0x11B;

//Takes two integer inputs and computes multiplication
//Computes multiplication of two numbers
int productOfTwoNumbers(int num1, int num2) {

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
//takes one integer input
//finds the multiplication inverse in 2^8 galois field using extended  algorithm
int mulInverse(int init) {
	if (init == 0) {//0 cannot be the divisor
		return 0;
	}

	int remainder = irreducable;
	int q = 0;
	int prev = 0;
	int present = 1;
	//Until the remainder is 1, this while loop runs
	while (remainder > 1) {
		int q1 = 1;
		// finding position of first 1 in the divisor
		int pos1 = position(init);
		// finding position of first 1 in the divident
		int pos2 = position(remainder);

		// if position of dividend(pos2) is greater than the position of the divisor (pos1), 
		// divisor has to be multiplied (or shifted, in case of binary)
		if (pos1 <= pos2) {
			// Calculate quotient for the first xor
			q1 = q1 << (pos2 - pos1);
			int temp2 = init << (pos2 - pos1);
			// xor the values to get the remainer (Here, temp is the remainder)
			remainder = remainder^temp2;
		}
		else {
			// calculate the intermediate inverse of the function using quotients
			int random = prev;
			// store previous intermediate inverse
			prev = present;
			// product in finite field to get the new intermediate inverse
			present = productOfTwoNumbers(present, q);
			present = random ^ present;
			int temporary = remainder;
			remainder = init;
			init = temporary;
			q1 = 0;
			q = 0;
		}
		q += q1;
	}

	int prevTemp = prev;
	prev = present;
	// final inverse by finding product in finite field and xoring it with the previous intermediate inverse.
	present = productOfTwoNumbers(present, q);
	present = prevTemp ^ present;
	return present;
}
//takes three input. 
//The first input is the input value for which the affine tranformation has to be computed
//The second and the third input is dependant on whether the  affine tranformation is for generation of subbytes or inverse subbytes
//Bitwise Matrix multiplication and addition of the matrix
int affineTrans(int k, int affConst, int addVariable) {
	// 01 doesn't have a multiplicative inverse that can be represented in 8 bits.
	if (k == 0x11a) {
		return 0x7c;
	}
	// 01's affine transformation is 7c but during sBox Generation the multiplicative inverse of 01 is not represented in 8 bit, so we change it to 0x7c, this is the inverse of that
	if (affConst == invAffineConst && k == 0x7c) {
		return 01;
	}
	int i = 0, j = 0;
	bitset<8> value, finalVal = 0;
	bitset<8> affineConstBit = affConst;
	bitset<8> prod = k;

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

	bitset<8> addVarBit = addVariable;
	finalVal = finalVal ^ addVarBit;
	prod = finalVal;

	for (int i = 0; i < 8; i++) {
		finalVal[7 - i] = prod[i];
	}

	return (int)(finalVal.to_ulong());
}
//Takes 1 integer input
// generates s box transformation for that input
// returns the S Box transformation Value
int sBoxGen(int value) {
	return affineTrans(mulInverse(value), affineConst, addVar);
}
//Finds sBox transformation for every value in the 4X4 state matrix
void SubBytes(int state[][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// generating state using sbox
			state[i][j] = sBoxGen(state[i][j]);
		}
	}
}

//Generating inverse s box
int invSBoxGen(int value) {
	if (mulInverse(affineTrans(value, invAffineConst, invAddVar)) == 0x11a) {
		return 0x01;
	}
	return mulInverse(affineTrans(value, invAffineConst, invAddVar));
}
//Finds inverse s box for every value in the 4X4 state matrix and stores it in the value
void InvSubBytes(int state[][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// generating state using invSbox
			state[i][j] = invSBoxGen(state[i][j]);
		}
	}
}

