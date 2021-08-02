/*
 * quike_solid.hpp
 *
 *  Created on: 2 août 2021
 *      Author: ajuvenn
 */

#ifndef QUIKE_SOLID_HPP_
#define QUIKE_SOLID_HPP_

#include "quike_header.hpp"



class Solid
{
	/* Initial list of points and masses provided at creation */
	Matrix3Xd initialPoints;
	VectorXd masses;
	size_t nbPoints;

	/* Center of mass and its velocity */
	Vector3d centerPosition;
	Vector3d centerVelocity;

	/* Position of the points relative to the center of mass, stacked into a big matrix */
	Matrix3Xd centeredInitialPoints;

	/* Total mass and its inverse */
	double totalMass;
	double totalMassInverse;

	/* Inertia matrix and its inverse */
	Matrix3d initialInertiaMatrix;
	Matrix3d initialInertiaMatrixInverse;

	/* Rotation quaternion (used to construct matrix representing orientation)
	 *  and angular momentum */
	Quaterniond rotationQuaternion;
	Matrix3d rotationMatrix;
	Vector3d angularMomentum;

	/* Integrates velocity and position for a time step dt if the sum
	 * of every forces is provided */
	void updatePosition(const Vector3d & totalForce, const double dt);

	/* Integrates the provided force momentum to change angular momentum,
       uses solid inertia to compute the angular speed and uses it to
       update the quaternion representing the object orientation */
	void updateOrientation(const Vector3d & totalForceMomentum, const double dt);


public:

	Solid(const Matrix3Xd & points, const VectorXd & masses);
	void update(const Vector3d & totalForce, const Vector3d & totalForceMomentum, const double dt);

	void glDraw() const;

	void getPoints(Matrix3Xd & outputPoint) const;

	void print() const;

};


extern Solid * qkGlobalSolid;


#endif /* QUIKE_SOLID_HPP_ */
