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


#define QK_FRAME_PERIOD 40

int main(int argc, char *argv[])
{
	if (argc != 2){
		fprintf(stderr, "Usage : %s bitmap_file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	Player p = Player(10., 40.);
	p.setGlobalPlayer();

	if (qkLoadNewMapFromBitmap(argv[1]) == EXIT_FAILURE){
		fprintf(stderr, "Couldn't load map from file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	glutCreateWindow("Quike");
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();
	glClearColor(0.0f, 0.7, 0.9, 1.0f);
	glEnable(GL_DEPTH_TEST);


	const size_t nbPoints = 10;
	const size_t nbRigidBodies = 10;
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

	AabbCollisionDetector d;
	qkGlobalAabbCollisionDetector = &d;

	for (Solid * s : qkGlobalSolidList){
		d.addSolid(s);
	}

	glutDisplayFunc(qkSceneRenderHandler);
	glutPassiveMotionFunc(qkPassiveMotionEventHandler);
	glutMotionFunc(qkMotionEventHandler);
	glutKeyboardFunc(qkKeyboardEventHandler);
	glutKeyboardUpFunc(qkKeyboardUpEventHandler);
	glutReshapeFunc(qkWindowReshapeHandler);

	qkInitMouseCentering();
	qkPeriodicSceneRender(QK_FRAME_PERIOD);

	glutMainLoop();

	return 0;
}
