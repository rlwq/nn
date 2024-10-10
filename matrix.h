#pragma once
#include <initializer_list>
#include <iostream>
#include <assert.h>

#define ASSERT_VECTOR(m) assert((m).rows() == 1)

#define FORALL(m, f) \
	for (size_t ROW = 0; ROW < (m).rows(); ROW++) \
		for (size_t COL = 0; COL < (m).cols(); COL++) \
			(f)

class Matrix {
private:
	size_t _rows, _cols;
	float *es;
	void alloc_mem();
	void free_mem();
public:
	Matrix ();
	Matrix (float v);
	Matrix (size_t r, size_t c);
	Matrix (const Matrix &m);
	Matrix (Matrix &&m);
	Matrix (size_t r, size_t c, float v);
	Matrix (std::initializer_list<float> li);
	Matrix (std::initializer_list<std::initializer_list<float>> li);

	size_t rows() const;
	size_t cols() const;

	Matrix get_row(size_t i) const;

	float& at(size_t r, size_t c);
	const float& at(size_t r, size_t c) const;

	Matrix& operator = (const Matrix& m);
	Matrix& operator = (Matrix&& m);
	Matrix& operator += (const Matrix& m);
	Matrix operator + (const Matrix& m) const;
	Matrix& operator *= (float v);
	Matrix operator * (float v) const;
	Matrix operator * (const Matrix& m) const;
	~Matrix();
};

std::ostream& operator << (std::ostream& out, const Matrix& m);
 Matrix operator * (float v, const Matrix& m);

float mag_sq(const Matrix& m);

float randf(float low, float high);
Matrix randm(size_t cols, size_t rows,
						 float low, float high);
