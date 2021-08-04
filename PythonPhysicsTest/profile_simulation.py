# -*- coding: utf-8 -*-

import numpy as np
from solid import Solid
from box_collision import BoxCollisionDetector
import cProfile

ux = np.array([1.,0.,0.])
uy = np.array([0.,1.,0.])
uz = np.array([0.,0.,1.])


def create_solid(centers, offset = np.array([0.,0.,0.]), d = 0.25):
    
    points = []

    for c in centers:
        points.append(c + offset + np.array([-d,-d,-d]))
        points.append(c + offset + np.array([d,-d,-d]))
        points.append(c + offset + np.array([d,d,-d]))
        points.append(c + offset + np.array([-d,d,-d]))
        points.append(c + offset + np.array([-d,d,d]))
        points.append(c + offset + np.array([-d,-d,d]))
        points.append(c + offset + np.array([d,-d,d]))
        points.append(c + offset + np.array([d,d,d]))
    
    return Solid(points, len(points) * [2.])  



def Array(x):
    return np.asarray(x).reshape(-1)


def AABB_to_points(AABB):

    x = np.array([np.array([AABB[0,x_i], AABB[1,y_i], AABB[2,z_i]]) 
            for x_i in range(2) 
            for y_i in range(2) 
            for z_i in range(2)])
    x.sort()
    return x
    
    

def rectangle_points(AABB):
    
    min_x, max_x, min_y, max_y, min_z, max_z = AABB[0,0], AABB[0,1], AABB[1,0], AABB[1,1], AABB[2,0], AABB[2,1]
    
    points = []

    points.append(np.array([min_x, max_y, min_z]))
    points.append(np.array([min_x, min_y, min_z]))
    points.append(np.array([max_x, min_y, min_z]))
    points.append(np.array([max_x, max_y, min_z]))
    points.append(np.array([min_x, max_y, min_z]))
    points.append(np.array([min_x, max_y, max_z]))
    points.append(np.array([min_x, min_y, max_z]))
    points.append(np.array([max_x, min_y, max_z]))
    points.append(np.array([max_x, max_y, max_z]))
    points.append(np.array([min_x, max_y, max_z]))
    points.append(np.array([min_x, min_y, max_z]))
    points.append(np.array([max_x, min_y, max_z]))
    points.append(np.array([max_x, min_y, min_z]))
    points.append(np.array([max_x, max_y, min_z]))
    points.append(np.array([max_x, max_y, max_z]))
    points.append(np.array([min_x, max_y, max_z]))
    points.append(np.array([min_x, min_y, max_z]))
    points.append(np.array([min_x, min_y, min_z]))
    
    return np.array(points)

  
D = 200
    
nb_solids = 10000
solids = []
forces = []
force_moments = []

for i in range(nb_solids):
    
    rand_position = np.random.uniform(-0.8* D, 0.8 * D, size = 3)
    rand_force = np.random.uniform(-100., 100., size = 3)
    rand_force_moment = np.random.uniform(-500., 500., size = 3)

    s = create_solid([0. * ux, ux, uy, uz, 2. * uz],
                     offset = rand_position)
    
    solids.append(s)
    forces.append(rand_force)
    force_moments.append(rand_force_moment)
    

collision_detector = BoxCollisionDetector(solids)




def sim(solids, forces, force_moments, nb_steps, dt):
    
    for t in range(nb_steps):
    
        for i, s in enumerate(solids):
            s.update(forces[i], force_moments[i], dt)
            
        print(collision_detector.colliding_boxes())
    
    
dt = 0.01    
nb_steps = 15

cProfile.run('sim(solids, forces, force_moments, nb_steps, dt)', sort = 'cumulative')




