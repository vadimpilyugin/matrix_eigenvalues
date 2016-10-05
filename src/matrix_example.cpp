#include "matrix.h"
#include <stdio.h>

int main()
{
	Matrix<int> a(10, 10);
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			a(i, j) = i+j;
	printf("Матрица А:\n");
	a.print();
	Matrix<float> b;
	b = Matrix<float>(3, 2);
	b.print();
	Matrix<double> *c = new Matrix<double> (3, 2);
	c->print();
	return 0;
}
