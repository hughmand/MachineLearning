#include"network.h"
#include"data.h"
#include<fstream>
#include<string>
#include<random>
#include<filesystem>
//Functions appear in the same order they appear in network.h
//Also includes generate_random() for floats

namespace network {
	float generate_random() 
	{
		std::random_device rd; //random device instance
		std::mt19937 gen(rd()); //specific random number algorithm, initialised using random device instance, so each is different
		std::normal_distribution<float> d(0, 1); //defaults to doubl
		return d(gen); //produces random number based on normal distribution
	}

	void Node::evaluate(float* pinput) 
	{
		float fvalue = 0;

		
		for (int i = 0; i < num_weight; i++) {
			fvalue += (pinput[i] * weight[i]);
		}
		fvalue += bias;
		
		set_value(fvalue);
		set_sigmoid_value(sigmoid(get_value()));
		
	}
	Node::Node(unsigned int pnum_weight) 
	{
		set_value(0);
		sigmoid_prime_value = 0;
		num_weight = pnum_weight;
		bias = generate_random();
		weight = new float[num_weight];
		for (int i = 0; i < num_weight; i++) 
		{
			weight[i] = generate_random();
		}
	}
	Node::Node(unsigned int pnum_weight, float pbias, float* pweight)
	{
		num_weight = pnum_weight;
		sigmoid_prime_value = 0;
		set_value(0);
		bias = pbias;
		weight = new float[num_weight];
		for (int i = 0; i < num_weight; i++)
		{
			weight[i] = pweight[i];
		}
	}
	

	void Layer::evaluate(float* pinput) 
	{
		for (int i = 0; i < num_node; i++)
		{
			node[i]->evaluate(pinput);
		}
	}
	void Layer::set_nodes(Layer_Type player_type, float* pinput) 
	{
		//TODO: Error handling for layertype!= input and mismatched numbers
		if (player_type == input) 
		{
			for (int i = 0; i < num_node; i++) 
			{
				node[i]->set_sigmoid_value(pinput[i]);
			}		
		}
	}
	float* Layer::get_output() 
	{
		float* output = new float[num_node];
		for (int i = 0; i < num_node; i++) 
		{
			output[i] = node[i]->get_sigmoid_value();		
			
		}
		return output;
	}
	Layer::Layer(unsigned int pnum_node) 
	{	
		set_layer_type(input);
		num_node = pnum_node;
		
		error = new float[get_num_node()];
		node = new Base_Node* [num_node];
		for (int i = 0; i < num_node; i++)
		{
			node[i] = new Base_Node();
		}
	}
	Layer::Layer(unsigned int pnum_node, unsigned int pnum_prev_node) 
	{
		set_layer_type(hidden);
		num_node = pnum_node;
		error = new float[get_num_node()];
		node = new Base_Node* [num_node];
		for (int i = 0; i < num_node; i++) 
		{
			node[i] = new Node(pnum_prev_node);
		}
	}
	Layer::Layer(unsigned int pnum_node, unsigned int pnum_weights, float* pbiases, float* pweights)
	{
		set_layer_type(hidden);
		num_node = pnum_node;
		error = new float[get_num_node()];
		node = new Base_Node * [num_node];
		unsigned int count = 0;
		for (int i = 0; i < num_node; i++) //initialise weights and biases from those read from file;
		{
			float* tempweights = new float[pnum_weights];
			for (int j = 0; j < pnum_weights; j++) 
			{
				tempweights[j] = pweights[count++];
			
			}
			
			node[i] = new Node(pnum_weights, pbiases[i], tempweights);
			delete[] tempweights;
		}


	}
	Layer::~Layer() 
	{
		delete[] error;
		for (int i = 0; i < num_node; i++) 
		{
			delete node[i];
		}
		delete[] node;
	}

