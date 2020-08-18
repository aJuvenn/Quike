import numpy.matlib as npmat
from pprint import pprint


def Vect(l):
    """
        Creates a vector, represented by a matrix column
    """
    return npmat.asmatrix(l).reshape((-1, 1))


def cross_product(x, y):
    """
        Returns a vector which is the cross product of x and y (both vector)
    """
    return Vect(npmat.cross(x.T, y.T))



def quaternion_mult(q1, q2):
    """
        Returns the quaternion product of two quaternions
        represented by a 4 dimensional vector
    """
    s1 = q1[0]
    v1 = q1[1:]
    s2 = q2[0]
    v2 = q2[1:]
    return npmat.vstack([s1 * s2 - v1.T * v2,
                         v2 * s1 + v1 * s2 + cross_product(v1, v2)])
    
    
def rotation_matrix_from_quaternion(q):
    """
        Returns the rotation matrix associated to a unitary
        quaternion represented by a 4 dimensional vector
    """
    a, b, c, d = npmat.asarray(q).reshape(-1)
    a2 = a * a
    b2 = b * b
    c2 = c * c
    d2 = d * d
    
    return npmat.asmatrix([[a2 + b2 - c2 - d2, 2.*(b*c - a*d)   , 2.*(a*c + b*d)],
                           [2.*(a*d + b*c)   , a2 - b2 + c2 - d2, 2.*(c*d - a*b)],
                           [2.*(b*d - a*c)   , 2.*(a*b + c*d)   , a2 - b2 - c2 + d2]])
    
    

def cross_product_matrix(w):
    """
        Returns a matrix representing the linear function (v -> cross_product(w,v))
        where w is a 3 dimensional vector
    """
    x = w[0,0]
    y = w[1,0]
    z = w[2,0]
    return npmat.asmatrix([[0.,-z , y ],
                           [z , 0.,-x ],
                           [-y, x , 0.]])
    
    
     

    
