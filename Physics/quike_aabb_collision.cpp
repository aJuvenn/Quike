/*
 * quike_aabb_collision.cpp
 *
 *  Created on: 4 août 2021
 *      Author: ajuvenn
 */
#include "../quike_header.hpp"


AabbCollisionDetector * qkGlobalAabbCollisionDetector;


AabbCollisionDetector::AabbCollisionDetector()
: nbSolids(0),
  solids(),
  currentProjectionAxis(1., 0., 0.),
  boundList(),
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


bool AabbCollisionDetector::solidIsColliding(Solid & body) const
{
	return (collidingSolids.find(&body) != collidingSolids.end());
}


void AabbCollisionDetector::addSolid(Solid * body)
{
	solids.push_back(body);
	ProjectedSolidBound b = (ProjectedSolidBound) {nbSolids, 0, 0.};
	boundList.push_back(b);
	b = (ProjectedSolidBound) {nbSolids, 1, 0.};
	boundList.push_back(b);
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


void qkProjectedSolidBoundInsertionSort(std::vector<ProjectedSolidBound> & v)
{
	const size_t nbElements = v.size();

	for (size_t i = 1 ; i < nbElements ; ++i){

		ProjectedSolidBound b = v[i];
		size_t j;

		for (j = i ; j > 0 && v[j-1].value > b.value ; --j){
			v[j] = v[j-1];
		}
		v[j] = b;
	}
}


void AabbCollisionDetector::computeCollisions()
{
	const Vector3d projectionAxis = computeProjectionAxis();
	const Vector3d oppositeProjectionAxis = -projectionAxis;
	const RowVector3d projectionAxisT = projectionAxis.transpose();
	Matrix2Xd minMaxProjections(2, nbSolids);

	for (size_t solidId = 0 ; solidId < nbSolids ; ++solidId){
		Solid * s = solids[solidId];
		minMaxProjections(0, solidId) = projectionAxisT * s->getClosestPoint(oppositeProjectionAxis);
		minMaxProjections(1, solidId) = projectionAxisT * s->getClosestPoint(projectionAxis);
	}

	for (ProjectedSolidBound & b : boundList){
		b.value = minMaxProjections(b.minOrMaxId, b.solidId);
	}

	qkProjectedSolidBoundInsertionSort(boundList);

	collisions.clear();
	collidingSolids.clear();
	std::set<size_t> activeSolidIds;

	for (const ProjectedSolidBound & currentBound : boundList){

		if (currentBound.minOrMaxId == 0){
			Solid * currentSolid = solids[currentBound.solidId];
			for (size_t activeSolidId : activeSolidIds){
				Solid * activeSolid = solids[activeSolidId];
				if (activeSolid->aabbIntersectsWith(*currentSolid)){
					collisions.push_back(std::make_pair(activeSolid, currentSolid));
					collidingSolids.insert(activeSolid);
					collidingSolids.insert(currentSolid);
				}
			}
			activeSolidIds.insert(currentBound.solidId);
		} else {
			activeSolidIds.erase(currentBound.solidId);
		}
	}
}





