/*
 * quike_scene_render.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */


#include "../quike_header.hpp"


void qkWindowReshapeHandler(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	GLdouble fovy = 60;
	GLdouble aspect = (GLdouble) w / (GLdouble) h;
	GLdouble zNear = 0.2;
	GLdouble zFar = 100;

	gluPerspective(fovy, aspect, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);


	qkUpdateMouseCenter(w, h);
}


void qkSceneRenderHandler()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	qkGlobalPlayer->lookAt();

	Vector3d arrowPosition(20.,20., 5.);
	qkGlobalAabbCollisionDetector->glDrawCurrentAxis(arrowPosition, 10.);

	for (Solid * s : qkGlobalSolidList){
		s->glDraw();
		s->glDrawAabb(qkGlobalAabbCollisionDetector->solidIsColliding(*s));
	}

	qkDrawMap();

	glutSwapBuffers();
}


void qkPeriodicSceneRender(int period)
{
	double dt = 0.001/((double) period);

	static double total_t = 0.;
	total_t += dt;

	qkGlobalPlayer->move(dt);

	Vector3d totalForce(1000000. * sin(1000. * total_t), 0., 0.);
	Vector3d totalMomentum(0., (10000000. * sin(1000. * total_t)), 0.);

	double sign = 1;
	for (Solid * s : qkGlobalSolidList){
		s->applyForce(totalForce * sign, totalMomentum, dt);
		sign *= -1;
	}

	qkGlobalAabbCollisionDetector->computeCollisions();

	glutPostRedisplay();
	glutTimerFunc(period, qkPeriodicSceneRender, period);
}


void qkStartGlLoop(int * argc, char * argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Quike");
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();
	glClearColor(0.0f, 0.7, 0.9, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(qkSceneRenderHandler);
	glutPassiveMotionFunc(qkPassiveMotionEventHandler);
	glutMotionFunc(qkMotionEventHandler);
	glutKeyboardFunc(qkKeyboardEventHandler);
	glutKeyboardUpFunc(qkKeyboardUpEventHandler);
	glutReshapeFunc(qkWindowReshapeHandler);

	qkInitMouseCentering();
	qkPeriodicSceneRender(QK_FRAME_PERIOD);

	glutMainLoop();
}



