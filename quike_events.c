/*
 * quike_events.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.h"


void qkPlayerEventReset(QKPlayerEvent * ev)
{
	memset(ev, 0, sizeof(QKPlayerEvent));
}


void qkKeyboardEventHandler(unsigned char c, int x, int y)
{
	switch (c){

	case 'z':
		qkGlobalPlayer->events.forward = 1;
		break;

	case 's':
		qkGlobalPlayer->events.backward = 1;
		break;

	case 'q':
		qkGlobalPlayer->events.left = 1;
		break;

	case 'd':
		qkGlobalPlayer->events.right = 1;
		break;

	case 27: // Escape
		exit(EXIT_SUCCESS); // TODO : proper exit
		break;

	default:
		break;

	}
}





static int previousMouseX;
static int previousMouseY;

static int centerMouseX;
static int centerMouseY;


void qkPassiveMotionEventHandler(int x, int y)
{
	int dx = x - previousMouseX;
	int dy = y - previousMouseY;

	qkGlobalPlayer->events.dMouseX += dx;
	qkGlobalPlayer->events.dMouseY += dy;

	previousMouseX = x;
	previousMouseY = y;
}


#define QK_MOUSE_CENTERING_PERIOD 500

void qkUpdateMouseCenter(int windowWidth, int windowHeight)
{
	centerMouseX = windowWidth / 2;
	centerMouseY = windowHeight / 2;
}


void qkMouseCenteringCallback(int period)
{
	previousMouseX = centerMouseX;
	previousMouseY = centerMouseY;
	glutWarpPointer(centerMouseX, centerMouseY);

	glutTimerFunc(period, qkMouseCenteringCallback, period);
}

void qkInitMouseCentering()
{
	qkMouseCenteringCallback(QK_MOUSE_CENTERING_PERIOD);
}


void qkMotionEventHandler(int x, int y)
{
	qkPassiveMotionEventHandler(x, y);
}



void qkPeriodicMoveUpdate(int period)
{
	glutTimerFunc(period, qkPeriodicMoveUpdate, period);
}

