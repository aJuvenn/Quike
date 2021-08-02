/*
 * quike_movable.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_MOVABLE_HPP_
#define QUIKE_MOVABLE_HPP_

#include "quike_header.hpp"


class Movable2d
{
	Vector2d position;
	Vector2d velocity;

public:

	Movable2d(const double x, const double y);
	Movable2d(const Vector2d & position);
	Movable2d(const Vector2d & position, const Vector2d & velocity);

	const Vector2d & getPosition() const;
	const Vector2d & getVelocity() const;

	const Vector2d & move(const double dt);
	void setPosition(const Vector2d & newPosition);
	void setVelocity(const Vector2d & newVelocity);

};


#endif /* QUIKE_MOVABLE_HPP_ */
