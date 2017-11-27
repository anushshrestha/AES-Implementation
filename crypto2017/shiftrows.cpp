#include "stdafx.h"
#include <iostream>
#include "stdint.h"
#include <string>
#include <vector>
#include "stdio.h"
#include "cipher.h"
using namespace std;

// Shift row r of state S left by r bytes
void ShiftRows(int state[][4])
{
	int Nb = 4;
	int temp[4];
	// No need to shift first row.
	for (int row = 1; row < Nb; row++) {
		for (int column = 0; column < Nb; column++) {
			// As column value iterates, the required sequence is generated
			// 0 1 2 3
			// 1 2 3 0
			// 2 3 0 1
			// 3 0 1 2
			temp[column] = state[row][(column + row) % Nb];
		}
		for (int column = 0; column < Nb; column++) {
			state[row][column] = temp[column];
		}
	}
}

// Shift row r of state S right by r bytes
void InvShiftRows(int state[][4])
{
	int Nb = 4;
	int temp[4];
	// No need to shift first row.
	for (int row = 1; row < Nb; row++) {
		for (int column = 0; column < Nb; column++) {
			// As column value iterates, the required sequence is generated
			// 0 1 2 3
			// 3 0 1 2
			// 2 3 0 1
			// 1 2 3 0
			temp[column] = state[row][(column + Nb - row) % Nb];
		}
		for (int column = 0; column < Nb; column++) {
			state[row][column] = temp[column];
		}
	}
}

