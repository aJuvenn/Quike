/*
 * quike_camera.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */


#ifndef QUIKE_CAMERA_HPP_
#define QUIKE_CAMERA_HPP_

#include "../quike_header.hpp"

#define QK_CAMERA_VERTICAL_BOUND 0.1


class Camera
{
	Vector3d position;
	double theta;
	double phi;

	Vector3d center;
	Vector3d direction;

	Vector2d floorDirection;
	Vector2d floorOrthoDirection;

public:

	Camera(const Vector3d & position, const double theta = 0., const double phi = M_PI_2);

	void updatePosition(const Vector3d & newPosition);
	void increasePosition(const Vector3d & dPosition);

	void updateDirection(const double newTheta, const double newPhi);
	void increaseDirection(const double dTheta, const double dPhi);

	void lookAt() const;

	const Vector3d & getPosition() const;
	double getTheta() const;
	double getPhi() const;

	const Vector3d & getDirection() const;
	const Vector3d & getCenter() const;

	const Vector2d & getFloorDirection() const;
	const Vector2d & getFloorOrthoDirection() const;

};

#endif /* QUIKE_CAMERA_HPP_ */
