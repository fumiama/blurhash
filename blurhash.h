#ifndef __BLURHASH_BLURHASH_H__

#define __BLURHASH_BLURHASH_H__

/* blurhash.h
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

#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// pixel_array_t means this array need to be freed by caller using
// function blurhash_free_pixel_array
typedef uint8_t* pixel_array_t;

const char *blurhash_encode(int xComponents, int yComponents, int width, int height, uint8_t *rgb, size_t bytesPerRow);

/*
	blurhash_decode : Returns the pixel array of the result image given the blurhash string,
	Parameters : 
		blurhash : A string representing the blurhash to be decoded.
		width : Width of the resulting image
		height : Height of the resulting image
		punch : The factor to improve the contrast, default = 1
		nChannels : Number of channels in the resulting image array, 3 = RGB, 4 = RGBA
	Returns : A pointer to memory region where pixels are stored in (H, W, C) format
*/
pixel_array_t blurhash_decode(const char * blurhash, int width, int height, int punch, int nChannels);

/*
	blurhash_decode2 : Decodes the blurhash and copies the pixels to pixelArray,
					This method is suggested if you use an external memory allocator for pixelArray.
					pixelArray should be of size : width * height * nChannels
	Parameters :
		blurhash : A string representing the blurhash to be decoded.
		width : Width of the resulting image
		height : Height of the resulting image
		punch : The factor to improve the contrast, default = 1
		nChannels : Number of channels in the resulting image array, 3 = RGB, 4 = RGBA
		pixelArray : Pointer to memory region where pixels needs to be copied.
	Returns : int, -1 if error 0 if successful
*/
int blurhash_decode2(const char * blurhash, int width, int height, int punch, int nChannels, uint8_t * pixelArray);

/*
	blurhash_is_valid : Checks if the Blurhash is valid or not.
	Parameters :
		blurhash : A string representing the blurhash
	Returns : bool (true if it is a valid blurhash, else false)
*/
bool blurhash_is_valid(const char * blurhash);

// blurhash_create_pixel_array malloc size bytes uint8_t array.
static inline pixel_array_t blurhash_create_pixel_array(int size) {
	return (pixel_array_t)malloc(size * sizeof(uint8_t));
}

/*
	blurhash_free_pixel_array : Frees the pixel array
	Parameters :
		pixelArray : Pixel array pointer which will be freed.
	Returns : void (None)
*/
void blurhash_free_pixel_array(pixel_array_t pixelArray) {
	if (pixelArray) {
		free(pixelArray);
	}
}

#endif
