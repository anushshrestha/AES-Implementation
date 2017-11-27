#include "stdafx.h"
#include "cipher.h"
#include <iostream>
#include <bitset>

using namespace std;
//Constant matrix/polynomial (AES standard)
//DCL30 - C.Declare objects with appropriate storage durations
const int mulMatrix[4][4] = { { 2,3,1,1 },{ 1,2,3,1, },{ 1,1,2,3 },{ 3,1,1,2 } };
const int invMulMatrix[4][4] = { {0x0E,0x0B,0x0D,0x09},{ 0x09, 0x0E, 0x0B, 0x0D},{0x0D, 0x09, 0x0E, 0x0B},{ 0x0B, 0x0D, 0x09, 0x0E} };
// Irreducable polynomial (AES standard(x^8+x^4+x^3+x+1))
const int modVal = 0x11b;

//takes integer input value
//returns the position of first 1 present in value (calculates from MSB)
int position(int value) {
	int count = 15; //EXP53-CPP. Do not read uninitialized memory
	bitset <16> x = value;
	while (count >= 0) {
		if (x[count] == 1) {
			return count;
		}
		count--;
	}
	//The compiler reaches here iff value is 0
	return (count);
}
//Takes integer input
//Finds mod Irreducable polynomial (x^8+x^4+x^3+x+1)
int FindMod(int prod) {
	//returns the value if it already can be stored in 8 bits
	if (prod<256) {
		return prod;
	}
	//ModVal : Invertible Polynomial
	int posMod = position(modVal);
	//prod: The number whose mod has to be generated
	int posProd = position(prod);
	//initializing temp to the divisor
	int temp = prod;
	//compares the position of first 1 in the Invertible Polynomial 
	while (posProd >= posMod) {
		//The next step is equivalent to finding the remainder. Temp is the remainder here, prod is the dividend and ModVal is the divisor.
		temp = temp ^ (modVal << (posProd - posMod));
		//Updating the remainder Position
		posProd = position(temp);
	}
	//returns after finding mod
	return temp;
}
//Mix Columns method
//Takes state as the input
//Does matrix multiplication with a matrix present in AES standard
void mixColumns(int state[][4]) {
	//TempState is the temporary state where updated state values are stored after finding the product of the two matrices which will later be transfered to state
	int tempState[4][4] = { 0 }; //EXP53 - CPP.Do not read uninitialized memory
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int temp = 0; //EXP53 - CPP.Do not read uninitialized memory
			for (int k = 0; k < 4; k++) {
				//multiplying the matrices and xor-ing them to find one state
				temp = temp ^ productOfTwoNumbers(mulMatrix[i][k], state[k][j]);
			}
			//finding mod, in case the value of temp exceeds what can be stored in 8 bits.
			tempState[i][j] = FindMod(temp);
		}
	}
	//Updating state
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = tempState[i][j];
		}
	}

	//MSC53 - CPP.Do not return from a function declared
}
void InvMixColumns(int state[][4]) {
	//TempState is the temporary state where updated state values are stored after finding the product of the two matrices which will later be transfered to state
	int tempState[4][4] = { 0 }; //EXP53 - CPP.Do not read uninitialized memory
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int temp = 0; //EXP53 - CPP.Do not read uninitialized memory
			for (int k = 0; k < 4; k++) {
				//multiplying the matrices and xor-ing them to find one state
				temp = temp ^ productOfTwoNumbers(invMulMatrix[i][k], state[k][j]);
			}
			//finding mod, in case the value of temp exceeds what can be stored in 8 bits.
			tempState[i][j] = FindMod(temp);
		}
	}
	//Updating state
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = tempState[i][j];
		}
	}
}

// xor the roundkey to the current state 
// takes as input the round number and the state and the expanded key
// modify the state with the result
void AddRoundKey(int state[][4], int * roundKey, int round) {
	for (int col = 0; col < Nb; ++col) {
		for (int row = 0; row < Nb; ++row) {
			state[row][col] ^= roundKey[round * Nb * 4 + col * Nb + row];
		}
	}
	//MSC53 - CPP.Do not return from a function declared
}
