# -*- coding: utf-8 -*-

import numpy.matlib as npmat
import numpy as np
from solid import Solid


def iterate(M, nb_iterations):
    
    w = npmat.asmatrix(npmat.random.uniform(0., 1., size = (3, 1)))
    w /= npmat.linalg.norm(w)
    
    for i in range(nb_iterations):
        w = M * w
        w /= npmat.linalg.norm(w)
                
    return w


class BoxCollisionDetector:
    
    NB_ITERATIONS = 10
    
    def __init__(self, solid_list):

        self.nb_solids = len(solid_list)
        self.solids = solid_list
        self.projected_bounds = None
        
        
        
    def get_u(self):
        points = npmat.hstack([s.center_position for s in self.solids])
        center = npmat.asmatrix(npmat.average(points, axis = 1)).T
        centered_points = points - center
        correlation_matrix = (centered_points * centered_points.T)/self.nb_solids   
        u = iterate(correlation_matrix, self.NB_ITERATIONS)
        return u
        
        
    def colliding_boxes(self):
        
        nb_solids = self.nb_solids
        points = npmat.hstack([s.center_position for s in self.solids])
        center = npmat.asmatrix(npmat.average(points, axis = 1)).T
        centered_points = points - center
        correlation_matrix = (centered_points * centered_points.T) / nb_solids
        u = iterate(correlation_matrix, self.NB_ITERATIONS)

    
        if self.projected_bounds is None:
            
            self.projected_bounds = []
            
            for i in range(nb_solids):
                self.projected_bounds.append((i, 0, 0.))
                self.projected_bounds.append((i, 1, 0.))
                
        
        min_max_projections = npmat.empty((nb_solids, 2))
        
        for solid_id in range(nb_solids):
            solid_AABB_corners = self.solids[solid_id].AABB_corners()
            corners_projections = u.T * solid_AABB_corners
            min_max_projections[solid_id, 0] = np.min(corners_projections)
            min_max_projections[solid_id, 1] = np.max(corners_projections)
            
        for i in range(2 * nb_solids):            
            solid_id, begin_or_end_id, value = self.projected_bounds[i]
            new_value = min_max_projections[solid_id, begin_or_end_id]
            self.projected_bounds[i] = (solid_id, begin_or_end_id, new_value)
                
        # TODO: linear sorting
        self.projected_bounds.sort(key = lambda x : x[2])
        
        output = []
        active_solids = []
    
        for i in range(2 * nb_solids):
            
            solid_id, begin_or_end_id, value = self.projected_bounds[i]
            
            if begin_or_end_id == 0:
                for active_solid_id in active_solids:
                    if self.solids[active_solid_id].AABB_intersect_with(self.solids[solid_id]):
                        output.append((active_solid_id, solid_id))
                        
                active_solids.append(solid_id)
                
            else:
                active_solids.remove(solid_id)
                
        return output
            
        
        
        
        
        
        
        
        
        
        
            
            
        
        
        
        
        
        
        
        
    
    
