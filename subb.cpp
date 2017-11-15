#include<iostream>
#include<bitset>
using namespace std;
int irreducable = 0x11B;
int affineConst = 143;
int addVar = 198;
bool firstTime = true;
int sBox[256] = { 0 };
int state[256] = { 0x01, 0x01, 0x02 };
int productInFiniteField(int num1, int num2) {
	bitset<8> num_1 = num1;
	bitset<16> num_2 = num2;
	bitset<16> prod=0;
	int i = 7;
	int product = 0;
	while (i>=0) {
		if (num_1[i] == 1) {
			prod = prod ^ (num_2 << i);
			
		}
		i--;
	}
	return (int)(prod.to_ulong());
}


int position(int value) {
	int count = 8;
	bitset <9> x = value;
	while (count>=0) {
		if (x[count] == 1) {
			return count;
		}
		count--;
	}
	return (count);
}
int mulInverse(int init) {
	if (init == 0) {
		return 0;
	}
	int temp = irreducable;
	int q = 0;
	int prev = 0;
	int present = 1;
	while (temp > 1) {
		int q1 = 1;
		int pos1 = position(init);//finding position of first 1 in the divisor
		int pos2 = position(temp);// finding position of first 1 in the divident

		if (pos1 <= pos2) {//if position of dividend is greater than the position of the divisor, divisor has to be multiplied (or shifted, in case of binary)
			q1 = q1 << (pos2 - pos1); //Finding quotient for the first xor
			int temp2 = init << (pos2 - pos1);
			temp = temp^temp2;
		}
		else {
			int random = prev;
			prev = present;
			present = productInFiniteField(present, q);
			present = random ^ present;
			int temporary = temp;
			temp = init;
			init = temporary;
			q1 = 0;
			q = 0;
		}
		q += q1;
	}
	int random = prev;
	prev = present;
	present = productInFiniteField(present, q);

	present = random ^ present;
	return present;
}
int affineTrans(int k) {
	int i = 0, j = 0;
	bitset<8> prod, finalVal=0;

	bitset<8> affineConstBit = affineConst;
	bitset<8> value = k;
	prod = value;
	for (int i = 0; i < 8; i++) {
		value[7 - i] = prod[i];
	}
	while (i < 8) {
		prod = affineConstBit & value;
		while (j < 8) {
			finalVal[i] = finalVal[i] ^ prod[j];
			j++;
		}

		j = 0;
		int temp = affineConstBit[0];
		affineConstBit=affineConstBit >> 1;
		affineConstBit[7] = temp;
		i++;
	}
	prod = finalVal;
	for (int i = 0; i < 8; i++) {
		finalVal[7 - i] = prod[i];
	}
	bitset<8> addVarBit = addVar;
	finalVal = finalVal ^ addVarBit ;
	prod = finalVal;
	for (int i = 0; i < 8; i++) {
		finalVal[7 - i] = prod[i];
	}
	return (int)(finalVal.to_ulong());
}
void byteSub() {
	for (int i = 0; i < 256; i++) {
		if (i % 16 == 0) {
			cout << endl;
		}
		state[i] = affineTrans(mulInverse(i));
		cout << std::hex << state[i] << " ";
	}
}

int main() {
	int k = mulInverse(0x10);
	bitset<8> kk = k;
	int r = affineTrans(k);
	int m;
	m = 44;
	byteSub();
	cin >> m;
	return 0;
};