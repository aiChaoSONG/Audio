/*
 * Copyright (c) 2016, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the Intel Corporation nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Ranjani Sridharan <ranjani.sridharan@linux.intel.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "twiddle.h"
#include "complex.h"
#include "fft.h"
#include "input.h"

#define NUM_ITERATIONS 100
#define SAMPLE_SIZE 256
#define WRITE_OUTPUT 0

int main(int argc, char *argv[])
{
	int32_t num;
	FILE *fft_file;
	unsigned int sample_length, iterations, i = 0, k = 0, input_downscale;

	if ((SAMPLE_SIZE & (SAMPLE_SIZE - 1)) == 0) {
		sample_length = SAMPLE_SIZE;
	} else {
		sample_length = power2round(SAMPLE_SIZE);
	}
	struct fcomplex32 in_samples[sample_length], fft_out[sample_length],
			ifft_out[sample_length];

	input_downscale = log2(sample_length);

	for (i = 0; i < SAMPLE_SIZE; i++) {
		in_samples[i].real = data[i];
		in_samples[i].imag = 0;
	}

	/* Pad with zeros */
	if(i < sample_length)
		for (; i < sample_length; i++) {
			in_samples[i].real = 0;
			in_samples[i].imag = 0;
		}

	for (iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
		fft_opt1(in_samples, fft_out, sample_length);
#if WRITE_OUTPUT
		fft_file = fopen(argv[2], "w+");
		for (i = 0; i < sample_length; i++) {
			fprintf(fft_file,"(%d, %d)\n",fft_out[i].real,
					fft_out[i].imag);
		}
		fclose(fft_file);
#endif
	}
	printf("Real Part***************************\n");
	for (i = 0; i < SAMPLE_SIZE; i++) {
		printf("%d\n", (int) ( (float)(fft_out[i].real)*(float)0x7FFFFFFF ));
	}
	printf("Image Part******************************\n");
	for (i = 0; i < SAMPLE_SIZE; i++) {
		printf("%d\n", (int) ( (float)(fft_out[i].imag)*(float)0x7FFFFFFF ));
	}
#if 0
	for (iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
		ifft(fft_out, ifft_out, sample_length, num_stages);
		ifft_file = fopen(argv[3], "w+");
		for (i = 0; i < sample_length; i++) {
			fprintf(ifft_file,"(%f, %f)\n",ifft[i].real,
					ifft[i].imag);
		}
		fclose(ifft_file);
	}
#endif

	return 0;
}
