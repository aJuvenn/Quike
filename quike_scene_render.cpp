/*
 * quike_scene_render.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */


#include "quike_header.hpp"


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
	qkGlobalSolid->glDraw();
	qkDrawMap();

	glutSwapBuffers();
}


void qkPeriodicSceneRender(int period)
{
	double dt = 0.001/((double) period);
	qkGlobalPlayer->move(dt);

	Vector3d totalForce(100000., 0., 0.);
	Vector3d totalMomentum(10000000., 0., 0.);

	qkGlobalSolid->update(totalForce, totalMomentum, dt);

	glutPostRedisplay();

	glutTimerFunc(period, qkPeriodicSceneRender, period);
}
