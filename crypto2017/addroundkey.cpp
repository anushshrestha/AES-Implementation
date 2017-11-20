#include <string>
#include <vector>
#include "stdio.h"
#include "stdafx.h"
#include "stdint.h"
#include <iostream>
#include "cipher.h"

#define NB 4
void AddRoundKey(int state[][4], int * roundKey, int round) {

	for (int i = 0; i<NB; ++i)
	{
		for (int j = 0; j < NB; ++j)
		{
			state[i][j] ^= roundKey[round * NB * 4 + i * NB + j];
		}
	}
}