/*
 * quike_main.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */


#include "quike_header.hpp"

Solid * randomSolid(double x, double y, double z, double width, size_t nbPoints)
{
	VectorXd masses(nbPoints);
	masses.setConstant(1.);

	Matrix3Xd points(3, nbPoints);
	points = Matrix3Xd::Random(3, nbPoints) * width;
	Vector3d offset(x, y, z);
	points.colwise() += offset;

	return new Solid(points, masses);
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

	Solid * solids[8];
	solids[0] = randomSolid(10., 20., 3., 2., 10);
	solids[1] = randomSolid(20., 10., 3., 2., 10);
	solids[2] = randomSolid(10., 10., 3., 2., 10);
	solids[3] = randomSolid(20., 20., 3., 2., 10);
	solids[4] = randomSolid(10., 40., 3., 2., 10);
	solids[5] = randomSolid(20., 30., 3., 2., 10);
	solids[6] = randomSolid(10., 30., 3., 2., 10);
	solids[7] = randomSolid(20., 40., 3., 2., 10);

	for (int i = 0 ; i < 8 ; ++i){
		qkGlobalSolidList.push_back(solids[i]);
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
