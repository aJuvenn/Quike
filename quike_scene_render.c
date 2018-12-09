/*
 * quike_scene_render.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */


#include "quike_header.h"


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






void qkLookAtPlayerCamera()
{
	QKCamera * cam = qkGlobalPlayer->camera;

	glLoadIdentity();
	gluLookAt(
			cam->position[0], cam->position[1], cam->position[2], // eye
			cam->center[0], cam->center[1], cam->center[2], // center
			0., 0., 1. // up
	);
}


void qkSceneRenderHandler()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	qkLookAtPlayerCamera();
	qkDrawMap();

	glutSwapBuffers();
}


void qkPeriodicSceneRender(int period)
{
	GLdouble dt = 0.001/((GLdouble) period);
	qkPlayerMove(qkGlobalPlayer, dt);

	glutPostRedisplay();

	glutTimerFunc(period, qkPeriodicSceneRender, period);
}






