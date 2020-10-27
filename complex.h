/*
 * complex.h
 *
 *  Created on: 6 Nov 2017
 *      Author: ranjani
 */

#ifndef __INCLUDE_COMPLEX_H__
#define __INCLUDE_COMPLEX_H__

#define NO_ETSI_ROUND 1

#include <hifi2/oper_32b_xtensa.h>

#define ROUND_FACTOR (0x1 << 30)

struct fcomplex32 {
	int32_t real;
	int32_t imag;
};

struct fcomplex16 {
	int16_t real;
	int16_t imag;
};

static inline int32_t sat_int32(int64_t x)
{
	if (x > INT32_MAX)
		return INT32_MAX;
	else if (x < INT32_MIN)
		return INT32_MIN;
	else
		return (int32_t)x;
}

inline unsigned int power2round (unsigned int N)
{
	if (N < 0)
		return 0;

	--N;
	N |= N >> 1;
	N = N >> 2;
	N |= N >> 4;
	N |= N >> 8;
	N |= N >> 16;

	return N + 1;
}

/* Complex arithmetic*/

/* complex multiplication */
inline void fcomplex_mult(struct fcomplex32 *A, struct fcomplex32 *B,
		struct fcomplex32 *result)
{
	//printf("A is %d + %di \n", A->real, A->imag);
	//printf("B is %d + %di \n", B->real, B->imag);

	int32_t temp1 = sat_int32(((int64_t)A->real * B->real + ROUND_FACTOR) >> 31);
	int32_t temp2 = sat_int32(((int64_t)A->imag * B->imag + ROUND_FACTOR) >> 31);
	int32_t real = temp1 - temp2;

	int32_t temp3 = sat_int32(((int64_t)A->real * B->imag + ROUND_FACTOR) >> 31);
	int32_t temp4 = sat_int32(((int64_t)A->imag * B->real + ROUND_FACTOR) >> 31);
	int32_t imag = temp3 + temp4;

	result->real = real;
	result->imag = imag;
	//printf("result is %d + %di \n", result->real, result->imag);
}

/* complex multiplication with dual MAC */
inline void fcomplex_mult_opt(struct fcomplex32 *A, struct fcomplex32 *B,
		struct fcomplex32 *result)
{
	ae_int32x2 A_AE = AE_MOVDA32X2(A->real, A->imag);
	ae_int32x2 B_AE = AE_MOVDA32X2(B->real, B->imag);

	ae_int32x2 REAL_AE_TEMP = AE_MULFP32X2RAS(A_AE, B_AE);
	int32_t real = (int32_t)AE_MOVAD32_H(REAL_AE_TEMP) - (int32_t)AE_MOVAD32_L(REAL_AE_TEMP);

	B_AE = AE_MOVDA32X2(B->imag, B->real);
	ae_int32x2 IMAG_AE_TEMP = AE_MULFP32X2RAS(A_AE, B_AE);
	int32_t imag = (int32_t)AE_MOVAD32_H(IMAG_AE_TEMP) + (int32_t)AE_MOVAD32_L(IMAG_AE_TEMP);

	result->real = real;
	result->imag = imag;
}

/* complex addition */
inline void fcomplex_add(struct fcomplex32 *A, struct fcomplex32 *B,
		struct fcomplex32 *result)
{
	result->real = sat_int32(A->real + B->real);
	result->imag = sat_int32(A->imag + B->imag);
}

/* complex addition with intrinsics*/
inline void fcomplex_add_opt(struct fcomplex32 *A, struct fcomplex32 *B,
		struct fcomplex32 *result)
{
	ae_int32x2 A_AE = AE_MOVDA32X2(A->real, A->imag);
	ae_int32x2 B_AE = AE_MOVDA32X2(B->real, B->imag);

	ae_int32x2 AE_TEMP = AE_ADD32S(A_AE, B_AE);

	result->real = (int32_t)AE_MOVAD32_H(AE_TEMP);
	result->imag = (int32_t)AE_MOVAD32_L(AE_TEMP);
}


/* complex subtraction */
inline void fcomplex_sub(struct fcomplex32 *A, struct fcomplex32 *B,
		struct fcomplex32 *result)
{
	result->real = sat_int32(A->real - B->real);
	result->imag = sat_int32(A->imag - B->imag);
}

/* complex subtraction with intrinsics*/
inline void fcomplex_sub_opt(struct fcomplex32 *A, struct fcomplex32 *B,
		struct fcomplex32 *result)
{
	ae_int32x2 A_AE = AE_MOVDA32X2(A->real, A->imag);
	ae_int32x2 B_AE = AE_MOVDA32X2(B->real, B->imag);

	ae_int32x2 AE_TEMP = AE_SUB32S(A_AE, B_AE);

	result->real = (int32_t)AE_MOVAD32_H(AE_TEMP);
	result->imag = (int32_t)AE_MOVAD32_L(AE_TEMP);
}

/* complex conjugate */
inline void fcomplex_conj(struct fcomplex32 *A)
{
	A->imag *= -1;
}

/* complex division (complex number divided by a real number) */
inline void fcomplex_div(struct fcomplex32 *A, float N, struct fcomplex32 *result)
{
	result->real = A->real / N;
	result->imag = A->imag / N;
}

/* bit-wise addition performed left->right to determine the next sample in
 * bit-reversed order
 */
static unsigned int bitwise_add(unsigned int x, unsigned int y)
{
	unsigned int carry = x & y;
	unsigned int sum = x ^ y;
	while (carry != 0) {
		unsigned int shiftedcarry = carry >> 1;
		carry = shiftedcarry & sum;
		sum = sum ^ shiftedcarry;
	}
	return sum;
}


#endif /*  */
