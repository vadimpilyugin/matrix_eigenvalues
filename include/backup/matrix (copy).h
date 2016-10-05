#pragma once

#include <fstream>

using namespace std;

const double eps = 1e-10;
const int val_limit = 20; //для ГСЧ: от -val/2 до val/2

template <typename T>
class Matrix
{
	T *a;
	int m, n;
public:
	// Construct matrix and fill with random numbers
	Matrix(int rows_n = 0, int columns_n = 0);
	// Move constructor from c++11 standard
	Matrix(Matrix<T> &&x): a(x.a), m(x.m), n(x.n) { x.a = nullptr; }
	// Load matrix from text file. File format:
	// <rows> <columns> <rows*columns numbers>
	Matrix(ifstream &file);
	// Move assignment operator for temporary objects
	Matrix<T> & operator= (Matrix<T> &&x);
	// Assignment operator. Makes a deep copy of an object
	Matrix<T> & operator = (const Matrix<T> &x);
	// Fill the matrix with value
	Matrix<T> & operator = (int value);
	// Write to text file. Format:
	// <rows> <columns> <rows*columns numbers>
	void write(ofstream &out) const;
	// Copy constructor. Allocates memory and copies all elements
	Matrix(const Matrix<T> &other);
	~Matrix();
	// Prints the matrix row by row
	void print() const;
	// Returns a reference to the element (row, col)
	T &operator() (int row, int col);
	// For constant matrices
	T operator() (int row, int col) const;
	//Getters
	int get_m() const { return m; }
	int get_n() const { return n; }
};

template <typename T>
class Vector: public Matrix<T>
{
public:
	// Vector is a matrix with cols = 1
	Vector(int _m = 0): Matrix<T>(_m, 1) {}
};
