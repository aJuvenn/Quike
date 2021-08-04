/*
 * quike_header.h
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_HEADER_HPP_
#define QUIKE_HEADER_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <set>
#include <iostream>

#include <Eigen/Dense>

using namespace Eigen;

#include "Player/quike_camera.hpp"
#include "Rendering/quike_draw.hpp"
#include "Player/quike_events.hpp"
#include "Map/quike_map.hpp"
#include "Player/quike_movable.hpp"
#include "Map/quike_picture.hpp"
#include "Player/quike_player.hpp"
#include "Rendering/quike_scene_render.hpp"
#include "Utils/quike_utils.hpp"
#include "Physics/quike_solid.hpp"
#include "Physics/quike_aabb_collision.hpp"

#endif /* QUIKE_HEADER_HPP_ */
