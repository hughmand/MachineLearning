"""
mnist_loader


A library to load the MNIST image data from mnist.pkl.gz - NOT train-images-idx3-ubyte
"""

import pickle
import gzip
import numpy as np

def load_data():
    f = gzip.open('mnist.pkl.gz', 'rb')
    u = pickle._Unpickler( f )
    u.encoding = 'latin1'
    training_data, validation_data, test_data = u.load();
    f.close()
    return (training_data, validation_data, test_data)

def load_data_wrapper():
    tr_d, va_d, te_d = load_data()

    training_inputs = [np.reshape(x, (784, 1)) for x in tr_d[0]] # 784 floats per image - 50000
    training_outputs = [vectorized_result(y) for y in tr_d[1]] #10 d array representing integers
 
    validation_inputs = [np.reshape(x, (784, 1)) for x in va_d[0]] # 784 floats per image - 10000
    validation_outputs = va_d[1]; # list of integers
    test_inputs = [np.reshape(x, (784, 1)) for x in te_d[0]] # 784 floats per image - 10000
    test_outputs = te_d[1] # list of integers
    return (training_inputs, training_outputs, validation_inputs, validation_outputs, test_inputs, test_outputs)

def vectorized_result(j):
    """Return a 10-dimensional unit vector with a 1.0 in the jth
    position and zeroes elsewhere.  This is used to convert a digit
    (0...9) into a corresponding desired output from the neural
    network."""
    e = np.zeros((10, 1))
    e[j] = 1.0
    return e
