#include "nn.h"

NeuralNetwork::NeuralNetwork (const std::initializer_list<size_t>& init) {
	layers_size = init;
	weights = std::vector<Matrix>(size() - 1);
	biases = std::vector<Matrix>(size() - 1);
	values = std::vector<Matrix>(size());

	// random initialization
	for (size_t i = 0; i < size() - 1; i++) {
		weights[i] = randm(layer_size(i), layer_size(i+1), -3, 3);
		biases[i] = randm(layer_size(i+1), 1, -3, 3);
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
	assert(input.get_cols() == layer_size(0));

	// loads the input
	// TODO: Needs an activation function so bad XDDD
	values[0] = input;
	for (size_t i = 0; i < size() - 1; i++)
		values[i+1] = values[i] * weights[i] + biases[i];
	return values[size()-1];
}

float NeuralNetwork::cost(const Matrix& input, const Matrix& output) {
	ASSERT_VECTOR(input);
	ASSERT_VECTOR(output);

	assert(input.get_cols() == layer_size(0));
	assert(input.get_cols() == layer_size(size()-1));

	Matrix result = forward(input);

	float dist = 0;
	for (size_t i = 0; i < output.get_cols(); i++) {
		float d = output.at(0, i) - result.at(0, i);
		dist += d*d;
	}

	return dist;
}
