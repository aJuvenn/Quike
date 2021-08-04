/*
 * quike_events.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_EVENTS_HPP_
#define QUIKE_EVENTS_HPP_

#include "../quike_header.hpp"


typedef struct {

	int forward;
	int backward;
	int left;
	int right;

	int dMouseX;
	int dMouseY;

} QKPlayerEvent;


void qkPlayerEventReset(QKPlayerEvent * ev);


void qkKeyboardEventHandler(unsigned char c, int x, int y);
void qkKeyboardUpEventHandler(unsigned char c, int x, int y);

void qkPassiveMotionEventHandler(int x, int y);
void qkMotionEventHandler(int x, int y);


void qkInitMouseCentering();
void qkUpdateMouseCenter(int windowWidth, int windowHeight);


void qkPeriodicMoveUpdate(int period);

#endif /* QUIKE_EVENTS_HPP_ */
