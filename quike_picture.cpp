/*
 * quike_picture.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.hpp"



int qkColorAreEquals(QKColor c1, QKColor c2)
{
	return c1.red == c2.red
			&& c1.green == c2.green
			&& c1.blue == c2.blue;
}


QKPicture * qkPictureNew(size_t width, size_t height)
{
	QKPicture * output;
	QK_SIMPLE_ALLOC(output, 1);

	output->width = width;
	output->height = height;

	QKColor * data;
	QK_SIMPLE_ALLOC(data, width * height);

	QK_SIMPLE_ALLOC(output->pixels, height);

	for (unsigned i = 0 ; i < height ; i++){
		output->pixels[i] = data + i * width;
	}

	return output;
}


struct QKBitmapHeader {

	/*
	 * Header
	 */

	uint8_t BM[2];
	uint32_t fileSize;
	uint32_t reserved; // unused (= 0)
	uint32_t dataOffset;

	/*
	 * Info Header
	 */

	uint32_t infoHeaderSize; //(= 40)
	uint32_t width;
	uint32_t height;
	uint16_t planes; // Number of planes (= 1)

	uint16_t bitCount; // Number of bits per pixel (24)
	uint32_t compression; // No compression : 0
	uint32_t imageSize; // = 0 if compression = 0

	uint32_t xPixelsPerM;
	uint32_t yPixelsPerM;
	uint32_t colorsUsed;
	uint32_t colorsImportant; // 0 = all

} __attribute__((packed));


typedef struct QKBitmapHeader QKBitmapHeader;



uint8_t * qkBitmapData(const char * const bitmapFilePath, uint32_t * output_fileSize)
{
	FILE * f = fopen(bitmapFilePath, "r");

	if (f == NULL){
		return NULL;
	}

	uint8_t firstBytes[8 + 32];

	if (fread(firstBytes, 1, 40, f) != 40
			|| firstBytes[0] != 'B'
					|| firstBytes[1] != 'M'){
		fclose(f);
		return NULL;
	}

	uint32_t fileSize = *((uint32_t *) (firstBytes + 2));
	uint8_t * data;
	QK_SIMPLE_ALLOC(data, fileSize);

	rewind(f);

	if (fread(data, 1, fileSize, f) != fileSize){
		fclose(f);
		free(data);
		return NULL;
	}

	fclose(f);

	*output_fileSize = fileSize;

	return data;
}



QKPicture * qkPictureLoadFromBitmap(const char * const bitmapFilePath)
{
	uint32_t fileSize;
	uint8_t * data = qkBitmapData(bitmapFilePath, &fileSize);

	if (data == NULL){
		return NULL;
	}

	QKBitmapHeader * header = (QKBitmapHeader *) data;

	if (header->bitCount != 24 || header->compression != 0){
		free(data);
		return NULL;
	}

	uint32_t height = header->height;
	uint32_t width = header->width;
	QKPicture * output = qkPictureNew(width, height);

	uint8_t * pixels = data + header->dataOffset;

	unsigned padding = (4 - ((width * sizeof(QKColor)) % 4)) % 4;

	for (uint32_t i = 0 ; i < height ; i++){
		uint8_t	* bitmapLine = pixels + i * (width*sizeof(QKColor) + padding);
		memcpy(output->pixels[i], bitmapLine, width * sizeof(QKColor));
	}

	free(data);

	return output;
}


void qkPictureFree(QKPicture * const pic)
{
	free(pic->pixels[0]);
	free(pic->pixels);
	free(pic);
}


