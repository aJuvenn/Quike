/*
 * quike_solid.cpp
 *
 *  Created on: 2 août 2021
 *      Author: ajuvenn
 */

#include "quike_header.hpp"


Solid * qkGlobalSolid;


Matrix3d crossProductMatrix(const Vector3d & v)
{
	Matrix3d output;
	output << 0., -v(2), v(1),
			v(2), 0., -v(0),
			-v(1), v(0), 0.;
	return output;
}


Matrix3d inertiaMatrix(const Matrix3Xd & centeredPoints, const VectorXd & masses)
{
	size_t nbPoints = centeredPoints.cols();
	Matrix3d output;
	output.setZero();

	for (size_t i = 0 ; i < nbPoints ; i++){
		Matrix3d pointCrossProductMatrix = crossProductMatrix(centeredPoints.col(i));
		output -= masses(i) * pointCrossProductMatrix * pointCrossProductMatrix;
	}

	return output;
}

Solid::Solid(const Matrix3Xd & points, const VectorXd & masses):
							initialPoints(points),
							masses(masses),
							nbPoints(points.cols()),
							centerPosition(points.rowwise().mean()),
							centerVelocity(0., 0., 0.),
							centeredInitialPoints(points.colwise() - centerPosition),
							totalMass(masses.sum()),
							totalMassInverse(1. / totalMass),
							initialInertiaMatrix(inertiaMatrix(centeredInitialPoints, masses)),
							initialInertiaMatrixInverse(initialInertiaMatrix.inverse()),
							rotationQuaternion(1., 0., 0., 0.),
							rotationMatrix(rotationQuaternion.toRotationMatrix()),
							angularMomentum(0., 0., 0.) {}


void Solid::updatePosition(const Vector3d & totalForce, const double dt)
{
	centerVelocity += dt * totalMassInverse * totalForce;
	centerPosition += dt * centerVelocity;
}


void Solid::updateOrientation(const Vector3d & totalForceMomentum, const double dt)
{
	/* Integration of the force momentum to get the angular momentum */
	angularMomentum += dt * totalForceMomentum;

	/* Rotation matrix corresponding to the current object angle */
	Matrix3d & R = rotationMatrix;

	/* Inertia matrix inverse is obtained from the initial one by a change of bases */
	Matrix3d currentInertiaMatrixInverse = R * initialInertiaMatrixInverse * R.transpose();

	/* Angular speed vector is obtained from the momentum and the inertia */
	Vector3d w = currentInertiaMatrixInverse * angularMomentum;
	Quaterniond wAsQuaternion(0., w(0), w(1), w(2));

	/* Rotation quaternion integration from a derivative computed with a quaternion multiplication */
	Quaterniond tmp = wAsQuaternion * rotationQuaternion;
	rotationQuaternion.w() += dt * 0.5 * tmp.w();
	rotationQuaternion.vec() += dt * 0.5 * tmp.vec();

	/* Normalization of the quaternion to ensure numerical stability through time steps */
	rotationQuaternion.normalize();

	/* Update of the rotation matrix from the new quaternion */
	rotationMatrix = rotationQuaternion.toRotationMatrix();
}


void Solid::update(const Vector3d & totalForce, const Vector3d & totalForceMomentum, const double dt)
{
	updatePosition(totalForce, dt);
	updateOrientation(totalForceMomentum, dt);
}

void Solid::getPoints(Matrix3Xd & outputPoints) const
{
	outputPoints = (rotationMatrix * centeredInitialPoints).colwise() + centerPosition;
}

void Solid::print() const
{
	std::cout << "-------- Solid --------" << std::endl;
	std::cout << "initialPoints:\n" << initialPoints << std::endl;
	std::cout << "masses:\n" << masses << std::endl;
	std::cout << "nbPoints:\n" << nbPoints << std::endl;
	std::cout << "centerPosition:\n" << centerPosition << std::endl;
	std::cout << "centerVelocity:\n" << centerVelocity << std::endl;
	std::cout << "centeredInitialPoints:\n" << centeredInitialPoints << std::endl;
	std::cout << "totalMass:\n" << totalMass << std::endl;
	std::cout << "totalMassInverse:\n" << totalMassInverse << std::endl;
	std::cout << "initialInertiaMatrix:\n" << initialInertiaMatrix << std::endl;
	std::cout << "initialInertiaMatrixInverse:\n" << initialInertiaMatrixInverse << std::endl;
	std::cout << "rotationQuaternion:\n" << rotationQuaternion.w() << std::endl << rotationQuaternion.vec() << std::endl;
	std::cout << "rotationMatrix:\n" << rotationMatrix << std::endl;
	std::cout << "angularMomentum:\n" << angularMomentum << std::endl;
	std::cout << "-----------------------" << std::endl;
}


void Solid::glDraw() const
{
	Matrix3Xd points(3, nbPoints);
	getPoints(points);

	glBegin(GL_POLYGON);

	for (size_t i = 0 ; i < nbPoints ; i++){
		glVertex3d(points(0, i), points(1, i), points(2, i));
	}

	glEnd();
}
