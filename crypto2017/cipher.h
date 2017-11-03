#pragma once
#include <vector>
#include <string>
using namespace std;

vector<vector<int>> ShiftRows(vector <vector <int>> state, int Nb);
vector<vector<int>> SubBytes(vector <vector <int>> state, int Nb);
vector<vector<int>> KeyExpansion(vector <int> key, int Nb);
extern vector <vector <int>> sbox;