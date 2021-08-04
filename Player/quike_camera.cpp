/*
 * quike_camera.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "../quike_header.hpp"



void Camera::updateDirection(const double newTheta, const double newPhi)
{
	theta = newTheta;

	if (newPhi < QK_CAMERA_VERTICAL_BOUND){
		phi = QK_CAMERA_VERTICAL_BOUND;
	} else if (newPhi > M_PI - QK_CAMERA_VERTICAL_BOUND) {
		phi = M_PI - QK_CAMERA_VERTICAL_BOUND;
	} else {
		phi = newPhi;
	}

	const double sinPhi = sin(phi);
	direction << sinPhi * cos(theta), sinPhi * sin(theta), cos(phi);
	floorDirection << direction.head(2);
	floorDirection.normalize();
	floorOrthoDirection << -floorDirection(1), floorDirection(0);
	center = position + direction;
}


void Camera::updatePosition(const Vector3d & newPosition)
{
	position = newPosition;
	center = position + direction;
}


void Camera::increaseDirection(const double dTheta, const double dPhi)
{
	updateDirection(theta + dTheta, phi + dPhi);
}


void Camera::increasePosition(const Vector3d & dPosition)
{
	updatePosition(position + dPosition);
}


Camera::Camera(const Vector3d & position, const double theta, const double phi)
{
	updatePosition(position);
	updateDirection(theta, phi);
}


const Vector2d & Camera::getFloorDirection() const
{
	return floorDirection;
}

const Vector2d & Camera::getFloorOrthoDirection() const
{
	return floorOrthoDirection;
}

const Vector3d & Camera::getPosition() const
{
	return position;
}

const Vector3d & Camera::getDirection() const
{
	return direction;
}

const Vector3d & Camera::getCenter() const
{
	return center;
}

double Camera::getTheta() const
{
	return theta;
}

double Camera::getPhi() const
{
	return phi;
}


void Camera::lookAt() const
{
	glLoadIdentity();
	gluLookAt(
			position(0), position(1), position(2), // eye
			center(0), center(1), center(2), // center
			0., 0., 1. // up
	);
}
