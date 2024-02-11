#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#ifndef MATRIX_H
#define MATRIX_H
#define get_matrix(m) _Generic((m),struct matrix*:(float(*)[(m)->cols])(m)->mx)

typedef struct {
	float theta;
	float rotater[2][2];
} rotation_matrix;

typedef struct {
	int dimension;
} identity_matrix;

struct matrix {
	size_t rows;
	size_t cols;
	float mx[];
};

rotation_matrix rotational_matrix(float angle) {
	rotation_matrix r_t;
	r_t.theta = angle;
	float predef[2][2] = {{cos(angle), sin(angle)}, {-sin(angle), cos(angle)}};
	memcpy(r_t.rotater, predef, sizeof(predef));
	return r_t;
}

struct matrix* new_matrix(int r, int c, char* s, float* a) { // :|
	struct matrix *m_t = malloc(sizeof(*m_t) + sizeof(float[r][c]));
	m_t->rows = (size_t)r;
	m_t->cols = (size_t)c;

	if(m_t != NULL) {
		if(s != NULL) {
			switch (*s)
			{
			case 'r':
				if(c == 2 && c == r) {
					for(int i = 0; i < r; i++) {
						for(int j = 0; j < c; j++) {
							get_matrix(m_t)[i][j] = rotational_matrix(*a).rotater[i][j]; //this is necessary for print_matrix basically
						}
					}
				}
				break;
			case 'i':
				for(int i = 0; i < r; i++) {
					for(int j = 0; j < c; j++) {
						if(i==j)
							{get_matrix(m_t)[i][j] = 1.0;}
						else {get_matrix(m_t)[i][j] = 0.0;}
					}
				}
				break;
			default:
				break;
			}
		} else {
			// this is for the segfault
			// also, the initial state of the coordinate system
			printf("error with character input!");
		}
	}

	return m_t;
}
void print_matrix(struct matrix* m) {
	for(int i = 0; i < m->rows; i++) {
		for(int j = 0; j < m->cols; j++) {
			printf("%f, ", get_matrix(m)[i][j]);
		}
		printf("\n");
	}
}



bool confirm_transform2d(struct matrix* m1, struct matrix* m2) {
	//find determinant of 2d matrix
	float det1 = abs( (get_matrix(m1)[0][0]*get_matrix(m1)[1][1]) - (get_matrix(m1)[0][1]*get_matrix(m1)[1][0]));
	float det2 = abs( (get_matrix(m2)[0][0]*get_matrix(m2)[1][1]) - (get_matrix(m2)[0][1]*get_matrix(m2)[1][0]));
	return (det1 == det2);
}

#endif