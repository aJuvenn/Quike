/*
 * quike_movable.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.h"



void qkMovableMove(QKMovable * mv, const GLdouble dt)
{
	GLdouble newX = mv->x + dt * mv->vX;
	GLdouble newY = mv->y + dt * mv->vY;

	if (qkCellTypef(newX, newY) == QK_WALL_CELL){
		return;
	}

	mv->x = newX;
	mv->y = newY;
}



void qkMovableInit(QKMovable * const mv, const GLdouble x, const GLdouble y)
{
	mv->x = x;
	mv->y = y;
	mv->vX = 0;
	mv->vY = 0;
}
