#include "matrix.h"
#include <assert.h>

#define FORALL(m, f) \
	for (size_t ROW = 0; ROW < (m).get_rows(); ROW++) \
		for (size_t COL = 0; COL < (m).get_cols(); COL++) \
			(f)

void Matrix::alloc_mem() {
	es = new float[rows * cols];
	assert(es != nullptr);
}

Matrix::Matrix () {
	rows = 0;
	cols = 0;
	alloc_mem();
}

Matrix::Matrix (float v) {
	rows = 1;
	cols = 1;
	alloc_mem();
	at(0, 0) = v;
}

Matrix::Matrix (size_t r, size_t c) {
	rows = r;
	cols = c;
	alloc_mem();
}

Matrix::Matrix (const Matrix &m) {
	rows = m.rows;
	cols = m.cols;
	alloc_mem();
	FORALL(*this, this->at(ROW, COL) = m.at(ROW, COL));
}

Matrix::Matrix (Matrix &&m) {
	rows = m.rows;
	cols = m.cols;
	es = m.es;
	m.es = nullptr;
}

Matrix::Matrix (size_t r, size_t c, float v) {
	rows = r;
	cols = c;
	alloc_mem();

	FORALL(*this, at(ROW, COL) = v);
}

Matrix::Matrix (std::initializer_list<float> li) {
	rows = 1;
	cols = li.size();
	alloc_mem();

	size_t i = 0;
	for (auto &elem : li)
		at(0, i++) = elem;
	i++;
}

Matrix::Matrix (std::initializer_list<std::initializer_list<float>> li) {
	rows = li.size();
	cols = li.begin()->size();
	alloc_mem();

	size_t i = 0;
	for (auto &row : li) {
		assert(row.size() == cols);
		size_t j = 0;
		for (auto &elem : row)
			at(i, j++) = elem;
		i++;
	}
}

size_t Matrix::get_rows() const { return rows; }
size_t Matrix::get_cols() const { return cols; }

float&
Matrix::at(size_t r, size_t c) { return es[r * cols + c]; }

const float&
Matrix::at(size_t r, size_t c) const { return es[r * cols + c]; }


Matrix& Matrix::operator = (const Matrix& m) {
	if (this == &m) return *this;
	delete [] es;
	rows = m.rows;
	cols = m.cols;
	alloc_mem();

	FORALL(*this, at(ROW, COL) = m.at(ROW, COL));
	return *this;
}

Matrix& Matrix::operator = (Matrix&& m) {
	if (this == &m) return *this;

	delete [] es;
	rows = m.rows;
	cols = m.cols;
	es = m.es;
	m.es = nullptr;

	return *this;
}

Matrix& Matrix::operator += (const Matrix& m) {
	assert (rows == m.get_rows() && cols == m.get_cols());
	FORALL(*this, this->at(ROW, COL) += m.at(ROW, COL));
	return *this;
}

Matrix Matrix::operator + (const Matrix& m) const {
	assert (rows == m.get_rows() && cols == m.get_cols());
	Matrix result(rows, cols);
	FORALL(result, result.at(ROW, COL) = at(ROW, COL) + m.at(ROW, COL));
	return result;
}


Matrix& Matrix::operator *= (float v) {
	FORALL(*this, this->at(ROW, COL) *= v);
	return *this;
}

Matrix Matrix::operator * (float v) const {
	Matrix result(rows, cols);
	FORALL(result, result.at(ROW, COL) = at(ROW, COL) * v);
	return result;
}

Matrix Matrix::operator * (const Matrix& m) const {
	assert(get_cols() == m.get_rows());
	Matrix result(get_rows(), m.get_cols());

	for (size_t i = 0; i < result.get_rows(); i++)
		for (size_t j = 0; j < result.get_cols(); j++) {
			result.at(i, j) = 0;
			for (size_t k = 0; k < get_cols(); k++)
				result.at(i, j) += at(i, k) * m.at(k, j);
		}
	return result;
}

Matrix::~Matrix() {
	delete [] es;
}

std::ostream& operator << (std::ostream& out, const Matrix& m) {
	out << "[ ";
	for (int j = 0; j < m.get_cols(); j++)
		out << m.at(0, j) << " ";

	for (int i = 1; i < m.get_rows(); i++) {
		out << std::endl << "  ";
		for (int j = 0; j < m.get_cols(); j++)
			out << m.at(i, j) << " ";
	}
	out << "]";
	return out;
}

Matrix operator * (float v, const Matrix& m)
{ return m * v; }

float randf(float low, float high) {
	float d = (float) rand() / (float) RAND_MAX;
	return d * (high - low) + low;
}

Matrix randm(size_t rows, size_t cols, float low, float high) {
	Matrix result(rows, cols);
	FORALL(result, result.at(ROW, COL) = randf(low, high));
	return result;
}
