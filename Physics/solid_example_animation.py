# -*- coding: utf-8 -*-

import numpy as np
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from solid import Solid

ux = np.array([1.,0.,0.])
uy = np.array([0.,1.,0.])
uz = np.array([0.,0.,1.])

  
d = 0.25
centers = [0. * ux, ux, uy, uz, 2. * uz, 3. * uz, 4. * uz]

points = []

for c in centers:
    points.append(c + np.array([-d,-d,-d]))
    points.append(c + np.array([d,-d,-d]))
    points.append(c + np.array([d,d,-d]))
    points.append(c + np.array([-d,d,-d]))
    points.append(c + np.array([-d,d,d]))
    points.append(c + np.array([-d,-d,d]))
    points.append(c + np.array([d,-d,d]))
    points.append(c + np.array([d,d,d]))

s = Solid(points, len(points) * [2.])  
s.print()


force = np.array([0.,0.,100.])
force_moment = np.array([0.,1000.,0.])
dt = 0.01
T = 1.


fig = plt.figure()
ax = fig.add_subplot(1,1,1,projection='3d')
D = 10
ax.set_xlim3d(-D, D)
ax.set_ylim3d(-D, D)
ax.set_zlim3d(-D, D)

previous_plots = None


def Array(x):
    return np.asarray(x).reshape(-1)

def update(t):
    
    global previous_plots
    global s
    global ax
    
    s.update_center_position(force, dt)
    s.update_rotation_quaternion(force_moment, dt)
    points_hstack = s.points_hstack()
    
    if previous_plots is not None:
        previous_plots[0].remove()
        
    previous_plots = ax.plot3D(Array(points_hstack[0, :]), 
                               Array(points_hstack[1, :]),
                               Array(points_hstack[2, :]),
                               color = 'black')
    if True:
        s.print()
    
    

ani = FuncAnimation(fig, update, 
                    frames = np.arange(0., T, dt), 
                    interval=20)
        
plt.show()




