/*
 * quike_solid.cpp
 *
 *  Created on: 2 août 2021
 *      Author: ajuvenn
 */

#include "../quike_header.hpp"


std::vector<Solid *> qkGlobalSolidList;
AabbCollisionDetector * qkGlobalAabbCollisionDetector;

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
																			totalMass(masses.sum()),
																			totalMassInverse(1. / totalMass),
																			centerPosition((points.array().rowwise() * masses.transpose().array()).rowwise().sum() / totalMass),
																			centerVelocity(0., 0., 0.),
																			centeredInitialPoints(points.colwise() - centerPosition),
																			initialInertiaMatrix(inertiaMatrix(centeredInitialPoints, masses)),
																			initialInertiaMatrixInverse(initialInertiaMatrix.inverse()),
																			rotationQuaternion(1., 0., 0., 0.),
																			rotationMatrix(rotationQuaternion.toRotationMatrix()),
																			angularMomentum(0., 0., 0.),
																			currentPoints(points)
{
	updateAABB();
}


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
	const Matrix3d & R = rotationMatrix;

	/* Inertia matrix inverse is obtained from the initial one by a change of bases */
	const Matrix3d currentInertiaMatrixInverse = R * initialInertiaMatrixInverse * R.transpose();

	/* Angular speed vector is obtained from the momentum and the inertia */
	const Vector3d angularSpeed = currentInertiaMatrixInverse * angularMomentum;
	Quaterniond angularSpeedAsQuaternion(0., angularSpeed(0), angularSpeed(1), angularSpeed(2));

	/* Rotation quaternion integration from a derivative computed with a quaternion multiplication */
	const Quaterniond tmp = angularSpeedAsQuaternion * rotationQuaternion;
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
	updateCurrentPoints();
	updateAABB();
}


void Solid::update(const Matrix3Xd & forcePerPoint, const double dt)
{
	Vector3d totalForce = forcePerPoint.rowwise().sum();
	Matrix3Xd relativePositions = rotationMatrix * centeredInitialPoints;
	Vector3d totalForceMomentum;
	totalForceMomentum.setZero();

	for (size_t i = 0 ; i < nbPoints ; i++){
		totalForceMomentum += relativePositions.col(i).cross(forcePerPoint.col(i));
	}

	update(totalForce, totalForceMomentum, dt);
}


const Matrix3Xd & Solid::getCurrentPoints() const
{
	return currentPoints;
}


void Solid::updateCurrentPoints()
{
	currentPoints = (rotationMatrix * centeredInitialPoints).colwise() + centerPosition;
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
	std::cout << "currentPoints:\n" << currentPoints << std::endl;
	std::cout << "-----------------------" << std::endl;
}


void Solid::glDraw() const
{
	const Matrix3Xd & points = getCurrentPoints();

	glColor3f(1., 0., 0.);
	glBegin(GL_POLYGON);

	for (size_t i = 0 ; i < nbPoints ; i++){
		glVertex3d(points(0, i), points(1, i), points(2, i));
	}

	glEnd();
}


void Solid::updateAABB()
{
	const Matrix3Xd & points = getCurrentPoints();

	minAndMaxCoords.col(0) = points.rowwise().minCoeff();
	minAndMaxCoords.col(1) = points.rowwise().maxCoeff();

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


const Vector3d & Solid::getCenterPosition() const
{
	return centerPosition;
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


const Matrix<double, 3, 8> & Solid::getAabbCornerPoints() const
{
	return aabbCornerPoints;
}





AabbCollisionDetector * qkAabbCollisionDetector;

const Vector3d & AabbCollisionDetector::getCurrentProjectionAxis() const
{
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



void insertionSort(std::vector<AabbBound> & v)
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


const AabbCollisionDetector::CollisionList & AabbCollisionDetector::getCollisions() const
{
	return collisions;
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

	insertionSort(aabbBoundList);

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


bool isInACollision(const Solid * const s, const std::vector<std::pair<Solid *, Solid *>> & collisions)
{
	for (const std::pair<Solid *, Solid *> & pair : collisions){
		if (pair.first == s || pair.second == s){
			return true;
		}
	}
	return false;
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

const std::vector<Solid *> & AabbCollisionDetector::getSolids() const
{
	return solids;
}


AabbCollisionDetector::AabbCollisionDetector():
										nbSolids(0),
										solids(),
										currentProjectionAxis(1., 0., 0.),
										aabbBoundList(),
										collisions(),
										collidingSolids()
{

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



const std::set<Solid *> & AabbCollisionDetector::getCollidingSolids() const
{
	return collidingSolids;
}


bool AabbCollisionDetector::solidIsColliding(Solid & solid) const
{
	return (collidingSolids.find(&solid) != collidingSolids.end());
}





