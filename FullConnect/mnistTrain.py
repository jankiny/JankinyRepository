import numpy as np
from NeuralNetwork import neuralNetwork

input_nodes = 784
hidden_nodes = 100
output_nodes = 10

learning_rate = 0.3

n = neuralNetwork( input_nodes, hidden_nodes, output_nodes, learning_rate)

training_data_file = open("mnist_dataset/mnist_train_100.csv", 'r')
training_data_list = training_data_file.readlines()
training_data_file.close()

for training_data in training_data_list:
    values = training_data.split(',')
    inputs = ( np.asfarray(values[1:]) / 255.0 * 0.99) + 0.01
    targets = np.zeros(output_nodes) + 0.01
    targets[int(values[0])] = 0.99
    n.train( inputs, targets )
    pass