	void Network::evaluate(data::Character* pinput) 
	{
		float* floatdata = new float[pinput->num_pixel];
		for (int i = 0; i < pinput->num_pixel; i++) 
		{
			floatdata[i] = (float) pinput->pixel[i];
		}
		layer[0]->set_nodes(layer[0]->get_layer_type(), floatdata);
		delete[] floatdata;
		
		for (int i = 1; i < num_layer; i++)
		{
			
			floatdata = layer[i - 1]->get_output();
			layer[i]->evaluate(floatdata);
			delete[] floatdata;
		}
	}
	void Network::save() 
	{
		std::ofstream file_layer_num("C:/Dev/MLC/data/layer_num.dat",std::ios::binary);
		std::ofstream file_layer_sizes("C:/Dev/MLC/data/layer_sizes.dat", std::ios::binary);
		std::ofstream file_biases_and_weights("C:/Dev/MLC/data/layer_biases_and_weights.dat", std::ios::binary);

		char* towrite = new char[4];	
		std::memcpy(towrite, &num_layer, 4);
		file_layer_num.write(towrite, 4);
		delete[] towrite;

		
		towrite = new char[12];
		std::memcpy(towrite, layer_sizes, 12);
		file_layer_sizes.write(towrite, 12);	
		delete[] towrite;

		float* biases_layer_1 = new float[15];
		float* weights_layer_1 = new float[11760];
		float* biases_layer_2 = new float[10];
		float* weights_layer_2 = new float[150];
		for (int i = 0; i < 15; i++) {
			biases_layer_1[i] = get_layer()[1]->get_node()[i]->get_bias();
		}
		
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 784; j++) {
				weights_layer_1[(i*784)+j] = get_layer()[1]->get_node()[i]->get_weight()[j];
			}
		}
		for (int i = 0; i < 10; i++) {
			biases_layer_2[i] = get_layer()[2]->get_node()[i]->get_bias();
		}
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 15; j++) {
				weights_layer_2[(i*15)+j] = get_layer()[2]->get_node()[i]->get_weight()[j];
			}
		}
		float* typeconversionfloat = new float[11935];
		int count = 0;
		
		for (int i = 0; i < 15; i++) {
			typeconversionfloat[count++] = biases_layer_1[i];
		}
		
		for (int i = 0; i < 11760; i++) {
			typeconversionfloat[count++] = weights_layer_1[i];
		}
		for (int i = 0; i < 10; i++) {
			typeconversionfloat[count++] = biases_layer_2[i];
		}
		for (int i = 0; i < 150; i++) {
		
			typeconversionfloat[count++] = weights_layer_2[i];
			
		}

		towrite = new char[47740];
		std::memcpy(towrite, typeconversionfloat, 47740);
	

		file_biases_and_weights.write(towrite, 47740);
		delete[] typeconversionfloat;
		delete[] towrite;
		delete[] biases_layer_1;
		delete[] biases_layer_2;
		delete[] weights_layer_1;
		delete[] weights_layer_2;
	}
	
	Network::Network()
	{
		char* read;
		unsigned int typeconversionuint;

		read = new char[4];
		std::ifstream file_layer_num("C:/Dev/MLC/data/layer_num.dat", std::ios::binary);
		file_layer_num.read(read, 4);
		std::memcpy(&typeconversionuint, read, 4);
		num_layer = typeconversionuint;
		file_layer_num.close();
		delete[] read;

		read = new char[12];
		layer_sizes = new unsigned int[3];
		std::ifstream file_layer_sizes("C:/Dev/MLC/data/layer_sizes.dat", std::ios::binary);
		file_layer_sizes.read(read, 12);
		std::memcpy(layer_sizes, read, 12);
		file_layer_sizes.close();
		delete[] read;

		read = new char[47740];
		std::ifstream file_biases_and_weights("C:/Dev/MLC/data/layer_biases_and_weights.dat", std::ios::binary);
		file_biases_and_weights.read(read, 47740);
		float* typeconversionfloat = new float[11935];
		std::memcpy(typeconversionfloat, read, 47740);
		file_biases_and_weights.close();
		delete[] read;

		int count = 0;
	
		float* biases_layer_1 = new float[15];
		float* weights_layer_1 = new float[11760];
		float* biases_layer_2 = new float[10];
		float* weights_layer_2 = new float[150];
		for (int i = 0; i < 15; i++) {
			biases_layer_1[i] = typeconversionfloat[count++];
		}
		for (int i = 0; i < 11760; i++) {
			weights_layer_1[i] = typeconversionfloat[count++];
		}
		for (int i = 0; i < 10; i++) {
			biases_layer_2[i] = typeconversionfloat[count++];
		}
		for (int i = 0; i < 150; i++) {
			weights_layer_2[i] = typeconversionfloat[count++];
		}

		layer = new Layer * [num_layer];
	
		layer[0] = new Layer(layer_sizes[0]);
		layer[1] = new Layer(layer_sizes[0], layer_sizes[1], biases_layer_1, weights_layer_1); //Needs biases and weights
		layer[2] = new Layer(layer_sizes[1], layer_sizes[2], biases_layer_2, weights_layer_2);

		saved_state = true;
		delete[] typeconversionfloat;
		delete[] biases_layer_1;
		delete[] biases_layer_2;
		delete[] weights_layer_1;
		delete[] weights_layer_2;
	}
	Network::Network(unsigned int pnum_layers, unsigned int* player_sizes)
	{

		num_layer = pnum_layers;
		//TODO: common sense for num_layers large enough
		layer_sizes = new unsigned int[3];
		layer = new Layer*[num_layer];
		for (int i = 0; i < 3; i++) {
			layer_sizes[i] = player_sizes[i];
		}
		layer[0] = new Layer(player_sizes[0]);
		for (int i = 1; i < num_layer; i++) {	
			layer[i] = new Layer(player_sizes[i], player_sizes[i-1]);
		}
		layer[num_layer - 1]->set_layer_type(output);
		saved_state = false;
	}

	Network::~Network()
	{
		for (int i = 0; i < num_layer; i++)
		{
			delete layer[i];
		}
		delete[] layer;
		//delete[] layer_sizes;
		
	}
























	
}