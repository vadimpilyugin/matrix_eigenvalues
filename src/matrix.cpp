#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <iomanip>
#include <unistd.h>
#include <fstream>
#include <math.h>

const int debug = 0;

template<typename T>
Grid<T>::Grid(int rows_n, int columns_n, bool set_null): a(nullptr), m(rows_n), n(columns_n)
{
	srand(time(NULL));
	if(debug) printf("Grid::Grid(int, int)\n");
	if(n < 0 || m < 0)
	{
		fprintf(stderr, "Некорректный размер матрицы: %dx%d\n", m, n);
		throw -1;
	}
	if(n == 0 || m == 0)
		a = nullptr;
	else
	{
		while((a = new T [m*n]) == nullptr)
		{
			printf("Memory allocation for matrix %dx%d", m, n);
			sleep(1);
		}
		if(set_null)
			for(int i = 0; i < m; i++)
				for(int j = 0; j < n; j++)
					(*this)(i, j) = 0;
		else
			for(int i = 0; i < m; i++)
				for(int j = 0; j < n; j++)
					(*this)(i, j) = rand() % val_limit - val_limit/2; //заполнение случайными числами
	}
}
template<typename T>
Grid<T>::Grid(ifstream &file): a(nullptr), m(0), n(0)
{
	if(debug) printf("Grid::Grid(ifstream &)\n");
	int i, j;
	file >> m >> n;
	if(m == 0 || n == 0)
		a = nullptr;
	else
	{
		while((a = new T [m*n]) == nullptr)
		{
			printf("Memory allocation for matrix %dx%d", m, n);
			sleep(1);
		}
		for(i = 0; i < m; i++)
			for(j = 0; j < n; j++)
			{
				file >> (*this)(i, j);
			}
	}

}
template<typename T>
void Grid<T>::write(ofstream &out) const
{
	if(debug) printf("Grid::write(ofstream &)\n");
	if(a == nullptr)
	{
		printf("Попытка записать пустую матрицу в файл\n");
		return;
	}
	int i, j;
	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n; j++)
			out << std::scientific << std::setprecision(8) << (*this)(i, j) << ' ';
		out << endl;
	}
}

template<typename T>
Grid<T> & Grid<T>::operator = (Grid<T> &&x)
{
	if(debug) printf("Grid::Grid(Grid<T>&&)\n");
	if(a != nullptr)
		delete [] a;
	a = x.a;
	x.a = nullptr;
	m = x.m;
	n = x.n;
	return *this;
}
template<typename T>
Grid<T> & Grid<T>::operator = (const Grid<T>&other)
{
	if(debug) printf("Grid::operator = (const Grid<T>&)\n");
	if(other.a == nullptr)
	{
		a = nullptr;
		return *this;
	}
	m = other.m;
	n = other.n;
	while((a = new T [m*n]) == nullptr)
	{
		printf("Memory allocation for matrix %dx%d", m, n);
		sleep(1);
	}
	memcpy(a, other.a, m*n*sizeof(T));
	return *this;
/*	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			(*this)(i, j) = other(i, j);
*/
}
template<typename T>
Grid<T> & Grid<T>::operator = (T val)
{
	if(debug) printf("Grid::operator = (int)\n");
	if(a == nullptr)
		return *this;
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			(*this)(i, j) = val;
	return *this;
}
template<typename T>
Grid<T>::Grid(const Grid<T>&other): a(nullptr), m(other.m), n(other.n)
{
	if(debug) printf("Grid::Grid(const Grid<T>&)\n");
	if(other.a == nullptr)
	{
		a = nullptr;
		return;
	}
	while((a = new T [m*n]) == nullptr)
	{
		printf("Memory allocation for matrix %dx%d", m, n);
		sleep(1);
	}
	memcpy(a, other.a, m*n*sizeof(T));
/*	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			(*this)(i, j) = other(i, j);
*/
}
template<typename T>
Grid<T>::~Grid()
{
	if(debug) printf("Grid::~Grid()\n");
	if(a != nullptr)
		delete [] a;
};
template<typename T>
void Grid<T>::print() const
{
	if(debug) printf("Grid::print()\n");
	printf("\n");
	if(a == nullptr)
	{
		printf("Попытка напечатать пустую матрицу\n");
		return;
	}
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
			printf("%5.2f\t", (*this)(i, j));
		printf("\n");
	}
	printf("\n");
}
template<>
void Grid<int>::print() const
{
	if(debug) printf("Grid::print()\n");
	printf("\n");
	if(a == nullptr)
	{
		printf("Попытка напечатать пустую матрицу\n");
		return;
	}
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
			printf("%5d\t", (*this)(i, j));
		printf("\n");
	}
	printf("\n");
}
template<typename T>
T & Grid<T>::operator() (int row, int col)
{
	if(debug) printf("T & Grid::operator()(int, int)\n");
	if(a == nullptr)
	{
		fprintf(stderr, "\nНекорректный доступ к элементу: матрица нулевого размера\n");
		throw -9;
	}
	else if(row < 0 || row >= m || col < 0 || col >= n)
	{
		fprintf(stderr, "\nНекорректный доступ к элементу: (%d, %d).\nМаксимально возможный: (%d, %d)\n", row, col, m-1, n-1);
		throw -6;
	}
	return a[n*row + col];
}
template<typename T>
T Grid<T>::operator() (int row, int col) const
{
	if(debug) printf("T Grid::operator()(int, int)\n");
	if(a == nullptr)
	{
		fprintf(stderr, "\nНекорректный доступ к элементу: матрица нулевого размера\n");
		throw -10;
	}
	else if(row < 0 || row >= m || col < 0 || col >= n)
	{
		fprintf(stderr, "\nНекорректный доступ к элементу: (%d, %d).\nМаксимально возможный: (%d, %d)\n", row, col, m-1, n-1);
		throw -6;
	}
	return a[n*row + col];
}

