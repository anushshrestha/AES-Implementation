#include "stdafx.h"
#include <iostream>
#include "stdint.h"
#include <string>
#include <vector>
#include "stdio.h"

using namespace std;

vector<vector<int>> ShiftRows(vector <vector <int>> state, int Nb)
{
	vector <int> temp;
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


