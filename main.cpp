#include <initializer_list>
#include <iostream>
#include <assert.h>
using namespace std;

template<typename T>
using init_list = initializer_list<T>;

#define FORALL(m, f) \
	for (size_t ROW = 0; ROW < (m).get_rows(); ROW++) \
		for (size_t COL = 0; COL < (m).get_cols(); COL++) \
			(f)

class Matrix {
private:
	size_t rows, cols;
	float *es;

	inline void alloc_mem() {
		es = new float[rows * cols];
		assert(es != nullptr);
	}

public:
	Matrix () {
		rows = 0;
		cols = 0;
		alloc_mem();
	}

	Matrix (float v) {
		rows = 1;
		cols = 1;
		alloc_mem();
		at(0, 0) = v;
	}

	Matrix (size_t r, size_t c) {
		rows = r;
		cols = c;
		alloc_mem();
	}

	Matrix (const Matrix &m) {
		rows = m.rows;
		cols = m.cols;
		alloc_mem();
		FORALL(*this, this->at(ROW, COL) = m.at(ROW, COL));
	}

	Matrix (size_t r, size_t c, float v) {
		rows = r;
		cols = c;
		alloc_mem();

		FORALL(*this, at(ROW, COL) = v);
	}

	Matrix (init_list<float> li) {
		rows = 1;
		cols = li.size();
		alloc_mem();

		size_t i = 0;
		for (auto &elem : li)
			at(0, i++) = elem;
		i++;
	}

	Matrix (init_list<init_list<float>> li) {
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

	inline float& at(size_t r, size_t c) const { return es[r * cols + c]; }
	inline size_t get_rows() const { return rows; }
	inline size_t get_cols() const { return cols; }

	Matrix& operator = (const Matrix& m) {
		delete [] es;
		rows = m.rows;
		cols = m.cols;
		alloc_mem();

		FORALL(*this, at(ROW, COL) = m.at(ROW, COL));
		return *this;
	}

	Matrix& operator += (const Matrix& m) {
		assert (rows == m.get_rows() && cols == m.get_cols());
		FORALL(*this, this->at(ROW, COL) += m.at(ROW, COL));
		return *this;
	}

	Matrix operator + (const Matrix& m) const {
		assert (rows == m.get_rows() && cols == m.get_cols());
		Matrix result(rows, cols);
		FORALL(result, result.at(ROW, COL) = at(ROW, COL) + m.at(ROW, COL));
		return result;
	}


	Matrix& operator *= (float v) {
		FORALL(*this, this->at(ROW, COL) *= v);
		return *this;
	}

	Matrix operator * (float v) const {
		Matrix result(rows, cols);
		FORALL(result, result.at(ROW, COL) = at(ROW, COL) * v);
		return result;
	}

	Matrix operator * (const Matrix& m) const {
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

	~Matrix() {
		delete [] es;
	}
};

ostream& operator << (ostream& out, const Matrix& m) {
	cout << "[ ";
	for (int j = 0; j < m.get_cols(); j++)
			cout << m.at(0, j) << " ";

	for (int i = 1; i < m.get_rows(); i++) {
		cout << endl << "  ";
		for (int j = 0; j < m.get_cols(); j++)
			cout << m.at(i, j) << " ";
	}
	cout << "]";
	return out;
}

int main () {
	Matrix m = {5, 4, 6, 3, 2};
	m = 0;
	cout << m << endl;
	return 0;
}
