#include <stdio.h>
#include <math.h>

#ifndef MATRIX_H

struct rotation_matrix {
	 double m[2][2];
}
struct rotation_matrix populate(float theta) {
	struct rotation_matrix init;
	init.m[0][0] = cos(theta);
	init.m[1][0] = sin(theta);
	init.m[0][1] = -sin(theta);
	init.m[1][1] = cos(theta);
	return init;
}


#endif MATRIX_H
