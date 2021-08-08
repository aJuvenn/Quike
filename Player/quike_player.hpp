/*
 * quike_player.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_PLAYER_HPP_
#define QUIKE_PLAYER_HPP_

#include "../quike_header.hpp"


class Player
{
	Movable2d mouvable;
	Camera camera;

public:
	QKPlayerEvent events;

	Player(const double x, const double y, const double theta = 0., const double phi = M_PI_2);

	void setGlobalPlayer();
	void lookAt() const;

	void move(const double dt);

	const Vector3d & getCameraPosition() const;
	const Vector3d & getCameraDirection() const;
};

extern Player * qkGlobalPlayer;


#endif /* QUIKE_PLAYER_HPP_ */
