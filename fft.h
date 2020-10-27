/*
 * fft.h
 *
 *  Created on: 6 Nov 2017
 *      Author: ranjani
 */

#ifndef __INCLUDE_FFT_H__
#define __INCLUDE_FFT_H__

/* compute FFT */
static void fft(struct fcomplex32 *in_samples, struct fcomplex32 *out_samples,
		unsigned int sample_length)
{
	unsigned int i = 0, next_sample  = 0, j, k,
		half_sample_length = sample_length >> 1, twiddle_start = 0, o;
	struct fcomplex32 temp;

	unsigned int t = 0, b;

	/* 1st stage */
	for (i = 0; i < sample_length; i += 2) {
		b = bitwise_add(t,half_sample_length);
		fcomplex_add(&in_samples[t],
			&in_samples[b],
			&out_samples[i]);

		fcomplex_sub(&in_samples[t],
			&in_samples[b],
			&out_samples[i + 1]);

		t = bitwise_add(b,half_sample_length);
	}
#if 0
	for (o = 0; o < sample_length; o++) {
		printf("out sample after 1st stage is %d + %di\n", out_samples[o].real, out_samples[o].imag);
	}
#endif
	twiddle_start = 1;

	unsigned int b_size = 2;
	unsigned int half_b = 1;
	/* loop for each consecutive butterfly stages in the FFT */
	for (i = sample_length; i > 2; i >>= 1) {
		half_b = b_size;
		b_size <<= 1; // butterfly size
		struct fcomplex32 twiddle[half_b];

		//printf("butter size is %d %d\n", b_size, half_b);

		/* compute twiddle factors */
		for (k = 0; k < half_b; k++) {
			twiddle[k].real = twiddle_real[twiddle_start + k];
			twiddle[k].imag = twiddle_imag[twiddle_start + k];
#if 0
			printf("twiddle[%d] is %d + %di \n", k, twiddle[k].real, twiddle[k].imag);
#endif
		}
		twiddle_start += half_b;

		/* k'th output sample position in
		 * jth butterfly */
		unsigned int top_sample_pos = 0;
		/* (k + half_b)th output sample pos
		* in jth butterfly */
		unsigned int bottom_sample_pos;

		/* loop for each butterfly in the current stage */
		for (j = 0; j < sample_length; j += b_size) {
			/* compute the output for each butterfly */
			for (k = 0; k < half_b; k++) {
				top_sample_pos = k + j;
				bottom_sample_pos = k + j + half_b;

				/* multiply bottom half of the butterfly
				 * by twiddle factor */
				fcomplex_mult(&out_samples[bottom_sample_pos],
					&twiddle[k],
					&out_samples[bottom_sample_pos]);

				fcomplex_add(&out_samples[top_sample_pos],
					&out_samples[bottom_sample_pos],
					&temp);

				fcomplex_sub(&out_samples[top_sample_pos],
					&out_samples[bottom_sample_pos],
					&out_samples[bottom_sample_pos]);

				out_samples[top_sample_pos].real = temp.real;
				out_samples[top_sample_pos].imag = temp.imag;
			}
		}
#if 0
		for (o = 0; o < sample_length; o++) {
			printf("out sample after stage i is %d + %di\n", out_samples[o].real, out_samples[o].imag);
		}
#endif
	}

}

/* compute FFT */
static void fft_opt1(struct fcomplex32 *in_samples, struct fcomplex32 *out_samples,
		unsigned int sample_length)
{
	unsigned int i = 0, next_sample  = 0, j, k,
		half_sample_length = sample_length >> 1, twiddle_start = 0, o;
	struct fcomplex32 temp;

	unsigned int t = 0, b;

	/* 1st stage */
	for (i = 0; i < sample_length; i += 2) {
		b = bitwise_add(t,half_sample_length);
		fcomplex_add_opt(&in_samples[t],
			&in_samples[b],
			&out_samples[i]);

		fcomplex_sub_opt(&in_samples[t],
			&in_samples[b],
			&out_samples[i + 1]);

		t = bitwise_add(b,half_sample_length);
	}
#if 0
	for (o = 0; o < sample_length; o++) {
		printf("out sample after 1st stage is %d + %di\n", out_samples[o].real, out_samples[o].imag);
	}
#endif
	twiddle_start = 1;

	unsigned int b_size = 2;
	unsigned int half_b = 1;
	/* loop for each consecutive butterfly stages in the FFT */
	for (i = sample_length; i > 2; i >>= 1) {
		half_b = b_size;
		b_size <<= 1; // butterfly size
		struct fcomplex32 twiddle[half_b];

		//printf("butter size is %d %d\n", b_size, half_b);

		/* compute twiddle factors */
		for (k = 0; k < half_b; k++) {
			twiddle[k].real = twiddle_real[twiddle_start + k];
			twiddle[k].imag = twiddle_imag[twiddle_start + k];
#if 0
			printf("twiddle[%d] is %d + %di \n", k, twiddle[k].real, twiddle[k].imag);
#endif
		}
		twiddle_start += half_b;

		/* k'th output sample position in
		 * jth butterfly */
		unsigned int top_sample_pos = 0;
		/* (k + half_b)th output sample pos
		* in jth butterfly */
		unsigned int bottom_sample_pos;

		/* loop for each butterfly in the current stage */
		for (j = 0; j < sample_length; j += b_size) {
			/* compute the output for each butterfly */
			for (k = 0; k < half_b; k++) {
				top_sample_pos = k + j;
				bottom_sample_pos = k + j + half_b;

				/* multiply bottom half of the butterfly
				 * by twiddle factor */
				fcomplex_mult_opt(&out_samples[bottom_sample_pos],
					&twiddle[k],
					&out_samples[bottom_sample_pos]);

				fcomplex_add_opt(&out_samples[top_sample_pos],
					&out_samples[bottom_sample_pos],
					&temp);

				fcomplex_sub_opt(&out_samples[top_sample_pos],
					&out_samples[bottom_sample_pos],
					&out_samples[bottom_sample_pos]);

				out_samples[top_sample_pos].real = temp.real;
				out_samples[top_sample_pos].imag = temp.imag;
			}
		}
#if 0
		for (o = 0; o < sample_length; o++) {
			printf("out sample after stage i is %d + %di\n", out_samples[o].real, out_samples[o].imag);
		}
#endif
	}

}

/*compute IFFT */
static void ifft(struct fcomplex32 *input, struct fcomplex32 *output,
		int sample_length, int num_stages)
{
	int i = 0;

	for (i = 0; i < sample_length; i++) {
		complex_conj(&input[i]);
	}

	FFT(input, output, sample_length, num_stages);

	for (i = 0; i < sample_length; i++) {
		fcomplex_conj(&output[i]);
		fcomplex_div(&output[i], sample_length, &output[i]);
	}
}




#endif /*  */
