/*
 * quike_aabb_collision.hpp
 *
 *  Created on: 4 août 2021
 *      Author: ajuvenn
 */


#ifndef PHYSICS_QUIKE_AABB_COLLISION_HPP_
#define PHYSICS_QUIKE_AABB_COLLISION_HPP_

#include "../quike_header.hpp"


struct AabbBound
{
	size_t solidId;
	int minOrMaxId;
	double value;
};


class AabbCollisionDetector
{
public:
	typedef std::vector<std::pair<Solid *, Solid *>> CollisionList;

private:
	size_t nbSolids;
	std::vector<Solid *> solids;

	Vector3d currentProjectionAxis;
	const Vector3d & computeProjectionAxis();

	std::vector<AabbBound> aabbBoundList;
	CollisionList collisions;
	std::set<Solid *> collidingSolids;

public:

	const std::set<Solid *> & getCollidingSolids() const;

	bool solidIsColliding(Solid & solid) const;

	const Vector3d & getCurrentProjectionAxis() const;

	void glDrawCurrentAxis(const Vector3d & position, const double len) const;

	AabbCollisionDetector();

	void addSolid(Solid * solid);
	const std::vector<Solid *> & getSolids() const;

	const CollisionList & getCollisions() const;
	void computeCollisions();

};


extern AabbCollisionDetector * qkGlobalAabbCollisionDetector;


#endif /* PHYSICS_QUIKE_AABB_COLLISION_HPP_ */
