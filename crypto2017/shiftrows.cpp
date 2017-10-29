#include "stdafx.h"
#include <iostream>
#include "stdint.h"
#include <string>
#include <vector>
#include "stdio.h"

using namespace std;

vector<vector<string>> ShiftRows(vector <vector <string>> state, int Nb)
{
	vector <string> temp;
	for (int row = 1; row < Nb; row++) {
		for (int column = 0; column < Nb; column++) {
			temp.push_back(state[row][(column+row)%Nb]);
		}
		for (int column = 0; column < Nb; column++) {
			state[row][column] = temp[column];
		}
		temp.clear();
	}
	return state;
}


