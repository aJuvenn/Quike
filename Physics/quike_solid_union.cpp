/*
 * quike_solid_union.cpp
 *
 *  Created on: 11 août 2021
 *      Author: ajuvenn
 */

#include "../quike_header.hpp"


double qkSolidListMass(const std::vector<Solid *> & solids)
{
	double mass = 0.;

	for (const Solid * const s : solids){
		mass += s->getMass();
	}

	return mass;
}


Vector3d qkSolidListCenterOfMass(const std::vector<Solid *> & solids)
{
	Vector3d output(0., 0., 0.);
	const double totalMass = qkSolidListMass(solids);

	for (const Solid * const s : solids){
		output += s->getMass() * s->getCenterPosition();
	}

	return output / totalMass;
}


Matrix3d qkSolidListInertiaMatrix(const std::vector<Solid *> & solids)
{
	Matrix3d output;
	output.setZero();
	const Vector3d centerOfMass = qkSolidListCenterOfMass(solids);
	const Matrix3d I3 = Matrix3d::Identity();

	for (const Solid * const s : solids){
		const Vector3d c = s->getCenterPosition() - centerOfMass;
		const double m = s->getMass();
		output += s->getInertiaMatrix() + m * (c.dot(c) * I3 - c * c.transpose());
	}

	return output;
}


SolidUnion::SolidUnion(std::vector<Solid *> & solids)
: Solid(qkSolidListMass(solids), qkSolidListInertiaMatrix(solids)),
  nbSolids(solids.size()),
  solids(solids)
{
	const Vector3d initialCenter = qkSolidListCenterOfMass(solids);

	for (size_t i = 0 ; i < nbSolids ; ++i){
		solids[i]->displace(-initialCenter);
	}

	displace(initialCenter);
	updateAABB();
}


SolidUnion::~SolidUnion()
{

}


Vector3d SolidUnion::getClosestPoint(const Vector3d & direction) const
{
	const Vector3d rotatedDirection = rotationMatrix.transpose() * direction;
	Vector3d output;
	double maxDotProduct;
	bool firstLoop = true;

	for (const Solid * const s : solids){
		const Vector3d relativeClosest = s->getClosestPoint(rotatedDirection);
		const Vector3d closest = centerPosition +  rotationMatrix * relativeClosest;
		const double dotProduct = closest.dot(direction);
		if (firstLoop || dotProduct > maxDotProduct){
			maxDotProduct = dotProduct;
			output = closest;
		}
		firstLoop = false;
	}

	return output;
}


void SolidUnion::print() const
{
	std::cout << "Solid Union" << std::endl;
}



void SolidUnion::glDraw() const
{
	glPushMatrix();
	glMultMatrixd(getGlTransfMatrix().data());

	for (const Solid * const s : solids){
		s->glDraw();
	}

	glPopMatrix();
}
