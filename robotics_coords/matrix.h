#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#ifndef MATRIX_H
#define MATRIX_H
#define get_matrix(m) _Generic((m),struct matrix*:(float(*)[(m)->cols])(m)->mx)

const int init_size = 0;

typedef struct {
	float theta;
	float rotater[2][2];// = {{cos(*theta), -sin(*theta)}, {cos(*theta), sin(*theta)}};
} rotation_matrix;

typedef struct {
	int dimension;
} identity_matrix;

struct matrix {
	size_t rows;
	size_t cols;
	float mx[];
	// rotation_matrix *rm;
	// identity_matrix *im;
};
struct matrix* new_matrix(int r, int c) {
	struct matrix* m = malloc(sizeof(struct matrix));
	m->rows = (size_t)r;
	m->cols = (size_t)c;
	return m;
}

void populate(struct matrix* m) { // :|
	struct matrix *m_t = malloc(sizeof(*m_t) + sizeof(float[m->rows][m->cols]));
	if(m_t != NULL) {
		for(int i = 0; i < m->rows; i++) {
			for(int j = 0; j < m->cols; j++) {
				get_matrix(m_t)[i][j] = 1.0000;
				printf("%f, ", get_matrix(m_t)[i][j]);
			}
			printf("\n");
		}
	}
}


// void confirm_transform2d(struct matrix m) {
// 	//find determinant of matrix
	
// }

#endif