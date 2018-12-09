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


QKMapCell qkCellType(const unsigned x, const unsigned y)
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

	return qkCellType((unsigned) nearestX, (unsigned) nearestY);
}




void qkFreeMap()
{
	free(qkGlobalMap.cells);
}


void qkDrawMap()
{
	for (unsigned x = 0 ; x < qkGlobalMap.xSize ; x++){
		for (unsigned y = 0 ; y < qkGlobalMap.ySize ; y++){

			switch (qkCellType(x, y)){

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