class Solid:
    """
        Perfect solid object represented by a list of points with their masses.
    """
    
    
    def __init__(self, points, masses):
        """
            Inputs:
                points: list of 3 dimensional points
                masses: list of float (same length as points)
        """
        
        self.nb_points = len(points)
        
        if self.nb_points == 0:
            raise(Exception, "Provide at least one point")
        
        if len(masses) != self.nb_points:
            raise(Exception, "Same number of points and masses must be provided")
            
        self.masses = masses
        self.initial_points = [Vect(p) for p in points]

        # Center of mass p and its velocity
        self.center_position = npmat.sum(self.initial_points, axis = 0) / self.nb_points
        self.center_velocity = npmat.zeros((3,1))
        
        # Position of the points relative to the center of mass, stacked into a big matrix
        self.centered_initial_points_hstack = npmat.hstack(self.initial_points) - self.center_position
        
        # Total mass and its inverse
        self.total_mass = npmat.sum(self.masses)
        self.total_mass_inverse = 1./self.total_mass
                
        # Inertia matrix and its inverse
        self.initial_inertia_matrix = Solid._inertia_matrix(self.masses, self.centered_initial_points_hstack)
        self.initial_inertia_matrix_inverse = npmat.linalg.pinv(self.initial_inertia_matrix)
        
        # Rotation quaternion (used to construct matrix representing orientation) and angular momentum (sigma) 
        self.rotation_quaternion = Vect([1.,0.,0.,0.])
        self.rotation_matrix = rotation_matrix_from_quaternion(self.rotation_quaternion)
        self.angular_momentum = npmat.zeros((3,1))
        
        self._compute_AABB()
        
        
        
    def _compute_AABB(self):
        points = self.points_hstack()
        self.AABB = npmat.hstack([points.min(axis = 1), points.max(axis = 1)])
        #npmat.vstack([points.min(axis = 1).T, points.max(axis = 1).T])
        
      
        
    @staticmethod
    def _inertia_matrix(masses, centered_points_hstack): 
        
        nb_points = centered_points_hstack.shape[1]
        output = npmat.zeros((3, 3))
        
        for i in range(nb_points):
            centered_point = centered_points_hstack[:, i]
            mi = masses[i]
            point_cross_mat = cross_product_matrix(centered_point)
            output -= mi * point_cross_mat * point_cross_mat
            
        return output
            
    
    def _update_position(self, total_force, dt):
        """
            Integrates velocity and position for a time step dt if the sum
            of every forces is provided
        """

        total_force = Vect(total_force)
        self.center_velocity += dt * self.total_mass_inverse * total_force
        self.center_position += dt * self.center_velocity
    
    
    def _update_orientation(self, total_force_moment, dt):
        """
            Integrates the provided force momentum to change angular momentum,
            uses solid inertia to compute the angular speed and uses it to
            update the quaternion representing the object orientation
        """
        
        total_force_moment = Vect(total_force_moment)
        
        # Integration of the force momentum to get the angular momentum
        self.angular_momentum += dt * total_force_moment
        
        # Rotation matrix corresponding to the current object angle
        R = self.rotation_matrix 

        # Inertia matrix inverse is obtained from the initial one by a change of bases
        current_inertia_matrix_inverse = R * self.initial_inertia_matrix_inverse * R.T
        
        # Angular speed vector is obtained from the momentum and the inertia
        w = current_inertia_matrix_inverse * self.angular_momentum
        w_as_quaternion = npmat.vstack([0., w])
        
        # Rotation quaternion integration from a derivative computed with a quaternion multiplication 
        self.rotation_quaternion += dt * 0.5 * quaternion_mult(w_as_quaternion, self.rotation_quaternion)
        
        # Normalization of the quaternion to ensure numerical stability through time steps
        self.rotation_quaternion /= npmat.linalg.norm(self.rotation_quaternion)
        
        # Update of the rotation matrix from the new quaternion
        self.rotation_matrix = rotation_matrix_from_quaternion(self.rotation_quaternion)
        
        
    def update(self, total_force, total_force_moment, dt):
        
        self._update_position(total_force, dt)
        self._update_orientation(total_force_moment, dt)
        self._compute_AABB()
        
        
    def points_hstack(self):
        """
            Returns a matrix whose columns are the current positions 
            of the material points in the solid
        """
        
        # Centered points are obtained by a rotation of the original ones
        current_centered_points_hstack = self.rotation_matrix * self.centered_initial_points_hstack 
        
        # The center of mass position is added to have the actual position
        return current_centered_points_hstack + self.center_position
        
    
    def AABB_intersect_with(self, other_solid):
        
        AABB = self.AABB
        other_AABB = other_solid.AABB
        
        for i in range(3):
            if not ((other_AABB[i, 0] <= AABB[i, 0] <= other_AABB[i, 1]) 
                    or (AABB[i, 0] <= other_AABB[i, 0] <= AABB[i, 1])):
                return False
            
        return True
    
    
    def AABB_corners(self):
        
        AABB = self.AABB
        output = npmat.empty((3, 8))
        
        i = 0
        
        for x_i in range(2):
            for y_i in range(2):
                for z_i in range(2):
                    output[0, i] = AABB[0, x_i]
                    output[1, i] = AABB[1, y_i]
                    output[2, i] = AABB[2, z_i]
                    i += 1
                    
        return output
        
    
    def print(self):
        
        print("--------------------------")
        print("--------------------------")
        
        print("Position:")
        pprint(self.center_position)
        
        print("Velocity:")
        pprint(self.center_velocity)
        
        print("Mass:")
        pprint(self.total_mass)
        
        print("Mass inverse:")
        pprint(self.total_mass_inverse)
        
        print("--------------------------")
        
        print("Rotation quaternion:")
        pprint(self.rotation_quaternion)
        
        print("Rotation matrix:")
        pprint(self.rotation_matrix)
        
        print("Angular momentum (sigma):")
        pprint(self.angular_momentum)
        
        print("Initial inertia matrix:")
        pprint(self.initial_inertia_matrix)
        
        print("Initial inertia matrix inverse:")
        pprint(self.initial_inertia_matrix_inverse)
        
        print("Inertia matrix times inertia matrix inverse:")
        pprint(self.initial_inertia_matrix * self.initial_inertia_matrix_inverse)
 
    
        
        print("--------------------------")
    
        print("Axis Aligned Bounding Box (AABB):")
        pprint(self.AABB)
    
        print("--------------------------")
        print("--------------------------")
