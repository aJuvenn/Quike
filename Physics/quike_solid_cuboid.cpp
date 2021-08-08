/*
 * quike_rigid_cuboid.cpp
 *
 *  Created on: 8 août 2021
 *      Author: ajuvenn
 */

#include "../quike_header.hpp"


Matrix3d solidCuboidInertiaMatrix(const double widthX, const double widthY, const double widthZ, const double mass)
{
	Matrix3d output;
	const double widthX2 = widthX * widthX;
	const double widthY2 = widthY * widthY;
	const double widthZ2 = widthZ * widthZ;
	output.setZero();
	output(0,0) = widthY2 + widthZ2;
	output(1,1) = widthX2 + widthZ2;
	output(2,2) = widthX2 + widthY2;
	return output * (mass / 12.);
}


SolidCuboid::SolidCuboid(const double widthX, const double widthY, const double widthZ, const double mass)
: Solid(mass, solidCuboidInertiaMatrix(widthX, widthY, widthZ, mass)),
  widthX(widthX), widthY(widthY), widthZ(widthZ)
{
	updateAABB();
}


SolidCuboid::~SolidCuboid()
{

}

double SolidCuboid::getWidthX() const
{
	return widthX;
}


double SolidCuboid::getWidthY() const
{
	return widthY;
}


double SolidCuboid::getWidthZ() const
{
	return widthZ;
}


Vector3d SolidCuboid::getClosestPoint(const Vector3d & direction)
{
	const Vector3d rotatedDirection = rotationMatrix.transpose() * direction;
	const Vector3d rotatedOutput = Vector3d(0.5 * widthX * ((rotatedDirection(0) >= 0.) ? 1. : -1.),
											0.5 * widthY * ((rotatedDirection(1) >= 0.) ? 1. : -1.),
											0.5 * widthZ * ((rotatedDirection(2) >= 0.) ? 1. : -1.));
	return centerPosition + rotationMatrix * rotatedOutput;
}


void SolidCuboid::print() const
{
	std::cout << "Cuboid of dimensions (" << widthX << "," << widthY << "," << widthZ << ")";
	std::cout << " at position (" << centerPosition(0) << "," << centerPosition(1) << "," << centerPosition(2) << ")";
	std::cout << std::endl;
}


void SolidCuboid::glDraw() const
{
	glColor3dv(color.data());
	glPushMatrix();

	glMultMatrixd(getGlTransfMatrix().data());

	glBegin(GL_POLYGON);

	glVertex3d(0.5 * widthX, -0.5 * widthY, -0.5 * widthZ);
	glVertex3d(0.5 * widthX, -0.5 * widthY, 0.5 * widthZ);
	glVertex3d(0.5 * widthX, 0.5 * widthY, 0.5 * widthZ);
	glVertex3d(0.5 * widthX, 0.5 * widthY, -0.5 * widthZ);

	glVertex3d(-0.5 * widthX, -0.5 * widthY, -0.5 * widthZ);
	glVertex3d(-0.5 * widthX, -0.5 * widthY, 0.5 * widthZ);
	glVertex3d(-0.5 * widthX, 0.5 * widthY, 0.5 * widthZ);
	glVertex3d(-0.5 * widthX, 0.5 * widthY, -0.5 * widthZ);

	glVertex3d(-0.5 * widthX, 0.5 * widthY, -0.5 * widthZ);
	glVertex3d(-0.5 * widthX, 0.5 * widthY, 0.5 * widthZ);
	glVertex3d(0.5 * widthX, 0.5 * widthY, 0.5 * widthZ);
	glVertex3d(0.5 * widthX, 0.5 * widthY, -0.5 * widthZ);

	glVertex3d(-0.5 * widthX, -0.5 * widthY, -0.5 * widthZ);
	glVertex3d(-0.5 * widthX, -0.5 * widthY, 0.5 * widthZ);
	glVertex3d(0.5 * widthX, -0.5 * widthY, 0.5 * widthZ);
	glVertex3d(0.5 * widthX, -0.5 * widthY, -0.5 * widthZ);

	glVertex3d(-0.5 * widthX, -0.5 * widthY, 0.5 * widthZ);
	glVertex3d(-0.5 * widthX, 0.5 * widthY, 0.5 * widthZ);
	glVertex3d(0.5 * widthX, 0.5 * widthY, 0.5 * widthZ);
	glVertex3d(0.5 * widthX, -0.5 * widthY, 0.5 * widthZ);

	glVertex3d(-0.5 * widthX, -0.5 * widthY, -0.5 * widthZ);
	glVertex3d(-0.5 * widthX, 0.5 * widthY, -0.5 * widthZ);
	glVertex3d(0.5 * widthX, 0.5 * widthY, -0.5 * widthZ);
	glVertex3d(0.5 * widthX, -0.5 * widthY, -0.5 * widthZ);

	glEnd();
	glPopMatrix();
}


