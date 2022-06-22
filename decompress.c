/*

Not implemented yet

Currently using zlib's decompress (sane)





uint8_t getCM(uint8_t b){
	uint8_t r = b<<4;
	return r>>4;
}

uint8_t getCINFO(uint8_t b){
	return b>>4;
}

bool validateCMFFLG(uint8_t CMF, uint8_t FLG){
	uint16_t CMFFLGcheck = (CMF<<8) + FLG;
	return getCM(CMF) == 8 && CMFFLGcheck%31 == 0;
}

typedef bool bit;


bool getBit(int n, int bit){
	return n & (1 << bit);
}

void printBinary(uint8_t n){
    for(int i = 7; i >= 0; --i)
        printf("%d", getBit(n, i));
}

uint8_t reverseBits(uint8_t n){
	uint8_t r = 0;
	for(int i = 0; i < 8; ++i)
		r |= ((bool)(n&(1<<(7-i))))<<i;
	return r;
}

uint8_t read8(bit *data, uint32_t *i){
	// This function is used for decompression
	// The first bit is the LSB
	uint8_t r = 0;
	for(int j = 0; j < 8; ++j){
		r += data[(*i)+j]<<j;
	}
	(*i) += 8;
	return r;
}

uint16_t read16(bit *data, uint32_t *i){
	// This function is used for decompression
	// The first bit is the LSB
	uint16_t r = 0;
	for(int j = 0; j < 16; ++j){
		r += data[(*i)+j]<<j;
	}
	(*i) += 16;
	return r;
}

typedef enum compressionType{
	NONE, FIXED, DYNAMIC, ERROR,
} compressionType;

bit *decompressData(bit *data, uint32_t len){
	// we have to decompress with zlib
				// in zlib the first bit is the least significant
				// (b0 b1 b2 b3)
				// ftp://ftp.isi.edu/in-notes/rfc1950.txt
				// https://www.ietf.org/rfc/rfc1950.txt
				// https://stackoverflow.com/questions/42934720/understanding-the-zlib-header-cmf-cm-cinfo-flg-fdict-dictid-flevel-rfc
				
				/*uint8_t CMF = data[0], FLG = data[1], FDICT, FLEVEL;
				if(!validateCMFFLG(CMF, FLG)){
					fprintf(stderr, "Invalid CMF+FLG check\n");
					fclose(fp);
					return 0;
				}
				uint32_t LZ77size = (1<<getCINFO(CMF))<<8;
				FDICT = getBit(FLG, 5);
				FLEVEL = getBit(FLG, 6) + (getBit(FLG, 7)<<1);
				if(FDICT){
					fprintf(stderr, "I dont know how to deal with DICT\n");
					fclose(fp);
					return 0;
				}
				
				// Deflate data
				// https://www.ietf.org/rfc/rfc1951.txt
				/*
				Data elements are packed into bytes in order of
					increasing bit number within the byte, i.e., starting
					with the least-significant bit of the byte.
				Data elements other than Huffman codes are packed
					starting with the least-significant bit of the data
					element.
				Huffman codes are packed starting with the most-
					significant bit of the code.
				*
				/

				// Adler32
	bit *out = malloc(len);
	bit BFINAL;
	uint8_t BTYPE;
	uint32_t ii = 0, oi = 0;
	while(!BFINAL){
		BFINAL = data[ii++];
		if(BFINAL){
			// TODO: exit
		}
		BTYPE = (data[ii]<<1) + data[ii+1];
		ii += 2;
		if(BTYPE == NONE){
			ii += 8-(ii%8);
		}
		uint16_t LEN, NLEN;
		LEN = read16(data, &ii);
		NLEN = read16(data, &ii);

		for(uint16_t j = 0; j < LEN; ++j){

		}
	}
}
*/