#pragma once
#include <vector>
#include <string>
using namespace std;

void ShiftRows(int state[][4]);
void SubBytes(int state[][4]);
int * KeyExpansion(int * key, int Nb);
extern int sbox[256];