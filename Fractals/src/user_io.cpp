#include <iostream>
#include <cstdlib>
#include <string>
#include "ActionData.h"

std::string getString(ActionData& action_data, const std::string& prompt) {
	std::string input;
	action_data.getOS() << prompt;
	action_data.getIS() >> input;
	return input;
}
int getInteger(ActionData& action_data, const std::string& prompt) {
	int input;
	action_data.getOS() << prompt;
	action_data.getIS() >> input;
	return input;
}

double getDouble(ActionData& action_data, const std::string& prompt) {
	double input;
	action_data.getOS() << prompt;
	action_data.getIS() >> input;
	return input;
}

int askQuestions3(ActionData& action_data) {
	std::string color = getString(action_data, "What is your favorite color? ");
	int integer = getInteger(action_data, "What is your favorite integer? ");
	double number = getDouble(action_data, "What is your favorite number? ");
	int i;
	for(i = 1; i < integer + 1; i++) {
		action_data.getOS() << i << " " << color << " " << number << std::endl;
	}
	return integer;
}

std::string getChoice(ActionData& action_data) {
	return getString(action_data, "Choice? ");
}

//check on this
void commentLine(ActionData& action_data) {
	bool reading = true;
	while(reading) {
		unsigned char c;
		action_data.getIS().read((char*) &c, sizeof(c));
		if(!action_data.getIS().good() || c == '\n') {
			reading = false;	
		}
	}
}

void quit(ActionData& action_data) {
	action_data.setDone();
}
