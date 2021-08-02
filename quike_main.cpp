/*
 * quike_main.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */


#include "quike_header.hpp"



#define QK_FRAME_PERIOD 40

int main(int argc, char *argv[])
{
	if (argc != 2){
		fprintf(stderr, "Usage : %s bitmap_file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	Player p = Player(10,10);
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

	VectorXd masses(5);
	masses << 1., 1., 1., 1., 1.;

	Matrix3Xd points(3, 5);
	points << 10.,12., 14., 16., 12.,
			  10., 14., 12., 9., 8.,
			  2.5, 3., 2.7, 2.4, 3.;

	Solid s(points, masses);
	s.print();
	qkGlobalSolid = &s;

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
