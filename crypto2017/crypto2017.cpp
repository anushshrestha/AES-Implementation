/**
* AES implementation,
* Author : Anush Shrestha, Vaishanvi Batni, Arindam Mitra
* This is an implementation of FIPS 197 (csrc.nist.gov/publications/fips/fips197/fips-197.pdf)
* This has security optimisations and prevention from side channel attack.
*/

#include "stdafx.h"
#include <string>
#include <iostream>
#include "cipher.h"

using namespace std;


// declare all the variables 
//DCL30-C. Declare objects with appropriate storage durations
int cipherkey[16] = { 0 };  // the secret key K initialized to all 0
int iv[16] = { 0 };			// the IV for CBC initialized to all 0

// Cipher is the main function that encrypts the PlainText.
// state: a 4x4 matrix with the plaintext
// At the end the state contains the cipher text
void Cipher(int state[][4]) {
	// expand the key
	int * expandedKeys = KeyExpansion(cipherkey, 128);
	// current round
	uint8_t round = 0;

	// Add the First round key to the state before starting the rounds.
	AddRoundKey(state, expandedKeys, round);

	// There will be Nr rounds.
	// The first Nr-1 rounds are identical.
	// These Nr-1 rounds are executed in the loop below.
	for (round = 1; round < Nr; ++round)
	{
		SubBytes(state);
		ShiftRows(state);
		mixColumns(state);
		AddRoundKey(state, expandedKeys, round);
	}

	// The last round is given below.
	// The MixColumns function is not here in the last round.
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, expandedKeys, Nr);
}

// InvCipher is the main function that decrypts the ciphertext.
// state: a 4x4 matrix with the ciphertext
// At the end the state contains the plain text text
void InvCipher(int state[][4]) {
	// expand the key
	int * expandedKeys = KeyExpansion(cipherkey, 128);
	// current round
	uint8_t round = Nr;

	// Add the First round key to the state before starting the rounds.
	AddRoundKey(state, expandedKeys, round);

	// There will be Nr rounds.
	// The first Nr-1 rounds are identical.
	// These Nr-1 rounds are executed in the loop below.
	for (round = Nr -1; round >=1; --round)
	{
		InvShiftRows(state);
		InvSubBytes(state);
		AddRoundKey(state, expandedKeys, round);
		InvMixColumns(state);
	}

	// The last round is given below.
	// The MixColumns function is not here in the last round.
	InvShiftRows(state);
	InvSubBytes(state);
	AddRoundKey(state, expandedKeys, 0);
}

/** read a 32 character hex string and represent it as a 4x4 matrix */
void convertHexStringToState(char* pliantext, int state[][4]) {
	for (int col = 0; col < Nb; col++) {
		for (int row = 0; row < Nb; row++) {
			// for position state(i,j), compute the value
			char c = pliantext[col * 8 + 2 * row];
			char c2 = pliantext[col * 8 + 2 * row + 1];
			int msb = ('0' <= c&&'9' >= c ? c - '0' : 10 + c - 'a');
			int lsb = ('0' <= c2&&'9' >= c2 ? c2 - '0' : 10 + c2 - 'a');
			state[row][col] = (msb * 16 + lsb);
		}
	}
}

/** convert a 4x4 int matrix to a Hex string of length 32 **/
void convertStateToHexString(int state[][4], char * ciphertext) {
	int pos = 0;
	for (int i = 0; i < Nb; i++) {
		for (int j = 0; j < Nb; j++) {
			int lsb = state[j][i] % 16;
			int msb = state[j][i] / 16;
			ciphertext[pos++] = (msb <= 9 ? '0' : 'a' - 10) + msb;
			ciphertext[pos++] = (lsb <= 9 ? '0' : 'a' - 10) + lsb;
		}
	}
	ciphertext[32] = '\0';
}

// create a key array of length 16 from hex string of 32 characters
void convertHexStringToKey(char * hexkey, int* cipherkey) {
	for (int i = 0; i < 33; i += 2) {
		char c = hexkey[i];
		char c2 = hexkey[i + 1];
		int msb = ('0' <= c&&'9' >= c ? c - '0' : 10 + c - 'a');
		int lsb = ('0' <= c2&&'9' >= c2 ? c2 - '0' : 10 + c2 - 'a');
		cipherkey[i / 2] = (msb * 16 + lsb);
	}
}

// display the 4x4 state in console
void displayState(int state[][4]) {
	cout << endl;
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			cout << hex << state[row][column] << ' ';
		}
		cout << endl;
	}
}



/** CBC implementation starts here**/
// xor the IV with state
void XorWithIv(int state[][4], int *iv) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			state[j][i] ^= iv[i * 4 + j];
		}
	}
}

// conver a 4x4 2d array to 16x1 array
// since we represented state as 4x4 matrix and key as 16x1 array , 
// a conversion is needed for CBC
void convert2Dto1D(int state[][4], int *iv) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			iv[i * 4 + j] = state[j][i];
		}
	}
}

// CBC: for multiple block message
// do not support padding
// input should be of exact multiple of 32, length denotes that ratio
void AES_CBC_encrypt(char* input, int length, char* key, char* IV, char* output) {

	int state[4][4]; // the state matrix
	convertHexStringToKey(key, cipherkey); // read the key
	convertHexStringToKey(IV, iv); // read the IV

	// encrypt and append each block
	for (int i = 0; i < length; i++) {
		// read the next plaintext block
		convertHexStringToState(input, state);
		// xor with iv
		XorWithIv(state, iv);
		// encrypt
		Cipher(state);
		// store in output as hex string
		convertStateToHexString(state, output);
		// update IV
		convert2Dto1D(state, iv);
		// go to next block
		// each block is 32 characters long
		input += 32;
		output += 32;
	}
}

