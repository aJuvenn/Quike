/*
 * quike_camera.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.h"



QKCamera * qkCameraNew(const GLdouble Position[3], const GLdouble theta, const GLdouble phi)
{
	QKCamera * output;
	QK_SIMPLE_ALLOC(output, 1);

	qkCameraUpdatePosition(output, Position);
	qkCameraUpdateDirection(output, theta, phi);

	return output;
}


void qkCameraFree(QKCamera * const cam)
{
	free(cam);
}


void qkCameraUpdatePosition(QKCamera * const cam, const GLdouble newPosition[3])
{
	for (int i = 0 ; i < 3 ; i++){
		cam->position[i] = newPosition[i];
		cam->center[i] = newPosition[i] + cam->direction[i];
	}
}


void qkCameraIncreasePosition(QKCamera * const cam, const GLdouble dPosition[3])
{
	for (int i = 0 ; i < 3 ; i++){
		cam->position[i] += dPosition[i];
		cam->center[i] = cam->position[i] + cam->direction[i];
	}
}


#define QK_CAMERA_VERTICAL_BOUND 0.1

void qkCameraUpdateDirection(QKCamera * const cam, const GLdouble newTheta, const GLdouble newPhi)
{

	cam->theta = newTheta;

	if (newPhi < QK_CAMERA_VERTICAL_BOUND){
		cam->phi = QK_CAMERA_VERTICAL_BOUND;
	} else if (newPhi > M_PI - QK_CAMERA_VERTICAL_BOUND) {
		cam->phi = M_PI - QK_CAMERA_VERTICAL_BOUND;
	} else {
		cam->phi = newPhi;
	}

	GLdouble sinPhi = sin(cam->phi);

	cam->floorDirection[0] = cam->direction[0] = sinPhi * cos(newTheta);
	cam->floorDirection[1] = cam->direction[1] = sinPhi * sin(newTheta);
	cam->direction[2] = cos(newPhi);
	cam->floorDirection[2] = 0.;
	qkVectNormalize(cam->floorDirection);

	cam->floorOrthoDirection[0] = -cam->floorDirection[1];
	cam->floorOrthoDirection[1] = cam->floorDirection[0];
	cam->floorOrthoDirection[2] = 0;

	for (int i = 0 ; i < 3 ; i++){
		cam->center[i] = cam->position[i] + cam->direction[i];
	}

}


void qkCameraIncreaseDirection(QKCamera * const cam, const GLdouble dTheta, const GLdouble dPhi)
{
	qkCameraUpdateDirection(cam, cam->theta + dTheta, cam->phi + dPhi);
}
