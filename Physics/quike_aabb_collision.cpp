/*
 * quike_aabb_collision.cpp
 *
 *  Created on: 4 août 2021
 *      Author: ajuvenn
 */
#include "../quike_header.hpp"


AabbCollisionDetector * qkAabbCollisionDetector;


AabbCollisionDetector::AabbCollisionDetector():
										nbSolids(0),
										solids(),
										currentProjectionAxis(1., 0., 0.),
										aabbBoundList(),
										collisions(),
										collidingSolids()
{

}


const Vector3d & AabbCollisionDetector::getCurrentProjectionAxis() const
{
	return currentProjectionAxis;
}


const AabbCollisionDetector::CollisionList & AabbCollisionDetector::getCollisions() const
{
	return collisions;
}


const std::vector<Solid *> & AabbCollisionDetector::getSolids() const
{
	return solids;
}


const std::set<Solid *> & AabbCollisionDetector::getCollidingSolids() const
{
	return collidingSolids;
}


bool AabbCollisionDetector::solidIsColliding(Solid & solid) const
{
	return (collidingSolids.find(&solid) != collidingSolids.end());
}


void AabbCollisionDetector::addSolid(Solid * solid)
{
	solids.push_back(solid);
	AabbBound b = (AabbBound) {nbSolids, 0, 0.};
	aabbBoundList.push_back(b);
	b = (AabbBound) {nbSolids, 1, 0.};
	aabbBoundList.push_back(b);
	++nbSolids;
}


const Vector3d & AabbCollisionDetector::computeProjectionAxis()
{
	Matrix3Xd solidCenters(3, nbSolids);

	for (size_t i = 0 ; i < nbSolids ; ++i){
		solidCenters.col(i) = solids[i]->getCenterPosition();
	}

	Vector3d center = solidCenters.rowwise().mean();
	solidCenters.colwise() -= center;

	Matrix3d correlationMatrix = (solidCenters * solidCenters.transpose()) / nbSolids;
	SelfAdjointEigenSolver<Matrix3d> solver(correlationMatrix);

	const Vector3d & eigenvector = solver.eigenvectors().col(2);

	if (currentProjectionAxis.dot(eigenvector) > 0){
		currentProjectionAxis = eigenvector;
	} else {
		currentProjectionAxis = -eigenvector;
	}

	return currentProjectionAxis;
}


void AabbCollisionDetector::glDrawCurrentAxis(const Vector3d & position, const double len) const
{
	glBegin(GL_LINES);
	glColor3d(0., 0., 1.);
	glVertex3dv(position.data());
	glColor3d(0., 1., 0.);
	Vector3d arrowEnd = position + len * currentProjectionAxis;
	glVertex3dv(arrowEnd.data());
	glEnd();
}


void qkAabbBoundInsertionSort(std::vector<AabbBound> & v)
{
	const size_t nbElements = v.size();

	for (size_t i = 1 ; i < nbElements ; ++i){

		AabbBound b = v[i];
		size_t j;

		for (j = i ; j > 0 && v[j-1].value > b.value ; --j){
			v[j] = v[j-1];
		}
		v[j] = b;
	}
}


void AabbCollisionDetector::computeCollisions()
{
	const RowVector3d projectionAxis = computeProjectionAxis().transpose();
	Matrix2Xd minMaxProjections(2, nbSolids);

	for (size_t solidId = 0 ; solidId < nbSolids ; ++solidId){
		Solid * s = solids[solidId];
		const Matrix<double, 1, 8> aabbCornerProjections = projectionAxis * s->getAabbCornerPoints();
		minMaxProjections(0, solidId) = aabbCornerProjections.minCoeff();
		minMaxProjections(1, solidId) = aabbCornerProjections.maxCoeff();
	}

	for (AabbBound & b : aabbBoundList){
		b.value = minMaxProjections(b.minOrMaxId, b.solidId);
	}

	qkAabbBoundInsertionSort(aabbBoundList);

	collisions.clear();
	collidingSolids.clear();
	std::set<size_t> activeSolidIds;

	for (size_t j = 0 ; j < 2 * nbSolids ; ++j){

		AabbBound & current = aabbBoundList[j];

		if (current.minOrMaxId == 0){
			Solid * currentSolid = solids[current.solidId];
			for (size_t activeSolidId : activeSolidIds){
				Solid * activeSolid = solids[activeSolidId];
				if (activeSolid->aabbIntersectsWith(*currentSolid)){
					collisions.push_back(std::make_pair(activeSolid, currentSolid));
					collidingSolids.insert(activeSolid);
					collidingSolids.insert(currentSolid);
				}
			}
			activeSolidIds.insert(current.solidId);
		} else {
			activeSolidIds.erase(current.solidId);
		}
	}
}





