/*
 * quike_rigid_body.cpp
 *
 *  Created on: 7 août 2021
 *      Author: ajuvenn
 */

#include "../quike_header.hpp"


std::vector<Solid *> qkGlobalSolidList;


Solid::Solid(const double mass, const Matrix3d & inertiaMatrix)
: totalMass(mass),
  totalMassInverse(1. / mass),
  centerPosition(0., 0., 0.),
  centerVelocity(0., 0., 0.),
  initialInertiaMatrix(inertiaMatrix),
  initialInertiaMatrixInverse(inertiaMatrix.inverse()),
  rotationQuaternion(1., 0., 0., 0.),
  rotationMatrix(rotationQuaternion.toRotationMatrix()),
  angularMomentum(0., 0., 0.),
  color((Vector3d::Ones() + Vector3d::Random())/2.)
{

}

Solid::~Solid()
{

}


void Solid::updateAABB()
{
	Vector3d direction;
	Vector3d p;

	direction << 1., 0., 0.;
	p = getClosestPoint(-direction);
	minAndMaxCoords(0, 0) = p(0);
	p = getClosestPoint(direction);
	minAndMaxCoords(0, 1) = p(0);

	direction << 0., 1., 0.;
	p = getClosestPoint(-direction);
	minAndMaxCoords(1, 0) = p(1);
	p = getClosestPoint(direction);
	minAndMaxCoords(1, 1) = p(1);

	direction << 0., 0., 1.;
	p = getClosestPoint(-direction);
	minAndMaxCoords(2, 0) = p(2);
	p = getClosestPoint(direction);
	minAndMaxCoords(2, 1) = p(2);

	int i = 0;

	for (int x = 0 ; x < 2 ; ++x){
		for (int y = 0 ; y < 2 ; ++y){
			for (int z = 0 ; z < 2 ; ++z){
				aabbCornerPoints(0, i) = minAndMaxCoords(0, x);
				aabbCornerPoints(1, i) = minAndMaxCoords(1, y);
				aabbCornerPoints(2, i) = minAndMaxCoords(2, z);
				++i;
			}
		}
	}
}


double Solid::getMass() const
{
	return totalMass;
}

Matrix3d Solid::getInertiaMatrix() const
{
	return rotationMatrix * initialInertiaMatrix * rotationMatrix.transpose();
}


const Vector3d & Solid::getCenterPosition() const
{
	return centerPosition;
}


bool Solid::aabbIntersectsWith(const Solid & otherSolid) const
{
	const Matrix<double, 3, 2> & AABB = minAndMaxCoords;
	const Matrix<double, 3, 2> & otherAABB = otherSolid.minAndMaxCoords;

	for (int i = 0 ; i < 3 ; i++){
		if (!((otherAABB(i, 0) <= AABB(i, 0) && AABB(i, 0) <= otherAABB(i, 1))
				|| (AABB(i, 0) <= otherAABB(i, 0) && otherAABB(i, 0) <= AABB(i, 1)))){
			return false;
		}
	}
	return true;
}


const Matrix<double, 3, 8> & Solid::getAabbCornerPoints() const
{
	return aabbCornerPoints;
}


void Solid::glDrawAabb(bool plotInRed) const
{
	if (plotInRed){
		glColor3f(1., 0., 0.);
	} else {
		glColor3f(0., 1., 0.);
	}

	glBegin(GL_LINES);

	for (int i = 0 ; i < 8 ; ++i){
		glVertex3f(aabbCornerPoints(0, i), aabbCornerPoints(1, i), aabbCornerPoints(2, i));
	}

	for (int i = 0 ; i < 4 ; ++i){
		int j = i+4;
		glVertex3f(aabbCornerPoints(0, i), aabbCornerPoints(1, i), aabbCornerPoints(2, i));
		glVertex3f(aabbCornerPoints(0, j), aabbCornerPoints(1, j), aabbCornerPoints(2, j));
	}

	for (int i = 0 ; i < 6 ; ++i){
		int j = i+2;
		glVertex3f(aabbCornerPoints(0, i), aabbCornerPoints(1, i), aabbCornerPoints(2, i));
		glVertex3f(aabbCornerPoints(0, j), aabbCornerPoints(1, j), aabbCornerPoints(2, j));
	}

	glEnd();
}


