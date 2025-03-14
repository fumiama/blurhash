/* decode.c
 * This file is part of the blurhash distribution (https://github.com/woltapp/blurhash).
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

#include <stb/stb_image_write.h>

#include "blurhash.h"
#include "common.h"

static inline uint8_t blurhash_clampToUByte(int s) {
	if (s < 0) return 0;
	if (s>>8) return 255;
	return (uint8_t)s;
}

bool blurhash_is_valid(const char * blurhash) {

	const int hashLength = strlen(blurhash);

	if ( !blurhash || strlen(blurhash) < 6) return false;

	int sizeFlag = blurhash_base83_decode_int(blurhash, 0, 1);	//Get size from first character
	int numY = (int)floorf(sizeFlag / 9) + 1;
	int numX = (sizeFlag % 9) + 1;

	if (hashLength != 4 + 2 * numX * numY) return false;
	return true;
}

void decodeDC(int value, float * r, float * g, float * b) {
	*r = blurhash_sRGBToLinear(value >> 16); 	// R-component
	*g = blurhash_sRGBToLinear((value >> 8) & 255); // G-Component
	*b = blurhash_sRGBToLinear(value & 255);	// B-Component
}

void decodeAC(int value, float maximumValue, float * r, float * g, float * b) {
	int quantR = (int)floorf(value / (19 * 19));
	int quantG = (int)floorf(value / 19) % 19;
	int quantB = (int)value % 19;

	*r = blurhash_signPow(((float)quantR - 9) / 9, 2.0) * maximumValue;
	*g = blurhash_signPow(((float)quantG - 9) / 9, 2.0) * maximumValue;
	*b = blurhash_signPow(((float)quantB - 9) / 9, 2.0) * maximumValue;
}

int blurhash_decode2(const char * blurhash, int width, int height, int punch, int nChannels, uint8_t * pixelArray) {
	if (! blurhash_is_valid(blurhash)) return -1;
	if (punch < 1) punch = 1;

	int sizeFlag = blurhash_base83_decode_int(blurhash, 0, 1);
	int numY = (int)floorf(sizeFlag / 9) + 1;
	int numX = (sizeFlag % 9) + 1;
	int iter = 0;

	float r = 0, g = 0, b = 0;
	int quantizedMaxValue = blurhash_base83_decode_int(blurhash, 1, 2);
	if (quantizedMaxValue == -1) return -1;

	float maxValue = ((float)(quantizedMaxValue + 1)) / 166;

	int colors_size = numX * numY;
	float colors[colors_size][3];

	for(iter = 0; iter < colors_size; iter ++) {
		if (iter == 0) {
			int value = blurhash_base83_decode_int(blurhash, 2, 6);
			if (value == -1) return -1;
			decodeDC(value, &r, &g, &b);
			colors[iter][0] = r;
			colors[iter][1] = g;
			colors[iter][2] = b;

		} else {
			int value = blurhash_base83_decode_int(blurhash, 4 + iter * 2, 6 + iter * 2);
			if (value == -1) return -1;
			decodeAC(value, maxValue * punch, &r, &g, &b);
			colors[iter][0] = r;
			colors[iter][1] = g;
			colors[iter][2] = b;
		}
	}

	int bytesPerRow = width * nChannels;
	int x = 0, y = 0, i = 0, j = 0;
	int intR = 0, intG = 0, intB = 0;

	for(y = 0; y < height; y ++) {
		for(x = 0; x < width; x ++) {

			float r = 0, g = 0, b = 0;

			for(j = 0; j < numY; j ++) {
				for(i = 0; i < numX; i ++) {
					float basics = cos((M_PI * x * i) / width) * cos((M_PI * y * j) / height);
					int idx = i + j * numX;
					r += colors[idx][0] * basics;
					g += colors[idx][1] * basics;
					b += colors[idx][2] * basics;
				}
			}

			pixelArray[nChannels * x + 0 + y * bytesPerRow] = blurhash_clampToUByte(blurhash_linearTosRGB(r));
			pixelArray[nChannels * x + 1 + y * bytesPerRow] = blurhash_clampToUByte(blurhash_linearTosRGB(g));
			pixelArray[nChannels * x + 2 + y * bytesPerRow] = blurhash_clampToUByte(blurhash_linearTosRGB(b));

			if (nChannels == 4)
				pixelArray[nChannels * x + 3 + y * bytesPerRow] = 255;   // If nChannels=4, treat each pixel as RGBA instead of RGB

		}
	}

	return 0;
}

pixel_array_t blurhash_decode(const char * blurhash, int width, int height, int punch, int nChannels) {
	int bytesPerRow = width * nChannels;
	pixel_array_t pixelArray = blurhash_create_pixel_array(bytesPerRow * height);

	if (blurhash_decode2(blurhash, width, height, punch, nChannels, pixelArray) == -1)
		return NULL;
	return pixelArray;
}

int blurhash_decode_file(const char* hash, int width, int height, int punch, int nChannels, const char *filename) {
	pixel_array_t bytes = blurhash_decode(hash, width, height, punch, nChannels);

	if (!bytes) return -1;

	if (stbi_write_png(filename, width, height, nChannels, bytes, nChannels * width) == 0)
		return -2;

	blurhash_free_pixel_array(bytes);

	return 0;
}
