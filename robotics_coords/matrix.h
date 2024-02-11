#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#ifndef MATRIX_H
#define MATRIX_H

const int init_size = 0;

typedef struct {
	float theta;
	float rotater[2][2];// = {{cos(*theta), -sin(*theta)}, {cos(*theta), sin(*theta)}};
} rotation_matrix;

struct matrix {
	rotation_matrix *rm;
};


void populate(struct matrix m) {
	if(m.rm != NULL) {
		printf("all valid thus far\n");
	}
}
//int size_r, bool is_square, int col=NULL, float theta=NULL

#endif