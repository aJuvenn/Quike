/*
 * quike_picture.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_PICTURE_HPP_
#define QUIKE_PICTURE_HPP_

#include "quike_header.hpp"

typedef struct __attribute__((packed))
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} QKColor;


typedef struct {
	size_t width;
	size_t height;
	QKColor ** pixels;
} QKPicture;


#define QK_COLOR_RED ((QKColor){255,0,0})
#define QK_COLOR_GREEN ((QKColor){0,255,0})
#define QK_COLOR_BLUE ((QKColor){0,0,255})
#define QK_COLOR_WHITE ((QKColor){255,255,255})
#define QK_COLOR_BLACK ((QKColor){0,0,0})

int qkColorAreEquals(QKColor c1, QKColor c2);



QKPicture * qkPictureNew(size_t width, size_t height);

QKPicture * qkPictureLoadFromBitmap(const char * const bitmapFilePath);

void qkPictureFree(QKPicture * const pic);



#endif /* QUIKE_PICTURE_HPP_ */
