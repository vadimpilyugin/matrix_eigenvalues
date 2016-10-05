#include "matrix.h"

#include <vector>
#include <math.h>
#include <stdio.h>
#include <fstream>

// Программа ищет собственные значения матрицы 
// методом Якоби.

const int debug=0;

template<typename T>
class Eigenvalues
{
	Vector<T> values;
	Matrix<T> a;
	Matrix<T> vectors;
	bool isFound = false;
public:
	Eigenvalues(const Matrix<T> & _m): values(_m.get_m()), a(_m), vectors(a.get_m(), a.get_n(), true)
	{
		if(a.get_m() != a.get_n())
		{
			fprintf(stderr, "Нельзя искать собственные значения не квадратной матрицы!\n");
			throw -1;
		}
		else if(a.get_m() == 1 && a.get_n() == 1)
		{
			isFound = true; //для матрицы 1x1 мы уже знаем ответ
			values[0] = a(0, 0);
			vectors(0, 0) = 1;
		}
		else if(a.get_m() == 0 && a.get_n() == 0)
		{
			fprintf(stderr, "Нельзя искать собственные значения пустой матрицы!\n");
			throw -2;
		}
	}
	const Vector<T> & jacobi();
	const Matrix<T> & get_vectors()
	{
		if(!isFound)
			fprintf(stderr, "Попытка получить собственные векторы, не выполнив метода Якоби\n");
		return vectors;	
	}
private:
	void stop()
	{
		if(values.size() < a.get_m())
			values = Vector<T>(a.get_m());
		for(int i = 0; i < a.get_m(); i++)
			values[i] = a(i, i);
	}
};

template <typename T>
const Vector<T> & Eigenvalues<T>::jacobi()
{
	if(isFound)
		return values;
	int i, j, cnt = 0;
	int m = a.get_m(), n = a.get_n();
	for(i = 0; i < m; i++)
		for(j = i+1; j < n; j++)
			if(fabs(a(i, j) - a(j, i)) > eps)
			{
				fprintf(stderr, "Матрица не симметричная: A(%d, %d) != A(%d, %d)!\n", i, j, j ,i);
				throw -1;
			}
	for(i = 0; i < m; i++)
		vectors(i, i) = 1;
	if(debug) printf("Eigenvectors:\n");
	if(debug) vectors.print();
	while(!isFound)
	{
		if(debug) printf("\n-----------------------------Step %d-----------------------------\n", cnt);
		if(debug) printf("Matrix is:\n");
		if(debug) a.print();
		vector<int> p(a.find_max());
		if(debug) printf("Largest value is %lf, located at (%d, %d)\n", a(p[0], p[1]), p[0], p[1]);
		if(fabs(a(p[0], p[1])) < eps)
		{
			stop();
			isFound = true;
if(debug) printf("\n-------------------------------END---------------------------------\n\n");
			return values;
		}
		double alpha;
		const double pi = 3.14159265358979323846;
		T a01 = a(p[0], p[1]), a00 = a(p[0], p[0]), a11 = a(p[1], p[1]);
		if(fabs(a00 - a11) < eps)
			alpha = pi / 4;
		else
			alpha = 0.5*atan2(2*a01, a00-a11);
		Matrix<double> rotation(2, 2);
		rotation(0, 0) = cos(alpha);
		rotation(0, 1) = -sin(alpha);
		rotation(1, 0) = -rotation(0, 1);
		rotation(1, 1) = rotation(0, 0);
		if(debug) printf("Rotation matrix is:\n");
		if(debug) rotation.print();
		vectors.rotate_right(rotation, p[0], p[1]);
		a.rotate_right(rotation, p[0], p[1]);
		//if(debug) printf("Matrix after rotation right:\n");
		//a.print();
		rotation(1, 0) = -rotation(1, 0);
		rotation(0, 1) = -rotation(0, 1);
		//if(debug) printf("Rotation matrix after transposition is:\n");
		//rotation.print();
		a.rotate_left(rotation, p[0], p[1]);
		if(debug) printf("Matrix after rotation: element (%d, %d) equals zero:\n", p[0], p[1]);
		if(debug) a.print();
		if(debug) printf("Eigenvectors:\n");
		if(debug) vectors.print();
		if(debug) printf("\n-------------------------End of Step %d--------------------------\n", cnt++);
	}
	return values;
}

int main()
{
	ifstream file("./matrices/matrix5x5.txt");	
	//Matrix<double> m(50, 50);
	Matrix<double> m(file);
	printf("Матрица А:\n");
	m.symmetrize().print();
	Eigenvalues<double> s = Eigenvalues<double> (m);
	Vector<double> val = s.jacobi();
	printf("Собственные значения матрицы А:\n");
	val.print();
	printf("Собственные векторы матрицы А:\n");
	s.get_vectors().print();
	return 0;
}
