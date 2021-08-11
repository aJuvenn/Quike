/*
 * quike_rigid_body.h
 *
 *  Created on: 7 août 2021
 *      Author: ajuvenn
 */

#include "../quike_header.hpp"

#ifndef PHYSICS_QUIKE_SOLID_HPP_
#define PHYSICS_QUIKE_SOLID_HPP_


class Solid
{
protected:
	/* Total mass and its inverse */
	double totalMass;
	double totalMassInverse;

	/* Center of mass and its velocity */
	Vector3d centerPosition;
	Vector3d centerVelocity;

	/* Inertia matrix and its inverse */
	Matrix3d initialInertiaMatrix;
	Matrix3d initialInertiaMatrixInverse;

	/* Rotation quaternion (used to construct matrix representing orientation)
	 *  and angular momentum */
	Quaterniond rotationQuaternion;
	Matrix3d rotationMatrix;
	Vector3d angularMomentum;

	/* AABB used for collision detection */
	Matrix<double, 3, 2> minAndMaxCoords;
	Matrix<double, 3, 8> aabbCornerPoints;

	Vector3d color;

	/* Bounding box must be update every time position or orientation changes */
	void updateAABB();

public:

	Solid(const double mass, const Matrix3d & inertiaMatrix);
	virtual ~Solid() = 0;

	double getMass() const;
	Matrix3d getInertiaMatrix() const;
	const Vector3d & getCenterPosition() const;

	bool aabbIntersectsWith(const Solid & otherSolid) const;
	const Matrix<double, 3, 8> & getAabbCornerPoints() const;

	virtual Vector3d getClosestPoint(const Vector3d & direction) const = 0;

	Matrix4d getGlTransfMatrix() const;

	virtual void print() const = 0;
	virtual void glDraw() const = 0;
	void glDrawAabb(bool plotInRed = false) const;

	void setPosition(const Vector3d & newPosition);
	void displace(const Vector3d & dPosition);

	void rotate(const Vector3d & axis, const double dAngle);
	void rotateX(const double dAngle);
	void rotateY(const double dAngle);
	void rotateZ(const double dAngle);
	void rotate(const Vector3d & dRotationVector);


	void applyForce(const Vector3d & totalForce, const Vector3d & totalForceMomentum, const double dt);
};


extern std::vector<Solid *> qkGlobalSolidList;


class SolidSphere : public Solid
{
	double radius;

public:

	SolidSphere(const double radius, const double mass);
	virtual ~SolidSphere();

	double getRadius() const;


	virtual Vector3d getClosestPoint(const Vector3d & direction) const;
	virtual void print() const;
	virtual void glDraw() const;
};


class SolidCuboid : public Solid
{
	double widthX, widthY, widthZ;

public:
	SolidCuboid(const double widthX, const double widthY, const double widthZ, const double mass);
	virtual ~SolidCuboid();

	double getWidthX() const;
	double getWidthY() const;
	double getWidthZ() const;

	virtual Vector3d getClosestPoint(const Vector3d & direction) const;
	virtual void print() const;
	virtual void glDraw() const;
};


class SolidPoints : public Solid
{
	Matrix3Xd initialPoints;
	Matrix3Xd centeredInitialPoints;
	VectorXd masses;
	size_t nbPoints;
	Matrix3Xd currentPoints;

public:

	SolidPoints(const Matrix3Xd & points, const VectorXd & masses);
	virtual ~SolidPoints();

	Matrix3Xd computeCurrentPoints() const;
	const Matrix3Xd & getInitialPoints() const;
	const VectorXd & getMasses() const;
	size_t getNbPoints() const;

	virtual Vector3d getClosestPoint(const Vector3d & direction) const;
	virtual void print() const;
	virtual void glDraw() const;
};


class SolidUnion : public Solid
{
	size_t nbSolids;
	std::vector<Solid *> solids;

public:

	SolidUnion(std::vector<Solid *> & solids);
	virtual ~SolidUnion();

	virtual Vector3d getClosestPoint(const Vector3d & direction) const;
	virtual void print() const;
	virtual void glDraw() const;
};

#endif /* PHYSICS_QUIKE_SOLID_HPP_ */
