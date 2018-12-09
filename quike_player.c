/*
 * quike_player.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.h"

#define QK_PLAYER_CAMERA_HEIGHT 1.5

QKPlayer * qkPlayerNew(const GLdouble x, const GLdouble y, const GLdouble theta)
{
	QKPlayer * output;
	QK_SIMPLE_ALLOC(output, 1);

	qkMovableInit(&(output->mouvable), x, y);

	qkPlayerEventReset(&(output->events));

	GLdouble cameraPosition[3] = {x, y, QK_PLAYER_CAMERA_HEIGHT};
	output->camera = qkCameraNew(cameraPosition, theta, M_PI/2);

	return output;
}


void qkPlayerFree(QKPlayer * const player)
{
	qkCameraFree(player->camera);
	free(player);
}


void qkPlayerSetGlobalPlayer(QKPlayer * const player)
{
	qkGlobalPlayer = player;
}


#define QK_PLAYER_FORWARD_SPEED 20000.
#define QK_PLAYER_LATERAL_SPEED 10000.

#define QK_PLAYER_HORIZONTAL_ANGULAR_SPEED 0.01
#define QK_PLAYER_VERTICAL_ANGULAR_SPEED 0.01


void qkPlayerMove(QKPlayer * const player, const GLdouble dt)
{
	if (player->events.dMouseX != 0  || player->events.dMouseY != 0){

		GLdouble dTheta = - QK_PLAYER_HORIZONTAL_ANGULAR_SPEED * player->events.dMouseX;
		GLdouble dPhi = QK_PLAYER_VERTICAL_ANGULAR_SPEED * player->events.dMouseY;

		qkCameraIncreaseDirection(player->camera, dTheta, dPhi);
	}

	int dForward = player->events.forward - player->events.backward;
	int dLateral = player->events.left - player->events.right;

	qkPlayerEventReset(&(player->events));

	if (dForward == 0 && dLateral == 0){
		return;
	}

	GLdouble vForward = QK_PLAYER_FORWARD_SPEED * dForward;
	GLdouble vLateral = QK_PLAYER_LATERAL_SPEED * dLateral;
	GLdouble * floorDirection = player->camera->floorDirection;
	GLdouble * floorOrthoDirection = player->camera->floorOrthoDirection;

	GLdouble vX = vForward * floorDirection[0] + vLateral * floorOrthoDirection[0];
	GLdouble vY = vForward * floorDirection[1] + vLateral * floorOrthoDirection[1];

	player->mouvable.vX = vX;
	player->mouvable.vY = vY;

	qkMovableMove(&(player->mouvable), dt);

	GLdouble newCamPosition[3] = {player->mouvable.x, player->mouvable.y, QK_PLAYER_CAMERA_HEIGHT};
	qkCameraUpdatePosition(player->camera, newCamPosition);

}

