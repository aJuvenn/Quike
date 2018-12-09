/*
 * quike_map.c
 *
 *  Created on: 8 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.h"


QKMap qkGlobalMap;


void qkLoadNewMap(const unsigned xSize, const unsigned ySize)
{
	qkGlobalMap.xSize = xSize;
	qkGlobalMap.ySize = ySize;
	QK_SIMPLE_ALLOC(qkGlobalMap.cells, xSize * ySize);

}




void qkMapSetCellType(const QKMapCell type, const unsigned x, const unsigned y)
{
	if (x >= qkGlobalMap.xSize || y >= qkGlobalMap.ySize){
		return;
	}

	qkGlobalMap.cells[x + qkGlobalMap.xSize * y] = type;
}

int qkLoadNewMapFromBitmap(const char * const bitmapPath)
{
	printf("a\n");

	QKPicture * pic = qkPictureLoadFromBitmap(bitmapPath);

	if (pic == NULL){
		return EXIT_FAILURE;
	}

	size_t width = pic->width;
	size_t height = pic->height;

	qkLoadNewMap(width, height);

	for (unsigned x = 0 ; x < width ; x++){
		for (unsigned y = 0 ; y < height ; y++){

			QKColor c = pic->pixels[height - 1 - y][x];

			if (qkColorAreEquals(c, QK_COLOR_BLACK)){
				qkMapSetCellType(QK_WALL_CELL, x, y);
			} else if (qkColorAreEquals(c, QK_COLOR_WHITE)){
				qkMapSetCellType(QK_FLOOR_CELL, x, y);
			} else {
				qkMapSetCellType(QK_NONE_CELL, x, y);
			}
		}
	}

	qkPictureFree(pic);

	return EXIT_SUCCESS;
}




QKMapCell qkMapCellType(const unsigned x, const unsigned y)
{
	if (x >= qkGlobalMap.xSize || y >= qkGlobalMap.ySize){
		return QK_NONE_CELL;
	}

	return qkGlobalMap.cells[x + qkGlobalMap.xSize * y];
}

QKMapCell qkCellTypef(const GLfloat x, const GLfloat y)
{
	long int nearestX, nearestY;

	nearestX = lrintf(x);
	nearestY = lrintf(y);

	if (nearestX < 0){
		nearestX = 0;
	}

	if (nearestY < 0){
		nearestY = 0;
	}

	return qkMapCellType((unsigned) nearestX, (unsigned) nearestY);
}




void qkFreeMap()
{
	free(qkGlobalMap.cells);
}


void qkDrawMap()
{
	for (unsigned x = 0 ; x < qkGlobalMap.xSize ; x++){
		for (unsigned y = 0 ; y < qkGlobalMap.ySize ; y++){

			switch (qkMapCellType(x, y)){

			case QK_FLOOR_CELL:
				qkDrawFloor(x, y);
				break;

			case QK_WALL_CELL:
				qkDrawWall(x, y);
				break;

			case QK_RESPAWN_CELL:
				qkDrawRespawn(x, y);
				break;

			default:
				break;
			}
		}
	}
}
