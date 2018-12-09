/*
 * quike_player.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_PLAYER_H_
#define QUIKE_PLAYER_H_

#include "quike_header.h"

typedef struct {

	QKMovable mouvable;
	QKPlayerEvent events;
	QKCamera * camera;

} QKPlayer;


QKPlayer * qkGlobalPlayer;

QKPlayer * qkPlayerNew(const GLdouble x, const GLdouble y, const GLdouble theta);
void qkPlayerFree(QKPlayer * const player);

void qkPlayerSetGlobalPlayer(QKPlayer * const player);

void qkPlayerMove(QKPlayer * const player, const GLdouble dt);


#endif /* QUIKE_PLAYER_H_ */
