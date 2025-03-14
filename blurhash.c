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
        case 'e':
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
        break;
        case 'd':
            if(argc < 6) {
                print_usage(argv[0]);
                return 1;
            }

            int width, height, punch = 1;
            char * hash = argv[2];
            width = atoi(argv[3]);
            height = atoi(argv[4]);
            char * output_file = argv[5];
        
            const int nChannels = 4;
        
            if(argc == 7)
                punch = atoi(argv[6]);

            if (blurhash_decode_file(hash, width, height, punch, nChannels, output_file)) {
                perror("blurhash_decode_file");
            }
        break;
        default:
            print_usage(argv[0]);
            return 1;
        break;
    }

	return 0;
}

