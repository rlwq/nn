#include "nn.h"
#include "matrix.h"
#include <iostream>
#include <cmath>

float sigmoidf(float x) {
	return 1.0f / (1.0f + expf(-x));
}

NeuralNetwork::NeuralNetwork (const std::initializer_list<size_t>& init) {
	layers_size = init;
	weights = std::vector<Matrix>(size() - 1);
	biases = std::vector<Matrix>(size() - 1);
	values = std::vector<Matrix>(size());

	// random initialization
	for (size_t i = 0; i < size() - 1; i++) {
		weights[i] = randm(layer_size(i), layer_size(i+1), -3, 3);
		biases[i] = randm(1, layer_size(i+1), -3, 3);
	}
}

NeuralNetwork::~NeuralNetwork() {};

size_t NeuralNetwork::size() const {
	return layers_size.size();
}

size_t NeuralNetwork::layer_size(size_t i) const {
	return layers_size[i];
}

Matrix NeuralNetwork::forward(const Matrix& input) {
	ASSERT_VECTOR(input);
	assert(input.cols() == layer_size(0));

	// loads the input
	values[0] = input;
	for (size_t i = 0; i < size() - 1; i++) {
		values[i+1] = values[i] * weights[i] + biases[i];
		FORALL(values[i+1], values[i+1].at(ROW, COL) = sigmoidf(values[i+1].at(ROW, COL)));
	}
	return values[size()-1];
}

float NeuralNetwork::cost(const Matrix& input_batch, const Matrix& output_batch) {
	assert(input_batch.rows() == output_batch.rows());
	assert(input_batch.cols() == layer_size(0));
	assert(output_batch.cols() == layer_size(size()-1));

	float C = 0;

	for (size_t i = 0; i < input_batch.rows(); i++)
		// TODO: add a function that loads a row from a given matrix
		// TODO: add a function that returns the output vector
		C += mag_sq(output_batch.get_row(i) + (-1) * forward(input_batch.get_row(i)));


	return C;
}

void NeuralNetwork::finite_dist(const Matrix& input_batch,
																const Matrix& output_batch,
																float rate, float eps) {

	std::vector<Matrix> new_weights = weights;
	std::vector<Matrix> new_biases = biases;

	float C = cost(input_batch, output_batch);

	for (size_t i = 0; i < size() - 1; i++) {
		Matrix &w = weights[i];
		for (size_t row = 0; row < w.rows(); row++) {
			for (size_t col = 0; col < w.cols(); col++) {
				float cache = w.at(row, col);
				w.at(row, col) += eps;

				float C2 = cost(input_batch, output_batch);
				float dC_dW = (C2 - C) / eps;
				w.at(row, col) = cache;
				new_weights[i].at(row, col) = cache - dC_dW * rate;
			}
		}

		Matrix &b = biases[i];
		for (size_t j = 0; j < layer_size(i+1); j++) {
			float cache = b.at(0, j);
			b.at(0, j) += eps;

			float C2 = cost(input_batch, output_batch);
			float dC_db = (C2 - C) / eps;
			b.at(0, j) = cache;
			new_biases[i].at(0, j) = cache - dC_db * rate;
		}

	}
	weights = new_weights;
	biases = new_biases;
}

std::ostream& operator << (std::ostream& out, const NeuralNetwork& nn) {
	for (size_t i = 0; i < nn.size(); i++) {
		out << "--- Layer " << i << " ---" << std::endl;
		out << nn.values[i] << std::endl;
	}
	return out;
}
