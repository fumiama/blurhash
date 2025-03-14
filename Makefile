PROGRAM=blurhash_encoder
DECODER=blurhash_decoder
$(PROGRAM): encode_stb.c encode.c blurhash.h common.h
	$(CC) -o $@ encode_stb.c encode.c -lm -lstb -Ofast

$(DECODER): decode_stb.c decode.c blurhash.h common.h
	$(CC) -o $(DECODER) decode_stb.c decode.c -lm -lstb -Ofast

.PHONY: clean
clean:
	rm -f $(PROGRAM)
	rm -f $(DECODER)
