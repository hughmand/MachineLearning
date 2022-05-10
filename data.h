#pragma once
#include<string>
#include<iostream>
#define DEFAULT_DIMENSION 28


namespace data {
	struct Character //represents a character in the MNIST database
	{
		uint8_t* pixel; //a value of 0 represents white, 255 represents black
		uint8_t value; //a value from 0 - 9
		unsigned int dimension;
		unsigned int num_pixel;

		void display(); //uses character_display.py to produce an image of the character

		Character(int pdimensions = DEFAULT_DIMENSION); //initialises to 0
		Character(uint8_t pvalue, char* ppixel, int pdimension = DEFAULT_DIMENSION);
		~Character();
	};

	struct Training_Data 
	{
		Character** character;
		unsigned int num_char;

		Character** get_character() { return character; };

		Training_Data(unsigned int pnum_char, unsigned int pdimension = DEFAULT_DIMENSION); //looks in /resources
		Training_Data(int pnum_char, unsigned int pdimension, const char* resources_path ); //provde file path
		~Training_Data();
	};

	struct Test_Data 
	{
		Character** character;
		unsigned int num_char;

		Test_Data(unsigned int pnum_char, unsigned int pdimension = DEFAULT_DIMENSION);//looks in /resources
		Test_Data(int pnum_char, unsigned int pdimension, const char* resources_path);//provde file path
		~Test_Data();
	};

	

}







