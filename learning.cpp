#include"network.h"
#include"data.h"
#define LEARNINGRATE 0.00003 //In this implementation Learning Rate should be scaled inversely with the number of characters in the training set

/* 
Methods are in order they appear in network.h
Also contains chartooutput(uint8_t pvalue) used for converting the value to the ideal float output of the final layer. This has fixed range 0-9.
*/

namespace network {
	float* chartooutput(uint8_t pvalue) 
	{
		float* char_value = new float[10];
		for (int i = 0; i < 10; i++) {
			char_value[i] = 0;
		}
		char_value[pvalue] = 1;//sets the desired output to 1 rest 0;
		return char_value;
	}

	void Layer::compute_error(float* poutput) 
	{
			for (int i = 0; i < get_num_node(); i++) 
			{
				Base_Node* node = get_node()[i];
				node->set_sigmoid_prime_value(node->sigmoid_prime(node->get_value()));	//sets sigmoid prime value
			}
			for (int i = 0; i < get_num_node(); i++) 
			{
				error[i] = get_node()[i]->get_sigmoid_value() - poutput[i]; //cost function derivative


			}
			for (int i = 0; i < get_num_node(); i++) 
			{
				error[i] *=  get_node()[i] ->get_sigmoid_prime_value(); //sigmoid function derivative
			}
	}
	void Layer::compute_error(Layer* player) 
	{
		for (int i = 0; i < get_num_node(); i++) {
			Base_Node* node = get_node()[i];
			node->set_sigmoid_prime_value(node->sigmoid_prime(node->get_value())); //sets sigmoid prime value
		}
		for (int k = 0; k < get_num_node(); k++) {
			float product = 0;
			for (int j = 0; j < player->get_num_node(); j++) 
			{
				product += player->get_error()[j] * player->get_node()[j]->get_weight()[k]; //sum over transpose weights error back propagation
			}
			product *= get_node()[k]->get_sigmoid_prime_value(); //  multiply by sigmoid prime back propagation
			error[k] = product;
		}
	}

	void Network::train(data::Training_Data* ptraining_data)
	{

		Gradient_Descent store(this); //create gradient descent object for this array
		
		for (int i = 0; i < 60000; i++) { //loop through training data
			evaluate(ptraining_data->get_character()[i]); //feedforward, sets the values of value and sigmoid_value of each layer for a given input
			float* char_value = chartooutput(ptraining_data->get_character()[i]->value);
			for (int j = get_num_layer()-1; j > 0; j--) 
			{
				if (get_layer()[j]->get_layer_type() == output)
				{
					get_layer()[j]->compute_error(char_value); //error for final layer
				}
				else
				{
					get_layer()[j]->compute_error(get_layer()[j + 1]); //error for other layers
				}

			}
			for (int i = 0; i < store.num_layer - 1; i++) 
			{
				for (int j = 0; j < store.layer_sizes[i + 1]; j++) 
				{
					float* ptr = get_layer()[i]->get_output();
					for (int k = 0; k < store.layer_sizes[i]; k++)
					{
						store.delta_weights[i][j][k] += get_layer()[i + 1]->get_error()[j] * ptr[k]; //appends the new amount to change the weight - this is then multiplied by the learning rate and subtracted from the bias after the sum over characters is completed
					}
					delete[] ptr;
				}
			}
			for (int i = 0; i < store.num_layer - 1; i++) 
			{
				for (int j = 0; j < store.layer_sizes[i + 1]; j++) 
				{
					store.delta_bias[i][j] += get_layer()[i+1]->get_error()[j]; //appends the new amount to change the bias by
				}
			}
			delete[] char_value;
		}
		
		//apply the changes to the weights and biases
		for (int i = 1; i < store.num_layer; i++) 
		{
			for (int j = 0; j < store.layer_sizes[i]; j++)
			{
				float new_bias = get_layer()[i]->get_node()[j]->get_bias() - ((LEARNINGRATE) * store.delta_bias[i-1][j]);
					get_layer()[i]->get_node()[j]->set_bias(new_bias); 
			}
		}

		for (int i = 1; i < store.num_layer; i++) 
		{
			for (int j = 0; j < store.layer_sizes[i]; j++)
			{
				for (int k = 0; k < store.layer_sizes[i - 1]; k++) 
				{
					float new_weight = get_layer()[i]->get_node()[j]->get_weight()[k] - ((LEARNINGRATE) * store.delta_weights[i-1][j][k]);
					get_layer()[i]->get_node()[j]->set_weight(new_weight, k);
				}
			}
		}
	}

	Gradient_Descent::Gradient_Descent(Network* pnetwork) 
	{
		num_layer = pnetwork->get_num_layer();
		layer_sizes = new int[num_layer];

		for (int i = 0; i < num_layer; i++) 
		{
			layer_sizes[i] = pnetwork->get_layer()[i]->get_num_node();
		}
		//Create matrix to store changes to weight
		delta_weights = new float**[num_layer-1]; //first index is layer // second is node // third is weight
		for (int i = 0; i < num_layer-1; i++) {
			delta_weights[i] = new float* [layer_sizes[i+1]];
			for (int j = 0; j < layer_sizes[i+1]; j++) {
				delta_weights[i][j] = new float[layer_sizes[i]];
				for (int k = 0; k < layer_sizes[i]; k++) {
					delta_weights[i][j][k] = 0;
					
				}
				
			
			
			}
		}

		//Create matrix to store changes to bias
		delta_bias = new float* [num_layer-1];
		for (int i = 0; i < num_layer-1; i++) 
		{
			delta_bias[i] = new float[layer_sizes[i+1]];
			for (int j = 0; j < layer_sizes[i + 1]; j++) 
			{
				delta_bias[i][j] = 0;
			}	
		}
	}

	Gradient_Descent::~Gradient_Descent() 
	{
		for (int i = 0; i < num_layer-1; i++) 
		{
			for (int j = 0; j < layer_sizes[i+1]; j++) 
			{
				delete[] delta_weights[i][j];
			}
			delete[] delta_weights[i];
		}
		delete[] delta_weights;
		for (int i = 0; i < num_layer - 1; i++) 
		{
			delete[] delta_bias[i];
		}
		delete[] delta_bias;
		delete[] layer_sizes;
	}
}