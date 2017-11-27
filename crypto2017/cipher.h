#pragma once
#include <string>
using namespace std;

void ShiftRows(int state[][4]);
void InvShiftRows(int state[][4]);

void SubBytes(int state[][4]);
void InvSubBytes(int state[][4]);

void mixColumns(int state[][4]);
void InvMixColumns(int state[][4]);

int * KeyExpansion(int * key, int Nb);
void AddRoundKey(int state[][4], int* roundKey, int round);

/**utility functions*/
int position(int value); // DCL60-CPP. Obey the one-definition rule
int sBoxGen(int value); // avoid memory access
int productOfTwoNumbers(int num1, int num2); // DCL60-CPP. Obey the one-definition rule

// DCL60-CPP. Obey the one-definition rule
const int Nr = 10; // number of rounds 
const int Nb = 4;  // number of columns in state matrix
const int Nk = 4;  // number of 32 bit words in a key.