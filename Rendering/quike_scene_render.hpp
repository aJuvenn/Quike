/*
 * quike_scene_render.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#ifndef DEBUG_QUIKE_SCENE_RENDER_H_
#define DEBUG_QUIKE_SCENE_RENDER_H_

#include "../quike_header.hpp"


void qkWindowReshapeHandler(int w, int h);

void qkSceneRenderHandler();

void qkPeriodicSceneRender(int period);

#endif /* DEBUG_QUIKE_SCENE_RENDER_H_ */