void AES_CBC_decrypt(char* input, int length, char* key, char* IV, char* output) {
	
	int state[4][4]; // the state matrix
	convertHexStringToKey(key, cipherkey); // read the key
	convertHexStringToKey(IV, iv); // read the IV

	// decrypt and append each block
	for (int i = 0; i < length; i++) {
		// read the next ciphertext block
		convertHexStringToState(input, state);
		// decrypt
		InvCipher(state);
		// xor with iv
		XorWithIv(state, iv);
		// store in output as hex string
		convertStateToHexString(state, output);
		// update IV to c_i-1
		convertHexStringToKey(input, iv); 
		// go to next block
		// each block is 32 characters long
		input += 32;
		output += 32;
	}
}

// validation for single block messages
void validateSingleBlockMessage(char* plaintext, char* output, char *key) {
	// declare memory
	int state[4][4];
	char ciphertext[33];
	ciphertext[32] = '\0';

	// convert key
	convertHexStringToKey(key, cipherkey);

	// convert state
	convertHexStringToState(plaintext, state);
	//displayState(state);

	// pass it to cipher
	Cipher(state);
	// convert back the cipher text
	convertStateToHexString(state, ciphertext);

	cout << "Expected: " << output << endl;
	cout << "Computed: " << ciphertext << endl;
	if (strcmp(ciphertext, output) == 0) {
		cout << "test case encryption passed" << endl;
	}
	else {
		cout << "test case encryption failed" << endl;
	}

	//decipher
	InvCipher(state);

	// convert back the plain text
	convertStateToHexString(state, ciphertext);

	cout << "Expected: " << plaintext << endl;
	cout << "Computed: " << ciphertext << endl;
	if (strcmp(ciphertext, plaintext) == 0) {
		cout << "test case decryption passed" << endl;
	}
	else {
		cout << "test case decryption failed" << endl;
	}

}

// test vectors are from NIPS validation suite
void knownAnswerTest() {

	// test case 0
	char key0[33] = "2b7e151628aed2a6abf7158809cf4f3c";
	char tv0[33] = "3243f6a8885a308d313198a2e0370734"; //plaintext
	char to0[33] = "3925841d02dc09fbdc118597196a0b32"; //ciphertext
	validateSingleBlockMessage(tv0, to0, key0);

	//test case 1
	char key1[33] = "00000000000000000000000000000000";
	char tv1[33] = "f34481ec3cc627bacd5dc3fb08f273e6"; //plaintext
	char to1[33] = "0336763e966d92595a567cc9ce537f5e"; //ciphertext
	validateSingleBlockMessage(tv1, to1, key1);

	// test case 2
	char key2[33] = "2b7e151628aed2a6abf7158809cf4f3c";
	char tv2[33] = "6bc1bee22e409f96e93d7e117393172a"; //plaintext
	char to2[33] = "3ad77bb40d7a3660a89ecaf32466ef97"; //ciphertext
	validateSingleBlockMessage(tv2, to2, key2);

	// test case 3
	char key3[33] = "2b7e151628aed2a6abf7158809cf4f3c";
	char tv3[33] = "ae2d8a571e03ac9c9eb76fac45af8e51"; //plaintext
	char to3[33] = "f5d3d58503b9699de785895a96fdbaaf"; //ciphertext
	validateSingleBlockMessage(tv3, to3, key3);

	// test case 4
	char key4[33] = "2b7e151628aed2a6abf7158809cf4f3c";
	char tv4[33] = "30c81c46a35ce411e5fbc1191a0a52ef"; //plaintext
	char to4[33] = "43b1cd7f598ece23881b00e3ed030688"; //ciphertext
	validateSingleBlockMessage(tv4, to4, key4);

	// test case 5
	char key5[33] = "2b7e151628aed2a6abf7158809cf4f3c";
	char tv5[33] = "f69f2445df4f9b17ad2b417be66c3710"; //plaintext
	char to5[33] = "7b0c785e27e8ad3f8223207104725dd4"; //ciphertext
	validateSingleBlockMessage(tv5, to5, key5);
}

// validate CBC
// test vectors are from NIPS validation suite
void multiblockMessageTest() {
	char plaintext[65] = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51";
	char key[33] = "2b7e151628aed2a6abf7158809cf4f3c";
	char iv[33] = "000102030405060708090a0b0c0d0e0f";
	char output[65] = "7649abac8119b246cee98e9b12e9197d5086cb9b507219ee95db113a917678b2";
	char ciphertext[65]; // stores the ouptput
	char deceryptedText[65]; //stores the decrypted message
	ciphertext[64] = '\0';
	deceryptedText[64] = '\0';

	AES_CBC_encrypt(plaintext, 2, key, iv, ciphertext);
	cout << "Expected: " << output << endl;
	cout << "Computed: " << ciphertext << endl;


	if (strcmp(ciphertext, output) == 0) {
		cout << "test case encrytion passed" << endl;
	}
	else {
		cout << "test case encrytion failed" << endl;
	}

	
	AES_CBC_decrypt(ciphertext, 2, key, iv, deceryptedText);
	
	if (strcmp(plaintext, deceryptedText) == 0) {
		cout << "test case decryption  passed" << endl;
	}
	else {
		cout << "test case decryption failed" << endl;
	}

}

// run validation
int main()
{

	// Test cipher key from FIPS 197 Appendix A Page 27
	cout << "Testing single block message" << endl;
	cout << "----------------------------" << endl;
	knownAnswerTest();
	cout << endl << "Testing multiple block message" << endl;
	cout << "----------------------------" << endl;
	multiblockMessageTest();
	std::getchar();
	return 0;
}
