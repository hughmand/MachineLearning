
#include<iostream>
#include<vector>
#include"network.h"
#include"data.h"
#include"UI.h"


/*Program entry point
	data namespace is located in data.h and data.cpp
		defines classes to hold data required for learning, and methods to read data for learning
		data::Character::display() method used character_display.py to produce .png file of the character in /output
	network namespace is located in network.h, network.cpp and learning.cpp
		network.cpp contains methods used for creating, managing and saving the network: biases, weights and node structure etc.
		learning.cpp contains the methods used for training the network: gradient descent etc.
	UI namespace is located in UI.h and UI.cpp and provides a text based interface for the program



	The saved network is in /data/data.dat and /structure.dat
	/resources should contain the 4 uncompressed data files from MNIST database



*/

int main() {
	

	std::cout << "Handwriting ML V1" << std::endl;
	UI::core();
	
	
	return 0;
	
}