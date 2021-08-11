/*
 * quike_main.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.hpp"


SolidPoints * randomRigidPoints(const Vector3d & position, double width, size_t nbPoints)
{
	VectorXd masses(nbPoints);
	masses.setConstant(1.);

	Matrix3Xd points(3, nbPoints);
	points = Matrix3Xd::Random(3, nbPoints) * width;
	points.colwise() += position;

	return new SolidPoints(points, masses);
}


int main(int argc, char *argv[])
{
	if (argc != 2){
		fprintf(stderr, "Usage : %s bitmap_file\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (qkLoadNewMapFromBitmap(argv[1]) == EXIT_FAILURE){
		fprintf(stderr, "Couldn't load map from file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	Player p = Player(10., 40.);
	p.setGlobalPlayer();

	const size_t nbPoints = 10;
	const size_t nbRigidBodies = 8;
	const double solidSizes = 2.;
	const Array3d center(20., 20., 3.);
	const Array3d sigma(20., 20., 1.);

	for (size_t i = 0 ; i < nbRigidBodies ; i++){
		Vector3d coords = Array3d::Random() * sigma + center;
		qkGlobalSolidList.push_back(randomRigidPoints(coords, solidSizes, nbPoints));
	}

	const size_t radius = 2.;

	for (size_t i = 0 ; i < nbRigidBodies ; i++){
		Vector3d coords = Array3d::Random() * sigma + center;
		SolidCuboid * s = new SolidCuboid(radius, 2. * radius, 0.5 * radius, 1.);
		s->displace(coords);
		qkGlobalSolidList.push_back(s);
	}

	for (size_t i = 0 ; i < nbRigidBodies ; i++){
		Vector3d coords = Array3d::Random() * sigma + center;
		SolidSphere * s = new SolidSphere(radius, 1.);
		s->displace(coords);
		qkGlobalSolidList.push_back(s);
	}


	Solid * sphere = new SolidSphere(1., 1.);
	sphere->displace(Vector3d(2.,1.,0.5));
	Solid * cuboid = new SolidCuboid(1., 1., 3., 1.);
	cuboid->rotateX(M_PI_4);
	cuboid->displace(Vector3d(0., sqrt(2.) * 1.5/2. + 1., sqrt(2.) * 1.5/2. - 1.));
	cuboid->displace(Vector3d(2.,1.,0.5));
	std::vector<Solid *> solidUnionList;
	solidUnionList.push_back(sphere);
	solidUnionList.push_back(cuboid);
	Solid * total = new SolidUnion(solidUnionList);
	total->displace(Vector3d(20., 20., 5.));

	qkGlobalSolidList.push_back(total);

	AabbCollisionDetector d;
	qkGlobalAabbCollisionDetector = &d;

	for (Solid * s : qkGlobalSolidList){
		d.addSolid(s);
	}

	qkStartGlLoop(&argc, argv);

	return EXIT_SUCCESS;
}
