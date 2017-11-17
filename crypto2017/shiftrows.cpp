#include "stdafx.h"
#include <iostream>
#include "stdint.h"
#include <string>
#include <vector>
#include "stdio.h"
using namespace std;

void ShiftRows(int state[][4])
{
	int Nb = 4;
	int temp[4];
	// No need to shift first row.
	for (int row = 1; row < Nb; row++) {
		for (int column = 0; column < Nb; column++) {
			// As column value iterates, the sequence starts with 1,2,3
			temp[column] = state[row][(column + row) % Nb];
		}
		for (int column = 0; column < Nb; column++) {
			state[row][column] = temp[column];
		}
	}
}


