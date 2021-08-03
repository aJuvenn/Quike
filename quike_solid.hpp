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

	/* Total mass and its inverse */
	double totalMass;
	double totalMassInverse;

	/* Center of mass and its velocity */
	Vector3d centerPosition;
	Vector3d centerVelocity;

	/* Position of the points relative to the center of mass, stacked into a big matrix */
	Matrix3Xd centeredInitialPoints;

	/* Inertia matrix and its inverse */
	Matrix3d initialInertiaMatrix;
	Matrix3d initialInertiaMatrixInverse;

	/* Rotation quaternion (used to construct matrix representing orientation)
	 *  and angular momentum */
	Quaterniond rotationQuaternion;
	Matrix3d rotationMatrix;
	Vector3d angularMomentum;

	/* Current points: need to be computed again after a change of position or orientation */
	Matrix3Xd currentPoints;

	/* AABB used for collision detection */
	Matrix<double, 3, 2> minAndMaxCoords;
	Matrix<double, 3, 8> aabbCornerPoints;


	/* Integrates velocity and position for a time step dt if the sum
	 * of every forces is provided */
	void updatePosition(const Vector3d & totalForce, const double dt);

	/* Integrates the provided force momentum to change angular momentum,
       uses solid inertia to compute the angular speed and uses it to
       update the quaternion representing the object orientation */
	void updateOrientation(const Vector3d & totalForceMomentum, const double dt);

	void updateCurrentPoints();

	void updateAABB();

public:

	Solid(const Matrix3Xd & points, const VectorXd & masses);

	void update(const Vector3d & totalForce, const Vector3d & totalForceMomentum, const double dt);
	void update(const Matrix3Xd & forcePerPoint, const double dt);

	void glDraw() const;

	const Matrix3Xd & getCurrentPoints() const;

	void print() const;

	bool aabbIntersectsWith(const Solid & otherSolid) const;

	const Vector3d & getCenterPosition() const;


	const Matrix<double, 3, 8> & getAabbCornerPoints() const;
};


extern Solid * qkGlobalSolid;


class AabbCollisionDetector
{
	size_t nbSolids;
	std::vector<Solid *> solids;

	Vector3d getProjectionAxis();

public:

	AabbCollisionDetector();

	void addSolid(Solid * solid);
	const std::vector<Solid *> & getSolids();

	std::vector<std::pair<Solid *, Solid *>> getCollidingSolids();

};

extern AabbCollisionDetector * qkGlobalAabbCollisionDetector;


#endif /* QUIKE_SOLID_HPP_ */
