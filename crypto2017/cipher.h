#pragma once
#include <vector>
#include <string>
using namespace std;

void ShiftRows(int state[][4]);
void InverseShiftRows(int state[][4]);
void SubBytes(int state[][4]);
void mixColumns(int state[][4]);
int * KeyExpansion(int * key, int Nb);
void AddRoundKey(int state[][4], int* roundKey, int round);
int position(int value);
int sBoxGen(int value);
void InvSubBytes(int state[][4]);
void InvMixColumns(int state[][4]);
int productOfTwoNumbers(int num1, int num2);
extern int sbox[256];

const int Nr = 10; // number of rounds 
const int Nb = 4;  // number of columns in state matrix
const int Nk = 4;  // number of 32 bit words in a key.