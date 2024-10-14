#include "nn.h"
#include "matrix.h"
#include <iostream>
#include <cmath>

float sigmoidf(float x) {
	return 1.0f / (1.0f + expf(-x));
}

float sigmoid_derf(float x) {
	float q = (1.0f + expf(-x));
	return expf(-x) / (q*q);
}

NeuralNetwork::NeuralNetwork (const std::initializer_list<size_t>& init) {
	layers_size = init;
	weights = std::vector<Matrix>(size() - 1);
	biases = std::vector<Matrix>(size() - 1);
	activations = std::vector<Matrix>(size());
	predactivations = std::vector<Matrix>(size());
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
	activations[0] = input;
	//Todo: REMOVE THAT!!!!
	predactivations[0] = Matrix(1, activations[0].cols(), 0);

	for (size_t i = 1; i < size(); i++) {
		predactivations[i] = activations[i-1] * weights[i-1] + biases[i-1];
		//Todo: REMOVE THAT!!!!
		activations[i] = predactivations[i];
		FORALL(activations[i], activations[i].at(ROW, COL) = sigmoidf(predactivations[i].at(ROW, COL)));
	}

	return activations[size()-1];
}

float NeuralNetwork::batch_cost(const Matrix& input_batch, const Matrix& output_batch) {
	assert(input_batch.rows() == output_batch.rows());
	assert(input_batch.cols() == layer_size(0));
	assert(output_batch.cols() == layer_size(size()-1));

	float C = 0;

	for (size_t i = 0; i < input_batch.rows(); i++)
		// TODO: add a function that loads a row from a given matrix
		// TODO: add a function that returns the output vector
		C += mag_sq(output_batch.get_row(i) + (-1) * forward(input_batch.get_row(i)));


	return C / output_batch.rows();
}

float NeuralNetwork::cost(const Matrix& input, const Matrix& output) {
	ASSERT_VECTOR(input); ASSERT_VECTOR(output);
	assert(input.cols() == layer_size(0));
	assert(output.cols() == layer_size(size()-1));

	float C = 0;
	// TODO: overwrite operator - for Matrix
	C += mag_sq(output + (-1) * forward(input));
	return C;
}

void NeuralNetwork::finite_dist(const Matrix& input_batch,
																const Matrix& output_batch,
																float rate, float eps) {

	std::vector<Matrix> new_weights = weights;
	std::vector<Matrix> new_biases = biases;

	float C = batch_cost(input_batch, output_batch);

	for (size_t i = 0; i < size() - 1; i++) {
		Matrix &w = weights[i];
		for (size_t row = 0; row < w.rows(); row++) {
			for (size_t col = 0; col < w.cols(); col++) {
				float cache = w.at(row, col);
				w.at(row, col) += eps;

				float C2 = batch_cost(input_batch, output_batch);
				float dC_dW = (C2 - C) / eps;
				w.at(row, col) = cache;
				new_weights[i].at(row, col) = cache - dC_dW * rate;
			}
		}

		Matrix &b = biases[i];
		for (size_t j = 0; j < layer_size(i+1); j++) {
			float cache = b.at(0, j);
			b.at(0, j) += eps;

			float C2 = batch_cost(input_batch, output_batch);
			float dC_db = (C2 - C) / eps;
			b.at(0, j) = cache;
			new_biases[i].at(0, j) = cache - dC_db * rate;

		}
	}
	weights = new_weights;
	biases = new_biases;
}

void NeuralNetwork::backward(const Matrix& input,
														 const Matrix& output, float rate) {
	ASSERT_VECTOR(input); ASSERT_VECTOR(output);
	assert(input.cols() == layer_size(0));
	assert(output.cols() == layer_size(size()-1));

	forward(input);
	// TODO: \/ make a better initialization for derivitives field
	derivatives = activations;
	derivatives[size()-1] = 2 * (activations[size()-1] - output);

	for (size_t k_ = size()-2 + 1; k_ > 0; k_--) {
		size_t k = k_ - 1; // Due to the fact that size_t is always non-negative
		for (size_t i = 0; i < layer_size(k); i++) {
			derivatives[k].at(0, i) = 0;

			for (size_t j = 0; j < layer_size(k+1); j++) {
				derivatives[k].at(0, i) +=
					derivatives[k+1].at(0, j) *
					weights[k].at(i, j) *
					sigmoid_derf(predactivations[k].at(0, i));
			}
		}
	}


	// Updating weights
	for (size_t k = 0; k < size()-1; k++)
		FORALL(weights[k], weights[k].at(ROW, COL) -= rate * derivatives[k+1].at(0, COL)
					 * activations[k].at(0, ROW));

	// Updating biases
	for (size_t k = 0; k < size()-1; k++)
		FORALL(biases[k],
					 biases[k].at(0, COL) -= rate * derivatives[k+1].at(0, COL)
					 * sigmoid_derf(biases[k].at(0, COL)));
}

std::ostream& operator << (std::ostream& out, const NeuralNetwork& nn) {
	for (size_t i = 0; i < nn.size(); i++) {
		out << "--- Layer " << i << " ---" << std::endl;
		out << nn.activations[i] << std::endl;
	}
	return out;
}
