/*
 * quike_movable.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_MOVABLE_H_
#define QUIKE_MOVABLE_H_

#include "quike_header.h"


typedef struct {

	GLdouble x;
	GLdouble y;

	GLdouble vX;
	GLdouble vY;

} QKMovable;



void qkMovableMove(QKMovable * mv, const GLdouble dt);

void qkMovableInit(QKMovable * const mv, const GLdouble x, const GLdouble y);




#endif /* QUIKE_MOVABLE_H_ */
