#pragma once
#include <vector>
#include <string>
using namespace std;

void ShiftRows(int ** state, int Nb);
void SubBytes(int ** state, int Nb);
int ** KeyExpansion(int * key, int Nb);
extern int ** sbox;