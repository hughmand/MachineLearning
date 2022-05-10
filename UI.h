#pragma once
#include"data.h"
#include"network.h"


namespace UI {
	enum class function { evaluate, train, save, close, help, create, open }; //enum of possible actions
	function get_text(); //gets user input
	void core(); //main looping function to get and carry out user input

}
