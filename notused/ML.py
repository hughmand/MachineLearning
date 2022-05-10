

#Hand writing recognition using Machine Learning
#MNIST Database
#Each image is 28x28 pixels - 784 dimensional vector
#0 means white, 1 means black
#output as 10 dimensional vector
#one hidden layer of 15 neurons

#import in python
#export to new datafile

#import in C++
#write rest of program in C++

import numpy as np;
import mnist_loader;
import os;

def progress(current, max):
    percent = str(round(current/max * 100, 1))
    if (current != max-1):
        print("\r", percent, "% complete", end='');
    else:
        print("\r", "100.0 % complete - Done")

#imports using python tuples and ndarrays
training_inputs, training_outputs, validation_inputs, validation_outputs, test_inputs, test_outputs = mnist_loader.load_data_wrapper()

#removes files if already present
if os.path.exists("training_data_input"):
    os .remove("training_data_input")
if os.path.exists("training_data_output"):
    os.remove("training_data_output")
if os.path.exists("validation_data_input"):
    os .remove("validation_data_input")
if os.path.exists("validation_data_output"):
    os.remove("validation_data_output")
if os.path.exists("test_data_input"):
    os .remove("test_data_input")
if os.path.exists("test_data_output"):
    os.remove("test_data_output")

#saves the data to the files in a csv format to be read by C++
print("Copying training data inputs:")

f = open("training_data_input", "a");
for i in range(50000):
    progress(i, 50000);
    for j in range(784):    
       f.write(str(*training_inputs[i][j]));#reformats as plain numer
       f.write("/")#end item
    f.write("%")#end entry
f.close();


print("Copying training data outputs:")

f = open("training_data_output", "a");
for i in range(50000):
    progress(i, 50000);
    for j in range(10):    
       f.write(str(*training_outputs[i][j])); #reformats as plain number
       f.write("/")
    f.write("%")
f.close();
 
print("Copying validation inputs:")
      
f = open("validation_data_input", "a");
for i in range(10000):
    progress(i, 10000);
    for j in range(784):    
       f.write(str(*validation_inputs[i][j]));#reformats as plain numer
       f.write("/")#end item
    f.write("%")#end entry
f.close();

print("Copying validation ouputs:")

f = open("validation_data_output", "a");
for i in range(10000):
    progress(i, 10000);   
    f.write(str(validation_outputs[i])); #reformats as plain number
    f.write("%")
f.close();

print("Copying test inputs:")

f = open("test_data_input", "a");
for i in range(10000):
    progress(i, 10000);
    for j in range(784):    
       f.write(str(*test_inputs[i][j]));#reformats as plain numer
       f.write("/")#end item
    f.write("%")#end entry
f.close();

print("Copying test outputs:")

f = open("test_data_output", "a");
for i in range(10000):
    progress(i, 10000);   
    f.write(str(test_outputs[i])); #reformats as plain number
    f.write("%")
f.close();