/*
 * quike_utils.h
 *
 *  Created on: 8 déc. 2018
 *      Author: ajuvenn
 */

#ifndef QUIKE_UTILS_H_
#define QUIKE_UTILS_H_

#define QK_SIMPLE_ALLOC(ptr, nb_elements)\
	if ((nb_elements) == 0){\
		fprintf(stderr, "QK_SIMPLE_ALLOC (in %s) : received 0 as number of elements\n", __FUNCTION__);\
		exit(EXIT_FAILURE);\
	}\
	(ptr) = malloc((nb_elements) * sizeof(*(ptr)));\
	if ((ptr) == NULL){\
		fprintf(stderr, "QK_SIMPLE_ALLOC (in %s) : malloc returned NULL\n", __FUNCTION__);\
		exit(EXIT_FAILURE);\
	}\



#endif /* QUIKE_UTILS_H_ */
