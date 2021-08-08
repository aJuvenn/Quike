/*
 * quike_solid_points.cpp
 *
 *  Created on: 8 août 2021
 *      Author: ajuvenn
 */

#include "../quike_header.hpp"


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


SolidPoints::SolidPoints(const Matrix3Xd & points, const VectorXd & masses)
: Solid(masses.sum(), Matrix3d::Identity()),
  centeredInitialPoints(),
  masses(masses),
  nbPoints(points.cols()),
  currentPoints(points)
{
	const Vector3d center = (points.array().rowwise() * masses.transpose().array()).rowwise().sum() / totalMass;
	centeredInitialPoints = points.colwise() - center;
	initialInertiaMatrix = inertiaMatrix(centeredInitialPoints, masses);
	initialInertiaMatrixInverse = initialInertiaMatrix.inverse();
	displace(center);
	updateAABB();
}


SolidPoints::~SolidPoints()
{

}


Matrix3Xd SolidPoints::computeCurrentPoints() const
{
	return (rotationMatrix * centeredInitialPoints).colwise() + centerPosition;
}


const Matrix3Xd & SolidPoints::getInitialPoints() const
{
	return initialPoints;
}


const VectorXd & SolidPoints::getMasses() const
{
	return masses;
}


size_t SolidPoints::getNbPoints() const
{
	return nbPoints;
}


Vector3d SolidPoints::getClosestPoint(const Vector3d & direction)
{
	const RowVector3d rotatedDirectionT = (rotationMatrix.transpose() * direction).transpose();
	const RowVectorXd scalarProducts = rotatedDirectionT * centeredInitialPoints;
	size_t maxIndex;
	scalarProducts.maxCoeff(&maxIndex);
	return centerPosition + rotationMatrix * centeredInitialPoints.col(maxIndex);
}


void SolidPoints::print() const
{
	std::cout << "Points solid" << std::endl;
}


void SolidPoints::glDraw() const
{
	glPushMatrix();
	glMultMatrixd(getGlTransfMatrix().data());

	glColor3dv(color.data());
	glBegin(GL_POLYGON);

	for (size_t i = 0 ; i < nbPoints ; i++){
		glVertex3dv(centeredInitialPoints.col(i).data());
	}

	glEnd();
	glPopMatrix();
}


