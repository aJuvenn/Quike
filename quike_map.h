/*
 * quike_map.h
 *
 *  Created on: 8 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_MAP_H_
#define QUIKE_MAP_H_

#include "quike_header.h"


typedef enum  {
	QK_NONE_CELL = 0,
	QK_FLOOR_CELL,
	QK_WALL_CELL,
	QK_RESPAWN_CELL

} QKMapCell;


typedef struct {

	size_t xSize;
	size_t ySize;
	QKMapCell * cells;

} QKMap ;


QKMap qkGlobalMap;

void qkLoadNewMap(const unsigned xSize, const unsigned ySize);

int qkLoadNewMapFromBitmap(const char * const bitmapPath);

QKMapCell qkMapCellType(const unsigned x, const unsigned y);

void qkMapSetCellType(const QKMapCell type, const unsigned x, const unsigned y);

void qkFreeGlobalMap();

QKMapCell qkCellTypef(const GLfloat x, const GLfloat y);


void qkDrawMap();

#endif /* QUIKE_MAP_H_ */
