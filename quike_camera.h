/*
 * quike_camera.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */


#ifndef QUIKE_CAMERA_H_
#define QUIKE_CAMERA_H_

#include "quike_header.h"


typedef struct {

	GLdouble theta;
	GLdouble phi;
	GLdouble position[3];

	GLdouble center[3];
	GLdouble direction[3];

	GLdouble floorDirection[3];
	GLdouble floorOrthoDirection[3];

} QKCamera;



QKCamera * qkCameraNew(const GLdouble Position[3], const GLdouble theta, const GLdouble phi);
void qkCameraFree(QKCamera * const cam);


void qkCameraUpdatePosition(QKCamera * const cam, const GLdouble newPosition[3]);
void qkCameraIncreasePosition(QKCamera * const cam, const GLdouble dPosition[3]);

void qkCameraUpdateDirection(QKCamera * const cam, const GLdouble newTheta, const GLdouble newPhi);
void qkCameraIncreaseDirection(QKCamera * const cam, const GLdouble dTheta, const GLdouble dPhi);


#endif /* QUIKE_CAMERA_H_ */
