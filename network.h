#pragma once
#include<vector>
#include<iostream>
#include<string>
#include"data.h"

/*
Base_Node is a class for the nodes in the first layer of the network, that don't have weights or biases
Node is a derived class of Base_Node for the other nodes. Sigmoid function used to compute output
Layer is a class for each layer
Network is a class for the network as a whole
Gradient Descent is a storage class for gradient descent to hold data while back propagation is being computed for each character.

*/


namespace network {
	enum Layer_Type { input, hidden, output };

	class Base_Node
	{ 
		private:
			float sigmoid_value;
			float value;
		public:
			virtual void evaluate(float* pinput) { };
			float sigmoid(float pinput) { return (1 / (1 + exp(-1 * pinput))); };

			virtual float sigmoid_prime(float pinput) { return 0; };

			virtual float get_bias() const { return 0; };
			virtual float* get_weight() const { return 0; };
			inline virtual float get_sigmoid_prime_value() const { return 0; };
			inline float  get_sigmoid_value() const { return sigmoid_value; };
			inline virtual float get_value() const { return value;  };

			inline virtual void set_weight(float, int) {};
			virtual void set_sigmoid_prime_value(float pinput) {};
			virtual void set_value(float pvalue) { value = pvalue;  };
			void set_sigmoid_value(float pinput) { sigmoid_value = pinput; };
			inline virtual void set_bias(float) { };
			inline virtual unsigned int get_num_weight() const { return 0; };
			
			Base_Node() { sigmoid_value = 0; };
	};

	class Node : public Base_Node 
	{
	private:
		float bias;
		float* weight;
		unsigned int num_weight;
		
		float sigmoid_prime_value;

	public:
		virtual void evaluate(float* pinput);//updates value and sigmoid value given input from the previous layer (value is (weights dot input + bias))
		
		virtual float sigmoid_prime(float pinput) { return get_sigmoid_value() * (1 - get_sigmoid_value());}; //derivative of sigmoid function
		
		virtual float get_bias() const { return bias; };
	
		virtual float* get_weight() const { return weight; };
		virtual float get_sigmoid_prime_value() const { return sigmoid_prime_value; };
		virtual unsigned int get_num_weight() const { return num_weight; }
		
		virtual void set_sigmoid_prime_value(float pinput) {sigmoid_prime_value = pinput;};
		virtual void set_bias(float pinput) { bias = pinput; };
		virtual void set_weight(float pweight, int pindex) { weight[pindex] = pweight; };
	

		Node(unsigned int pnum_weight);
		Node(unsigned int pnum_weight, float pbias, float* pweight);
	};
	
	class Layer 
	{
		private:
			Base_Node** node;
			Layer_Type layer_type;
			unsigned int num_node;
			float* error;

		public:
			void evaluate(float* pinput); //calls Node::evaluate for all nodes in layer
			void compute_error(Layer* player);//populate error for all other layers using backprop
			void compute_error(float* poutput); // populate error for the final layer using backprop

			unsigned int get_num_node() { return num_node; };
			inline float* get_output();
			inline float* get_error() { return error; };
			inline Base_Node** get_node() { return node; };
			Layer_Type get_layer_type() { return layer_type; };

			void set_nodes(Layer_Type player_type, float* pinput);
			void set_layer_type(Layer_Type player_type) { layer_type = player_type; };
			
			Layer(unsigned int pnum_node); //used to intialise first layer
			Layer(unsigned int pnum_node, unsigned int pnum_prev_node); //used to create all other new layers
			Layer(unsigned int pnum_node, unsigned int pnum_weights, float* pbiases, float* pweights); //used to import existing layers from /data
			~Layer();
	};

	class Network 
	{
		private:
			Layer** layer;
			unsigned int num_layer;
			unsigned int* layer_sizes;
			bool saved_state;

		public:
			void train(data::Training_Data* training_data); //trains the network using training data
			void evaluate(data::Character* pinput); //evaluates the network for a given Character, setting the sigmoid value throughout the network. Call Layer[{last}] -> get_output() to get result
			void save(); //saves the weights and biases into /data
			


			unsigned int get_num_layer() { return num_layer; };
			Layer** get_layer() { return layer; }

			Network(); //reads network from /data
			Network(unsigned int pnum_layers, unsigned int* player_sizes); //generates new network with random nodes
			~Network();
	};

	struct Gradient_Descent {
			float*** delta_weights; //store the changes to weights in matrix
			float** delta_bias; //store the changes to the biases in the matrix
			int num_layer;
			int* layer_sizes;
	
			Gradient_Descent(Network* pnetwork); //creates new object with correct dimensions to train pnetwork
			~Gradient_Descent();
	};

	float generate_random(); //produces a random number with mean 0 and std 1

}