
/*!	\file omea.cpp

	Implimentation of the 'One More Encryption Algorithm' based on an
	iterative Fiestel cipher.

	Written by:	Peter de Zwart.
	Username:	pntdz31
	Student #:	9840642

	Use doxygen to extract comments as HTML.
*/

#include <assert.h>

/*!	UINT

	Type definition of an unsigned int to reduce code verbage.
*/
typedef unsigned int UINT;

/*!	UBYTE
	
	Type definition of an unsiged char to reduce code verbage.
*/
typedef unsigned char UBYTE;

/*!	LEN

	This is the length of arrays used to express 32 bits as a component
	bytes.
*/
static const UINT	LEN = sizeof(UINT) / sizeof(UBYTE);

/*!	BYTE

	A byte of data with all the bits set to 1.
*/
static const UINT	BYTE = 0xFF;

/*!	S(UINT Xi)

	Replicates the functionality of an Rijndael S box by using table
	substituion.

	\param Xi	The byte component of X to scramble.
	\return		Scrambled Xi.
*/
static UINT S(UINT Xi) {
	static const UINT S_len = 256;
	static const UINT S[S_len] = {
		99, 124, 119, 123, 242, 107, 111, 197, 48, 1, 103, 43, 254, 215, 171, 118,
		202, 130, 201, 125, 250, 89, 71, 240, 173, 212, 162, 175, 156, 164, 114, 192,
		183, 253, 147, 38, 54, 63, 247, 204, 52, 165, 229, 241, 113, 216, 49, 21,
		4, 199, 35, 195, 24, 150, 5, 154, 7, 18, 128, 226, 235, 39, 178, 117,
		9, 131, 44, 26, 27, 110, 90, 160, 82, 59, 214, 179, 41, 227, 47, 132,
		83, 209, 0, 237, 32, 252, 177, 91, 106, 203, 190, 57, 74, 76, 88, 207,
		208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2, 127, 80, 60, 159, 168,
		81, 163, 64, 143, 146, 157, 56, 245, 188, 182, 218, 33, 16, 255, 243, 210,
		205, 12, 19, 236, 95, 151, 68, 23, 196, 167, 126, 61, 100, 93, 25, 115,
		96, 129, 79, 220, 34, 42, 144, 136, 70, 238, 184, 20, 222, 94, 11, 219,
		224, 50, 58, 10, 73, 6, 36, 92, 194, 211, 172, 98, 145, 149, 228, 121,
		231, 200, 55, 109, 141, 213, 78, 169, 108, 86, 244, 234, 101, 122, 174, 8,
		186, 120, 37, 46, 28, 166, 180, 198, 232, 221, 116, 31, 75, 189, 139, 138,
		112, 62, 181, 102, 72, 3, 246, 14, 97, 53, 87, 185, 134, 193, 29, 158,
		225, 248, 152, 17, 105, 217, 142, 148, 155, 30, 135, 233, 206, 85, 40, 223,
		140, 161, 137, 13, 191, 230, 66, 104, 65, 153, 45, 15, 176, 84, 187, 22
	};
	
	assert(Xi >= 0 && Xi < S_len);

	return(S[Xi]);
}

/*!	split(UINT n, UINT *N)

	Take a word and express it as an array of bytes.

	\param n	The word to split.
	\param N	The word split in to an array of component bytes.
*/
static void split(UINT n, UINT *N) {
	UINT	i;

	for (i = 0; i < LEN; i++) {
		N[i] = (n & (BYTE << (i * 8))) >> (i * 8);
	}
}

/*!	splice(UINT *N)

	Given a word expressed as any array of bytes, combine them to form
	the word.

	\param N	A word, expressed as an array of component bytes.
	\return		The word after assembling.
*/
static UINT splice(UINT *N) {
	UINT	n = 0;
	UINT	i;

	for (i = 0; i < LEN; i++) {
		n += N[i] << (i * 8);
	}

	return(n);
}

/*!	F(UINT x, UINT k)

	This is the round function that get's called to munge the left hand
	side of the 64 bit block.

	\param	x	The 32 bit block of data to munge.
	\param	k	A 32 bit key to perturb the data with.
	\return		The munged 32 bits.
*/
static UINT F(UINT x, UINT k) {
	UINT	X[LEN] = { 0 };
	UINT	K[LEN] = { 0 };
	UINT	Y[LEN] = { 0 };
	UINT	Z[LEN] = { 0 };

	UINT	i;

	split(x, X);
	split(k, K);

	for (i = 0; i < LEN; i++) {
		Y[i] = S(X[i]);
	}

	for (i = 0; i < LEN; i++) {
		Z[i] = Y[i] + Y[i + 1 % LEN] + K[i] % BYTE;
	}

	return(splice(Z));
}

/*!	Swap(UINT &l, UINT &r)

	Swaps two unsigned ints using funky XOR swap.

	\param	l	Left operand.
	\param	r	Right operand.
*/
static void Swap(UINT &l, UINT &r) {
	l ^= r ^= l ^= r;
}

/*!	Round(UINT &l, UINT &r, UINT k)

	Feistel Round function.

	\param	l	Left operand.
	\param	r	Right operand.
	\param	k	Sub Key.
*/
static void Round(UINT &l, UINT &r, UINT k) {
	l ^= F(l, k);

	Swap(l, r);
}

/*!	main(int argc, char **argv)

	Test harness.

	\param	argc	Argument counter.
	\param	argv	Vector of arguments as character strings.
	\return		Exit code.
*/
int main(int argc, char **argv) {
	return(0);
}
