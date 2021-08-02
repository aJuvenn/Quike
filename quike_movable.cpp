/*
 * quike_movable.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.hpp"


Movable2d::Movable2d(const double x, const double y)
{
	position << x, y;
	velocity << 0., 0.;
}


const Vector2d & Movable2d::move(const double dt)
{
	const Vector2d newPosition = position + dt * velocity;

	if (qkCellTypef(newPosition(0), newPosition(1)) == QK_WALL_CELL){
		return position;
	}

	position = newPosition;
	return position;
}


const Vector2d & Movable2d::getPosition() const
{
	return position;
}


const Vector2d & Movable2d::getVelocity() const
{
	return velocity;
}


Movable2d::Movable2d(const Vector2d & position):
		position(position)
{
	velocity << 0., 0.;
}


Movable2d::Movable2d(const Vector2d & position, const Vector2d & velocity):
		position(position), velocity(velocity)
{}


void Movable2d::setPosition(const Vector2d & newPosition)
{
	position = newPosition;
}


void Movable2d::setVelocity(const Vector2d & newVelocity)
{
	velocity = newVelocity;
}