void Solid::setPosition(const Vector3d & newPosition)
{
	centerPosition = newPosition;
	updateAABB();
}


void Solid::displace(const Vector3d & dPosition)
{
	centerPosition += dPosition;
	updateAABB();
}


void Solid::rotateX(const double dAngle)
{
	const Vector3d dRotationVector(dAngle, 0., 0.);
	rotate(dRotationVector);
}


void Solid::rotateY(const double dAngle)
{
	const Vector3d dRotationVector(0., dAngle, 0.);
	rotate(dRotationVector);
}


void Solid::rotateZ(const double dAngle)
{
	const Vector3d dRotationVector(0., 0., dAngle);
	rotate(dRotationVector);
}


void Solid::rotate(const Vector3d & axis, const double dAngle)
{
	const Vector3d dRotationVector = axis * dAngle;
	rotate(dRotationVector);
}


void Solid::rotate(const Vector3d & dRotationVector)
{
	Quaterniond dRotationVectorAsQuaternion;
	dRotationVectorAsQuaternion.w() = 0.;
	dRotationVectorAsQuaternion.vec() = dRotationVector;

	/* Rotation quaternion integration from a derivative computed with a quaternion multiplication */
	const Quaterniond tmp = dRotationVectorAsQuaternion * rotationQuaternion;
	rotationQuaternion.w() += 0.5 * tmp.w();
	rotationQuaternion.vec() += 0.5 * tmp.vec();

	/* Normalization of the quaternion to ensure numerical stability through time steps */
	rotationQuaternion.normalize();

	/* Update of the rotation matrix from the new quaternion */
	rotationMatrix = rotationQuaternion.toRotationMatrix();

	updateAABB();
}


void Solid::applyForce(const Vector3d & totalForce, const Vector3d & totalForceMomentum, const double dt)
{
	/* Position update by two integrations */
	centerVelocity += dt * totalMassInverse * totalForce;
	centerPosition += dt * centerVelocity;

	/* Integration of the force momentum to get the angular momentum */
	angularMomentum += dt * totalForceMomentum;

	/* Rotation matrix corresponding to the current object angle */
	const Matrix3d & R = rotationMatrix;

	/* Inertia matrix inverse is obtained from the initial one by a change of bases */
	const Matrix3d currentInertiaMatrixInverse = R * initialInertiaMatrixInverse * R.transpose();

	/* Angular speed vector is obtained from the momentum and the inertia */
	const Vector3d dRotationVector = currentInertiaMatrixInverse * angularMomentum * dt;

	rotate(dRotationVector);
}


Matrix4d Solid::getGlTransfMatrix() const
{
	Matrix4d output;
	output.block<3,1>(0, 3) = centerPosition;
	output.topLeftCorner(3, 3) = rotationMatrix;
	output(3,3) = 1.;
	output.block<1, 3>(3, 0).setZero();
	return output;
}


void Solid::print() const
{
	std::cout << "-------- Solid --------" << std::endl;
	std::cout << "centerPosition:\n" << centerPosition << std::endl;
	std::cout << "centerVelocity:\n" << centerVelocity << std::endl;
	std::cout << "totalMass:\n" << totalMass << std::endl;
	std::cout << "totalMassInverse:\n" << totalMassInverse << std::endl;
	std::cout << "initialInertiaMatrix:\n" << initialInertiaMatrix << std::endl;
	std::cout << "initialInertiaMatrixInverse:\n" << initialInertiaMatrixInverse << std::endl;
	std::cout << "rotationQuaternion:\n" << rotationQuaternion.w() << std::endl << rotationQuaternion.vec() << std::endl;
	std::cout << "rotationMatrix:\n" << rotationMatrix << std::endl;
	std::cout << "angularMomentum:\n" << angularMomentum << std::endl;
	std::cout << "-----------------------" << std::endl;
}

