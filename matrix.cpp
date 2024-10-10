#include "matrix.h"
#include <assert.h>

void Matrix::alloc_mem() {
	es = new float[_rows * _cols];
	assert(es != nullptr);
}

void Matrix::free_mem() {
	if (es) delete [] es;
}

Matrix::Matrix () {
	_rows = 1;
	_cols = 1;
	alloc_mem();
	at(0, 0) = 0;
}

Matrix::Matrix (float v) {
	_rows = 1;
	_cols = 1;
	alloc_mem();
	at(0, 0) = v;
}

Matrix::Matrix (size_t r, size_t c) {
	_rows = r;
	_cols = c;
	alloc_mem();
}

Matrix::Matrix (const Matrix &m) {
	_rows = m._rows;
	_cols = m._cols;
	alloc_mem();
	FORALL(*this, this->at(ROW, COL) = m.at(ROW, COL));
}

Matrix::Matrix (Matrix &&m) {
	_rows = m._rows;
	_cols = m._cols;
	es = m.es;
	m.es = nullptr;
}

Matrix::Matrix (size_t r, size_t c, float v) {
	_rows = r;
	_cols = c;
	alloc_mem();

	FORALL(*this, at(ROW, COL) = v);
}

Matrix::Matrix (std::initializer_list<float> li) {
	_rows = 1;
	_cols = li.size();
	alloc_mem();

	size_t i = 0;
	for (auto &elem : li)
		at(0, i++) = elem;
	i++;
}

Matrix::Matrix (std::initializer_list<std::initializer_list<float>> li) {
	_rows = li.size();
	_cols = li.begin()->size();
	alloc_mem();

	size_t i = 0;
	for (auto &row : li) {
		assert(row.size() == cols());
		size_t j = 0;
		for (auto &elem : row)
			at(i, j++) = elem;
		i++;
	}
}

size_t Matrix::rows() const { return _rows; }
size_t Matrix::cols() const { return _cols; }

Matrix Matrix::get_row(size_t i) const {
	Matrix result(1, _cols);
	for (int j = 0; j < cols(); j++)
		result.at(0, j) = at(i, j);
	return result;
}

float mag_sq(const Matrix& m) {
	ASSERT_VECTOR(m);
	float result = 0;
	FORALL(m, result += m.at(ROW, COL) * m.at(ROW, COL));
	return result;
}

float&
Matrix::at(size_t r, size_t c) { return es[r * cols() + c]; }

const float&
Matrix::at(size_t r, size_t c) const { return es[r * cols() + c]; }


Matrix& Matrix::operator = (const Matrix& m) {
	if (this == &m) return *this;
	delete [] es;
	_rows = m._rows;
	_cols = m._cols;
	alloc_mem();

	FORALL(*this, at(ROW, COL) = m.at(ROW, COL));
	return *this;
}

Matrix& Matrix::operator = (Matrix&& m) {
	if (this == &m) return *this;

	free_mem();
	_rows = m.rows();
	_cols = m.cols();
	es = m.es;
	m.es = nullptr;

	return *this;
}

Matrix& Matrix::operator += (const Matrix& m) {
	assert(rows() == m.rows() && cols() == m.cols());
	FORALL(*this, this->at(ROW, COL) += m.at(ROW, COL));
	return *this;
}

Matrix Matrix::operator + (const Matrix& m) const {
	assert (rows() == m.rows() && cols() == m.cols());
	Matrix result(rows(), cols());
	FORALL(result, result.at(ROW, COL) = at(ROW, COL) + m.at(ROW, COL));
	return result;
}


Matrix& Matrix::operator *= (float v) {
	FORALL(*this, this->at(ROW, COL) *= v);
	return *this;
}

Matrix Matrix::operator * (float v) const {
	Matrix result(rows(), cols());
	FORALL(result, result.at(ROW, COL) = at(ROW, COL) * v);
	return result;
}

Matrix Matrix::operator * (const Matrix& m) const {
	assert(cols() == m.rows());
	Matrix result(rows(), m.cols());

	for (size_t i = 0; i < result.rows(); i++)
		for (size_t j = 0; j < result.cols(); j++) {
			result.at(i, j) = 0;
			for (size_t k = 0; k < cols(); k++)
				result.at(i, j) += at(i, k) * m.at(k, j);
		}
	return result;
}

Matrix::~Matrix() {
	free_mem();
}

std::ostream& operator << (std::ostream& out, const Matrix& m) {
	out << "[ ";
	for (int j = 0; j < m.cols(); j++)
		out << m.at(0, j) << " ";

	for (int i = 1; i < m.rows(); i++) {
		out << std::endl << "  ";
		for (int j = 0; j < m.cols(); j++)
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
