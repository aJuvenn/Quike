/*
 * quike_vector_algebra.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */


#include "quike_header.h"


void qkVectNormalize(GLdouble v[3])
{
	GLdouble norm2 = 0.;

	for (int i = 0 ; i < 3 ; i++){
		norm2 += v[i] * v[i];
	}

	if (norm2 == 0.){
		return;
	}

	GLdouble inv_norm = 1./sqrt(norm2);

	for (int i = 0 ; i < 3 ; i++){
		v[i] *= inv_norm;
	}
}
