#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>
#include "zlib.h"

#define HSIZE 8
#define CSIZE 4


int byteArrayEquals(uint8_t *b1, uint8_t *b2, uint32_t size){
	uint32_t i = 0;
	while(i < size && b1[i] == b2[i])
		++i;
	return i == size;
}

uint8_t paethPredictor(uint8_t a, uint8_t b, uint8_t c){
	uint8_t p = (a + b - c)%256;	// initial estimate
	uint8_t pa = abs(p - a);		// distances to a, b, c
	uint8_t pb = abs(p - b);
	uint8_t pc = abs(p - c);
	// return nearest of a,b,c,
	// breaking ties in order a,b,c.
	if(pa <= pb && pa <= pc)
		return a;
	else if(pb <= pc)
		return b;
	else
		return c;
}

bool matchRGB(uint8_t c1[3], uint8_t c2[3]){
	return (c1[0] == c2[0]) && (c1[1] == c2[1]) && (c1[2] == c2[2]);
}


uint8_t PNG[HSIZE] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
uint8_t IHDR[CSIZE] = {0x49, 0x48, 0x44, 0x52};
uint8_t IDAT[CSIZE] = {0x49, 0x44, 0x41, 0x54};
uint8_t	IEND[CSIZE] = {0x49, 0x45, 0x4E, 0x44};


int main(){

	uint32_t pixels = 0;

	FILE *fp = fopen("download.png", "rb");	// File to read
	uint8_t red[3] = {0xFF, 0x00, 0x00};	// Color to look for

	// Read PNG header
	uint8_t header[HSIZE];
	fread(header, sizeof(uint8_t), HSIZE, fp);
	if(!byteArrayEquals(header, PNG, HSIZE)){
		fprintf(stderr, "Invalid PNG signature\n");
		fclose(fp);
		return 0;
	}
	
	uint32_t width = 0, height = 0;
	uint8_t bitDepth, colorType, compressionMethod, filterMethod, interlaceMethod;

	// Start reading file
	while(!feof(fp)){

		uint8_t lenbuf[CSIZE], ctype[CSIZE], crcbuf[CSIZE];

		// Read length of chunk
		fread(lenbuf, sizeof(uint8_t), CSIZE, fp);
		uint32_t len = 0;
		for(int i = 0; i < CSIZE; ++i){
			len += lenbuf[i]<<(8*(3-i));
		}
		uint8_t *data = calloc(len, sizeof(uint8_t));

		// Read chunk type
		fread(ctype, sizeof(uint8_t), CSIZE, fp);

		// Read data according to type
		fread(data, sizeof(uint8_t), len, fp);
		if(byteArrayEquals(ctype, IHDR, CSIZE)){
			for(int i = 0; i < 4; ++i){
				width += data[i]<<(8*(3-i));
				height += data[i+4]<<(8*(3-i));
			}
			bitDepth = data[8];
			colorType = data[9];
			compressionMethod = data[10];
			filterMethod = data[11];
			interlaceMethod = data[12];
		}else if(byteArrayEquals(ctype, IDAT, CSIZE)){

			// Only implementing RGB
			if(colorType == 2){

				uint8_t *d = malloc(height*(1 + width*3)*sizeof(uint8_t));
				uLongf nlen;
				uncompress((Bytef *)d, &nlen, (Bytef *)data, len);

				for(uint16_t i = 0; i < height; ++i){

					uint8_t color[3] = {0x00, 0x00, 0x00};
					uint8_t filterType = d[i*(1+width*3) + 0];
					uint8_t bpp = 3*(bitDepth/8);

					for(uint16_t j = 1; j < 1+width*3; ++j){
						
						uint8_t ant = (j-bpp < 1) ? 0 : d[i*(1+width*3) + j-bpp];
						uint8_t prev = (i == 0) ? 0 : d[(i-1)*(1+width*3) + j];
						uint8_t antprev = (j-bpp < 1 || i == 0) ? 0 : d[(i-1)*(1+width*3) + j-bpp];

						switch(filterType){
							case 1:
								d[i*(1+width*3) + j] = (d[i*(1+width*3) + j] + ant)%256;
								break;
							case 2:
								d[i*(1+width*3) + j] = (d[i*(1+width*3) + j] + prev)%256;
								break;
							case 3:
								d[i*(1+width*3) + j] = (d[i*(1+width*3) + j] + (uint8_t)floor((ant + prev)/2))%256;
								break;
							case 4:
								d[i*(1+width*3) + j] = (d[i*(1+width*3) + j] + paethPredictor(ant, prev, antprev))%256;
								break;
						}
						
						// Stores pixels in pixels array
						color[(j-1)%3] = d[i*(1+width*3) + j];

						// Counts red pixels
						if(j%3 == 0 && matchRGB(color, red)){
							++pixels;
						}
					}
				}	
			}
		}

		// Read CRC checksum
		fread(crcbuf, sizeof(uint8_t), CSIZE, fp);

		if(byteArrayEquals(ctype, IEND, CSIZE)){
			break;	
		}
	}
	
	fclose(fp);

	printf("Pixels: %d\n", pixels);

	return 0;
}