# -*- coding: utf-8 -*-

import numpy as np
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from solid import Solid
from box_collision import BoxCollisionDetector

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

  
D = 10
    
nb_solids = 30
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



dt = 0.01
T = 1.


fig = plt.figure()
ax = fig.add_subplot(1,1,1,projection='3d')

ax.set_xlim3d(-D, D)
ax.set_ylim3d(-D, D)
ax.set_zlim3d(-D, D)


previous_plots = nb_solids * [None]
previous_AABB_plots = nb_solids * [None]

previous_u_plot = None


def update(t):
    
    global previous_plots
    global previous_AABB_plots
    global previous_u_plot
    global ax
    
    for i, s in enumerate(solids):
        s.update(forces[i], force_moments[i], dt)
        
    print(collision_detector.colliding_boxes())
    
    if previous_u_plot is not None:
        previous_u_plot.remove()
        
    u = collision_detector.get_u()
    previous_u_plot = ax.quiver(0, 0, 0, 
                                u[0,0], u[1,0], u[2,0], 
                                length = 5, 
                                normalize = False, 
                                color = 'blue')
    
    
    for i, s in enumerate(solids):  
        
        points_hstack = s.points_hstack()
        
        if previous_plots[i] is not None:
            previous_plots[i][0].remove()
        
        if previous_AABB_plots[i] is not None:
            previous_AABB_plots[i][0].remove()

    
        previous_plots[i] = ax.plot3D(Array(points_hstack[0, :]), 
                                       Array(points_hstack[1, :]),
                                       Array(points_hstack[2, :]),
                                       color = 'black')
        
        rect_points = rectangle_points(s.AABB)
    
        collision_color = 'green'
        
        for j, s2 in enumerate(solids):
            if i == j:
                continue
            if s.AABB_intersect_with(s2):
                collision_color = 'red'
                break
            
        previous_AABB_plots[i] = ax.plot3D(rect_points[:, 0],
                                            rect_points[:, 1],
                                            rect_points[:, 2],
                                            color = collision_color,
                                            linewidth = 1)
    
    

ani = FuncAnimation(fig, update, 
                    frames = np.arange(0., T, dt), 
                    interval=20)
        
plt.show()




