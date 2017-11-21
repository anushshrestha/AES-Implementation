#pragma once
#include <vector>
#include <string>
using namespace std;

void ShiftRows(int state[][4]);
void SubBytes(int state[][4]);
void mixColumns(int state[][4]);
int * KeyExpansion(int * key, int Nb);
void AddRoundKey(int state[][4], int* roundKey, int round);
int position(int value);
extern int sbox[256];
int sBoxGen(int value);
