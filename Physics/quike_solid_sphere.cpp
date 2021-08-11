/*
 * quike_rigid_sphere.cpp
 *
 *  Created on: 7 août 2021
 *      Author: ajuvenn
 */

#include "../quike_header.hpp"



SolidSphere::SolidSphere(const double radius, const double mass)
: Solid(mass, Matrix3d::Identity() * (2. * mass * radius * radius / 5.)),
  radius(radius)
{
	updateAABB();
}


double SolidSphere::getRadius() const
{
	return radius;
}


Vector3d SolidSphere::getClosestPoint(const Vector3d & direction) const
{
	return centerPosition + direction * radius;
}


void SolidSphere::print() const
{
	std::cout << "Sphere of radius " << radius << " at position (";
	std::cout << centerPosition(0) << "," << centerPosition(1) << "," << centerPosition(2)  << ")";
	std::cout << std::endl;
}

#define QK_SPHERE_DRAW_SLICES 5

void SolidSphere::glDraw() const
{
	glColor3dv(color.data());
	glPushMatrix();
	Matrix4d transfMatrix = getGlTransfMatrix();
	glMultMatrixd(transfMatrix.data());
	glutSolidSphere(radius, QK_SPHERE_DRAW_SLICES, QK_SPHERE_DRAW_SLICES);
	glPopMatrix();
}


SolidSphere::~SolidSphere()
{

}
