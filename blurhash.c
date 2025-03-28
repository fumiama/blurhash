/* blurhash.c
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

#include <stdio.h>

#include "blurhash.h"

static int print_usage(const char* cmd) {
    fprintf(stderr,
        "Usage: %s [e|d]\n"
        "  e(encode): x_components y_components imagefile\n"
        "  d(decode): hash width height output_file [punch]\n"
    , cmd);
}

int main(int argc, const char **argv) {
	if(argc < 5) {
		print_usage(argv[0]);
		return 1;
	}

    switch (argv[1][0]) {
        case 'e': {
            int xComponents = atoi(argv[2]);
            int yComponents = atoi(argv[3]);
            if(xComponents < 1 || xComponents > 8 || yComponents < 1 || yComponents > 8) {
                fprintf(stderr, "Component counts must be between 1 and 8.\n");
                return 1;
            }

            const char *hash = blurhash_encode_file(xComponents, yComponents, argv[4]);
            if(!hash) {
                fprintf(stderr, "Failed to load image file \"%s\".\n", argv[4]);
                return 1;
            }

            printf("%s\n", hash);
        }
        break;
        case 'd': {
            if(argc < 6) {
                print_usage(argv[0]);
                return 1;
            }

            int width, height, punch = 1;
            const char * hash = argv[2];
            width = atoi(argv[3]);
            height = atoi(argv[4]);
            const char * output_file = argv[5];

            const int nChannels = 4;
        
            if(argc == 7)
                punch = atoi(argv[6]);

            if (blurhash_decode_file(hash, width, height, punch, nChannels, output_file)) {
                perror("blurhash_decode_file");
            }
        }
        break;
        default:
            print_usage(argv[0]);
            return 1;
        break;
    }

	return 0;
}
