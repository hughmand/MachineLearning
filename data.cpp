#include"data.h"
#include<fstream>
#include<stdint.h>
#include<vector>

/*
Methods are in order they appear in data.h
*/

namespace data {
	void Character::display() 
	{
		//writes image data to files to by read by pythons script
		std::ofstream pixel_file("C:/Dev/MLC/data/character.dat", std::ios::binary);
		std::ofstream structure_file("C:/Dev/MLC/data/character_dim.dat", std::ios::binary);

		pixel_file.write((char*)pixel, num_pixel); 

		unsigned int* buffer = &dimension;
		structure_file.write((char*)buffer, sizeof(unsigned int));
		unsigned int tvalue = (unsigned int) value;
		unsigned int* buffer2 = &tvalue;
		structure_file.write((char*) buffer2, sizeof(unsigned int));
		structure_file.close();
		pixel_file.close();

		system("python C:/Dev/MLC/character_display.py");
	}

	Character::Character(int pdimension)
	{
		dimension = pdimension;
		num_pixel = dimension * dimension;

		pixel = new uint8_t[num_pixel];
		value = 0;
		for (int i = 0; i < num_pixel; i++) //initlaises all pixels to 0 with given dimension
		{
			this->pixel[i] = 0;
		}
	}

	Character::Character(uint8_t pvalue, char* ppixel, int pdimension)
	{
		dimension = pdimension;
		num_pixel = dimension * dimension;

		pixel = new uint8_t[num_pixel];
		value = pvalue;
		//TODO: Error handling
		for (int i = 0; i < num_pixel; i++) //initliases pixels to given value
		{
			this->pixel[i] = ppixel[i]/255;
		}
	}

	Character::~Character()
	{
		delete[] pixel;
	}

	Training_Data::Training_Data(unsigned int pnum_char, unsigned int pdimension) {
		//TODO: Error handling
		num_char = pnum_char;

		character = new Character * [num_char];
		unsigned int num_pixel = pdimension * pdimension;
		char label_buffer = 0;
		char* lbuffer = &label_buffer;
		char* pbuffer = new char[num_pixel];
		
		std::ifstream labels("C:/Dev/MLC/resources/train-labels-idx1-ubyte", std::ios::binary); 
		std::ifstream pixels("C:/Dev/MLC/resources/train-images-idx3-ubyte", std::ios::binary); 
		if (labels.is_open() && pixels.is_open())
		{
			labels.seekg(8);//skip initial bytes, see MNIST documention
			pixels.seekg(16);//skip initial bytes, see MNIST documention
			for (int i = 0; i < num_char; i++) 
			{
				labels.read(lbuffer, 1);
				pixels.read(pbuffer, num_pixel);
				character[i] = new Character((uint8_t)*lbuffer, pbuffer, pdimension);	
			}
		}
		labels.close();
		pixels.close();
		delete[] (pbuffer);
	}

	Training_Data::Training_Data(int pnum_char, unsigned int pdimension, const char* resources_path) {
		//TODO: Error handling
		num_char = pnum_char;

		std::string ipath = (std::string) resources_path + "/train-images-idx3-ubyte";
		std::string lpath = (std::string) resources_path + "/train-labels-idx1-ubyte";
		character = new Character * [num_char];
		unsigned int num_pixel = pdimension * pdimension;
		char label_buffer =0;
		char* lbuffer = &label_buffer;
		char* pbuffer = new char[num_pixel];
		
		std::ifstream labels(lpath, std::ios::binary); 
		std::ifstream pixels(ipath, std::ios::binary); 
		if (labels.is_open() && pixels.is_open())
		{
			labels.seekg(8);
			pixels.seekg(16);
			for (int i = 0; i < num_char; i++)
			{
				labels.read(lbuffer, 1);
				pixels.read(pbuffer, num_pixel);
				character[i] = new Character((uint8_t)*lbuffer, pbuffer, pdimension);
			}
		}
		labels.close();
		pixels.close();
		delete[](pbuffer);
	}

	Training_Data::~Training_Data() {
		for (int i = 0; i < num_char; i++) {
			delete character[i];
			
		}
		delete[] character;
		
	}

	Test_Data::Test_Data(unsigned int pnum_char, unsigned int pdimension) {
		//TODO: Error handling
		num_char = pnum_char;
		character = new Character* [num_char];
		unsigned int num_pixel = pdimension * pdimension;
		char label_buffer =0;
		char* lbuffer = &label_buffer;
		char* pbuffer = new char[num_pixel];
		
		std::ifstream labels("C:/Dev/MLC/resources/t10k-labels-idx1-ubyte", std::ios::binary); 
		std::ifstream pixels("C:/Dev/MLC/resources/t10k-images-idx3-ubyte", std::ios::binary); 
		if (labels.is_open() && pixels.is_open())
		{
			labels.seekg(8);
			pixels.seekg(16);
			for (int i = 0; i < num_char; i++)
			{
				labels.read(lbuffer, 1);
				pixels.read(pbuffer, num_pixel);
				character[i] = new Character((uint8_t)*lbuffer, pbuffer, pdimension);
			}
		}
		labels.close();
		pixels.close();
		delete[] (pbuffer);
	}

	Test_Data::Test_Data(int pnum_char, unsigned int pdimension, const char* resources_path) {
		//TODO: Error handling
		num_char = pnum_char;

		std::string lpath = (std::string)resources_path + "/t10k-labels-idx1-ubyte";
		std::string ipath = (std::string)resources_path + "/t10k-images-idx3-ubyte";
		character = new Character* [num_char];
		unsigned int num_pixel = pdimension * pdimension;
		char label_buffer = 0;
		char* lbuffer = &label_buffer;
		char* pbuffer = new char[num_pixel];
		
		std::ifstream labels(lpath, std::ios::binary); 
		std::ifstream pixels(ipath, std::ios::binary);
		if (labels.is_open() && pixels.is_open())
		{
			labels.seekg(8);
			pixels.seekg(16);
			for (int i = 0; i < num_char; i++)
			{
				labels.read(lbuffer, 1);
				pixels.read(pbuffer, num_pixel);
				character[i] = new Character((uint8_t)*lbuffer, pbuffer, pdimension);
			}
		}
		labels.close();
		pixels.close();
		delete[] pbuffer;
	}

	Test_Data::~Test_Data() {
		for (int i = 0; i < num_char; i++) {
			delete character[i];

		}
		delete[] character;
	
	}
}