template <typename T>
vector<int> Matrix<T>::find_max() const
{
	T *a = Grid<T>::a;
	int m = Grid<T>::m, n = Grid<T>::n;
	if(a == nullptr || n == 0 || m == 0)
	{
		fprintf(stderr, "Попытка найти max в пустой матрице!\n");
		return {0, 0};
	}
	else if(m != n)
	{
		fprintf(stderr, "Нельзя применить метод к не квадратной матрице!\n");
		throw -1;
	}
	else if(m == 1 && n == 1)
		return{0, 0};
	int i, j;
	int row = 0, col = 1;
	T max = 0;
	for(i = 0; i < m; i++)
		for(j = i+1; j < n; j++)
			if(fabs(a[i*n+j]) > max)
			{
				max = fabs(a[i*n+j]);
				row = i;
				col = j;
			}
	return {row, col};
}
template <typename T>
void Matrix<T>::rotate_right(const Matrix & rot, int i, int j)
{
	int m = Grid<T>::m;
	Vector<T> vi(m), vj(m);
	for(int k = 0; k < m; k++)
	{
		vi[k] = (*this)(k, i) * rot(0, 0) + (*this)(k, j) * rot(1, 0);
		vj[k] = (*this)(k, i) * rot(0, 1) + (*this)(k, j) * rot(1, 1);
	}
	for(int k = 0; k < m; k++)
	{
		(*this)(k, i) = vi[k];
		(*this)(k, j) = vj[k];
	}
}
template <typename T>
void Matrix<T>::rotate_left(const Matrix & rot, int i, int j)
{
	int n = Grid<T>::n;
	Vector<T> vi(n), vj(n);
	for(int k = 0; k < n; k++)
	{
		vi[k] = (*this)(i, k) * rot(0, 0) + (*this)(j, k) * rot(0, 1);
		vj[k] = (*this)(i, k) * rot(1, 0) + (*this)(j, k) * rot(1, 1);
	}
	for(int k = 0; k < n; k++)
	{
		(*this)(i, k) = vi[k];
		(*this)(j, k) = vj[k];
	}
}
template <typename T>
Matrix<T> & Matrix<T>::symmetrize()
{
	if(Grid<T>::a == nullptr)
		return *this;
	int i, j;
	for(i = 0; i < Grid<T>::m; i++)
		for(j = i+1; j < Grid<T>::n; j++)
			(*this)(j, i) = (*this)(i, j);
			
	return *this;
}

template class Grid<float>;
template class Grid<double>;
template class Grid<int>;
template class Matrix<float>;
template class Matrix<double>;
//template class Matrix<int>;
//template class Vector<float>;
template class Vector<double>;
template class Vector<int>;
