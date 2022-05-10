#include"UI.h"
#include<string>

network::Network* main_network;
unsigned int* usizes;
unsigned int unum_layers;
bool network_open;

//namespace to manage the user interaction with the program


namespace UI {
	
	
	function get_text() {
		function f;

		std::string input;
		get_input: 
		std::cin >> input;
		if (input == "evaluate") {
			f = function::evaluate;
		}
		else if (input == "train") { f = function::train; }
		else if (input == "save") { f = function::save; }
		else if (input == "close") { f = function::close; }
		else if (input == "help") { f = function::help; }
		else if (input == "create") { f = function::create; }
		else if (input == "open") { f = function::open; }
		else {
			std::cout << "Not a valid command. 'help' to see options." << std::endl;
			goto get_input;
		}

		return f;
	}

	void core()
	{
		network_open = false;
		while (true) {
			try {
				function f = get_text();

				switch (f)
				{
				case (function::evaluate):
				{
					if (!network_open) {
						throw network_open;
					}
					
					data::Test_Data* test_data = new data::Test_Data(10000); //creates test data object
					std::cout << "State number in test data to be used: " << std::endl;
					int char_no;
					std::cin >> char_no;
					main_network->evaluate(test_data->character[char_no]);
				
					float* output = main_network->get_layer()[main_network->get_num_layer()-1]->get_output();
					std::cout << "Correct value: " << (int) test_data->character[char_no]->value << std::endl; //outputs correct numeric value
					for (int i = 0; i < 10; i++) //outputs node values
					{
						std::cout << "Output node "<<i<<": " << output[i] << std::endl;
					}
					test_data->character[char_no]->display(); //create image of character in /resources
					delete[] output;
					delete test_data;
					break;
				}
				case(function::train):
				{
				if (!network_open) {
					throw network_open;
				}
				data::Training_Data* training_data = new data::Training_Data(60000);//creates training data object;
				std::cout << "State number of cycles: " << std::endl;
				int number_of_cycles;
				std::cin >> number_of_cycles;
				for (int i = 0; i < number_of_cycles; i++) {
					main_network->train(training_data);//train
					
				}
				delete training_data;
				break;
			}
				case(function::save):
				{
					
					if (!network_open) {
						throw network_open;
					}
					main_network->save();
					break;
				case(function::close):
					if (!network_open) {
						throw network_open;
					}
					delete main_network;
					network_open = false;
					
					break;
				}
				case(function::help):
				{
					std::cout << "Commands: evaluate, train, save, close, create, open" << std::endl;
					break;
				}
				case(function::create):
				{
					if (network_open) {
						delete main_network;
					}
					std::cout << "State number of layers: " << std::endl;
					std::cin >> unum_layers;
					usizes = new unsigned int[unum_layers];
					for (int i = 0; i < unum_layers; i++) {
						std::cout << "State size of layer " << i << ":" << std::endl;
						std::cin >> usizes[i];
					}
					main_network = new network::Network(unum_layers, usizes);
					network_open = true;
					delete[] usizes;
					break;
				}
				case(function::open):
				{
					if (network_open) {
						delete main_network;
					}
					main_network = new network::Network();
					network_open = true;
					unum_layers = main_network->get_num_layer();
					
					break;
				}
				default:
				{
					std::cout << "Unrecgonised command. Type 'help' for a list of commands." << std::endl;
					break;
				}
				}
				std::cout << "Done" << std::endl;
			}
			catch (bool network_open) 
			{
				std::cout << "Please create or open a network before interacting." << std::endl;
			}
			catch (...) {
				std::cout << "An unexpected error occured." << std::endl;
				std::terminate();
			
			}


		}
		
	}
	
	
	
	

}