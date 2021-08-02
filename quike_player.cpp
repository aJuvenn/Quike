/*
 * quike_player.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.hpp"

#define QK_PLAYER_CAMERA_HEIGHT 1.5


Player * qkGlobalPlayer;


Player::Player(const double x, const double y, const double theta, const double phi):
		mouvable(x, y),
		camera(Vector3d(x, y, QK_PLAYER_CAMERA_HEIGHT), theta, phi)
{
	memset(&events, 0, sizeof(QKPlayerEvent));
}


void Player::setGlobalPlayer()
{
	qkGlobalPlayer = this;
}


#define QK_PLAYER_SPEED_FORWARD 20000.
#define QK_PLAYER_SPEED_LATERAL 10000.
#define QK_PLAYER_SPEED_DIAGONAL_NORMALISATION 0.7

#define QK_PLAYER_HORIZONTAL_ANGULAR_SPEED 0.01
#define QK_PLAYER_VERTICAL_ANGULAR_SPEED 0.01



void Player::move(const double dt)
{
	if (events.dMouseX != 0  || events.dMouseY != 0){
		GLdouble dTheta = - QK_PLAYER_HORIZONTAL_ANGULAR_SPEED * events.dMouseX;
		GLdouble dPhi = QK_PLAYER_VERTICAL_ANGULAR_SPEED * events.dMouseY;
		camera.increaseDirection(dTheta, dPhi);
	}

	int dForward = events.forward - events.backward;
	int dLateral = events.left - events.right;

	qkPlayerEventReset(&events);

	if (dForward == 0 && dLateral == 0){
		return;
	}

	GLdouble vForward = QK_PLAYER_SPEED_FORWARD * (GLdouble) dForward;
	GLdouble vLateral = QK_PLAYER_SPEED_LATERAL * (GLdouble) dLateral;

	if (dForward != 0 && dLateral != 0){
		vForward *= QK_PLAYER_SPEED_DIAGONAL_NORMALISATION;
		vLateral *= QK_PLAYER_SPEED_DIAGONAL_NORMALISATION;
	}

	const Vector2d & floorDirection = camera.getFloorDirection();
	const Vector2d & floorOrthoDirection = camera.getFloorOrthoDirection();
	const Vector2d velocity = vForward * floorDirection + vLateral * floorOrthoDirection;

	mouvable.setVelocity(velocity);
	const Vector2d & newPosition = mouvable.move(dt);

	Vector3d newCamPosition(newPosition(0), newPosition(1), QK_PLAYER_CAMERA_HEIGHT);
	camera.updatePosition(newCamPosition);
}


void Player::lookAt() const
{
	camera.lookAt();
}

