#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <iomanip>
#include <unistd.h>
#include <fstream>

const int debug = 0;

template<typename T>
Matrix<T>::Matrix(int rows_n, int columns_n): a(nullptr), m(rows_n), n(columns_n)
{
	srand(time(NULL));
	if(debug) printf("Matrix::Matrix(int, int)\n");
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
		for(int i = 0; i < m; i++)
			for(int j = 0; j < n; j++)
				(*this)(i, j) = rand() % val_limit - val_limit/2; //заполнение случайными числами
	}
}
template<typename T>
Matrix<T>::Matrix(ifstream &file): a(nullptr), m(0), n(0)
{
	if(debug) printf("Matrix::Matrix(ifstream &)\n");
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
void Matrix<T>::write(ofstream &out) const
{
	if(debug) printf("Matrix::write(ofstream &)\n");
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
Matrix<T> & Matrix<T>::operator = (Matrix<T> &&x)
{
	if(debug) printf("Matrix::Matrix(Matrix<T>&&)\n");
	if(a != nullptr)
		delete [] a;
	a = x.a;
	x.a = nullptr;
	m = x.m;
	n = x.n;
	return *this;
}
template<typename T>
Matrix<T> & Matrix<T>::operator = (const Matrix<T>&other)
{
	if(debug) printf("Matrix::operator = (const Matrix<T>&)\n");
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
Matrix<T> & Matrix<T>::operator = (int val)
{
	if(debug) printf("Matrix::operator = (int)\n");
	if(a == nullptr)
		return *this;
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			(*this)(i, j) = val;
	return *this;
}
template<typename T>
Matrix<T>::Matrix(const Matrix<T>&other): a(nullptr), m(other.m), n(other.n)
{
	if(debug) printf("Matrix::Matrix(const Matrix<T>&)\n");
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
Matrix<T>::~Matrix()
{
	if(debug) printf("Matrix::~Matrix()\n");
	if(a != nullptr)
		delete [] a;
};
template<typename T>
void Matrix<T>::print() const
{
	if(debug) printf("Matrix::print()\n");
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
void Matrix<int>::print() const
{
	if(debug) printf("Matrix::print()\n");
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
T & Matrix<T>::operator() (int row, int col)
{
	if(debug) printf("T & Matrix::operator()(int, int)\n");
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
T Matrix<T>::operator() (int row, int col) const
{
	if(debug) printf("T Matrix::operator()(int, int)\n");
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
template class Matrix<float>;
template class Matrix<double>;
template class Matrix<int>;
