#ifndef __BLURHASH_COMMON_H__
#define __BLURHASH_COMMON_H__

/* common.h
 * This file is part of the blurhash distribution (https://github.com/fumiama/blurhash).
 * Copyright (c) 2018 Wolt Enterprises and Copyright (c) 2025 Fumiama Minamoto.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include<math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static char blurhash_base83_table[83]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz#$%*+,-.:;=?@[]^_{|}~";

static inline char *blurhash_base83_encode_int(int value, int length, char *destination) {
	int divisor = 1;
	for(int i = 0; i < length - 1; i++) divisor *= 83;

	for(int i = 0; i < length; i++) {
		int digit = (value / divisor) % 83;
		divisor /= 83;
		*destination++ = blurhash_base83_table[digit];
	}
	return destination;
}

static inline int blurhash_base83_decode_int(const char * string, int start, int end) {
	int value = 0, iter1 = 0, iter2 = 0;
	for( iter1 = start; iter1 < end; iter1 ++) {
		int index = -1;
		for(iter2 = 0; iter2 < 83; iter2 ++) {
			if (blurhash_base83_table[iter2] == string[iter1]) {
				index = iter2;
				break;
			}
		}
		if (index == -1) return -1;
		value = value * 83 + index;
	}
	return value;
}

static inline int blurhash_linearTosRGB(float value) {
	float v = fmaxf(0, fminf(1, value));
	if(v <= 0.0031308) return v * 12.92 * 255 + 0.5;
	else return (1.055 * powf(v, 1 / 2.4) - 0.055) * 255 + 0.5;
}

static inline float blurhash_sRGBToLinear(int value) {
	float v = (float)value / 255;
	if(v <= 0.04045) return v / 12.92;
	else return powf((v + 0.055) / 1.055, 2.4);
}

static inline float blurhash_signPow(float value, float exp) {
	return copysignf(powf(fabsf(value), exp), value);
}

#endif
