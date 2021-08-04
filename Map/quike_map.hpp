/*
 * quike_map.h
 *
 *  Created on: 8 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_MAP_HPP_
#define QUIKE_MAP_HPP_

#include "../quike_header.hpp"


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


extern QKMap qkGlobalMap;

void qkLoadNewMap(const unsigned xSize, const unsigned ySize);

int qkLoadNewMapFromBitmap(const char * const bitmapPath);

QKMapCell qkMapCellType(const unsigned x, const unsigned y);

void qkMapSetCellType(const QKMapCell type, const unsigned x, const unsigned y);

void qkFreeGlobalMap();

QKMapCell qkCellTypef(const GLfloat x, const GLfloat y);


void qkDrawMap();

#endif /* QUIKE_MAP_HPP_ */
