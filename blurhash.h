#ifndef __BLURHASH_BLURHASH_H__
#define __BLURHASH_BLURHASH_H__

/* blurhash.h
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

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum blurhash_error_t {
	blurhash_error_ok,
	blurhash_error_invalid_x_components,
	blurhash_error_invalid_y_components,
	blurhash_error_stbi_load,
	blurhash_error_invalid_hash,
	blurhash_error_invalid_decode_quantized_max_value,
	blurhash_error_invalid_decode_dc,
	blurhash_error_invalid_decode_ac,
	blurhash_error_stbi_write_png
};
typedef enum blurhash_error_t blurhash_error_t;

// BLURHASH_ENCODE_BUFSZ defines maximum buffer size for hash
#define BLURHASH_ENCODE_BUFSZ (2 + 4 + (9 * 9 - 1) * 2 + 1)

// BLURHASH_ENCODE_BUFSZ defines maximum buffer size for image buffer
#define BLURHASH_DECODE_BUFSZ(width, height, nChannels) (width * height * nChannels)


/**
 * @brief encodes blurhash from filename to buffer.
 * @param xComponents range [1, 9]
 * @param yComponents range [1, 9]
 * @param filename valid file path
 * @param buffer must >= `BLURHASH_ENCODE_BUFSZ`
 * @return success is `0`, print others by `blurhash_perror`
*/
blurhash_error_t blurhash_encode_file(int xComponents, int yComponents, const char *filename, char* buffer);

/**
 * @brief encodes blurhash from rgb image bytes to buffer.
 * @param xComponents range [1, 9]
 * @param yComponents range [1, 9]
 * @param width image width pixels
 * @param height image height pixels
 * @param rgb [3][width][height]
 * @param buffer must >= `BLURHASH_ENCODE_BUFSZ`
 * @return success is `0`, print others by `blurhash_perror`
*/
blurhash_error_t blurhash_encode(int xComponents, int yComponents, int width, int height, const uint8_t *rgb, char* buffer);


/**
 * @brief decodes the blurhash and copies the pixels to buffer.
 * @param blurhash a string representing the blurhash to be decoded
 * @param width of the resulting image
 * @param height of the resulting image
 * @param punch the factor to improve the contrast, default = 1
 * @param nChannels number of channels in the resulting image array, 3 = RGB, 4 = RGBA
 * @param buffer must >= `BLURHASH_DECODE_BUFSZ`
 * @return success is `0`, print others by `blurhash_perror`
*/
blurhash_error_t blurhash_decode(const char * blurhash, int width, int height, int punch, int nChannels, uint8_t* buffer);

/**
 * @brief decodes the blurhash into filename.
 * @param blurhash a string representing the blurhash to be decoded
 * @param width of the resulting image
 * @param height of the resulting image
 * @param punch the factor to improve the contrast, default = 1
 * @param nChannels number of channels in the resulting image array, 3 = RGB, 4 = RGBA
 * @param filename valid file path
 * @param buffer must >= `BLURHASH_DECODE_BUFSZ`
 * @return success is `0`, print others by `blurhash_perror`
*/
blurhash_error_t blurhash_decode_file(const char* blurhash, int width, int height, int punch, int nChannels, const char *filename, uint8_t* buffer);


/**
 * @brief checks if the blurhash is valid or not.
 * @param blurhash astring representing the blurhash
 * @return bool (`true` if it is a valid blurhash, else `false`)
*/
bool blurhash_is_valid(const char * blurhash);

/**
 * @brief call perror on error
 * @param err the error
 * @return the input parameter `err`
*/
static inline blurhash_error_t blurhash_perror(blurhash_error_t err) {
	#define blurhash_perror_case(n) case blurhash_error_##n: perror("blurhash_error_"#n)
		if(err) switch(err) {
			blurhash_perror_case(invalid_x_components); break;
			blurhash_perror_case(invalid_y_components); break;
			blurhash_perror_case(stbi_load); break;
			blurhash_perror_case(invalid_hash); break;
			blurhash_perror_case(invalid_decode_quantized_max_value); break;
			blurhash_perror_case(invalid_decode_dc); break;
			blurhash_perror_case(invalid_decode_ac); break;
			blurhash_perror_case(stbi_write_png); break;
			default: perror("blurhash"); break;
		}
	#undef blurhash_perror_case
	return err;
}

#endif
