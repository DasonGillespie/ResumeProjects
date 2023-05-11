#include <iostream>
#include <string>
#include "image_menu.h"
#include "Image.h"
#include <fstream>
#include "PPM.h"
#include "MenuData.h"
#include "ActionData.h"

void drawAsciiImage(ActionData& action_data) {
    double sum;
    for(int i = 0; i < action_data.getOutputImage().getHeight(); i++) {
        for(int j = 0; j < action_data.getOutputImage().getWidth(); j++) {
            sum = 0;

	    //add together values of each channel
            for(int k = 0; k < 3; k++) {
                sum += action_data.getOutputImage().getChannel(i, j, k);
            }
	    //cacluate strength of pixel
            sum = sum / 765.0;

	    //display ascii based on strength
            if (sum >= 1.0) {
                action_data.getOS() << "@";    
            } else if (sum >= 0.9) {
                action_data.getOS() << '#';
            } else if (sum >= 0.8) {
                action_data.getOS() << '%';    
            } else if (sum >= 0.7) {
                action_data.getOS() << '*';    
            } else if (sum >= 0.6) {
                action_data.getOS() << '|';    
            } else if (sum >= 0.5) {
                action_data.getOS() << '+';    
            } else if (sum >= 0.4) {
                action_data.getOS() << ';';    
            } else if (sum >= 0.3) {
                action_data.getOS() << '~';    
            } else if (sum >= 0.2) {
                action_data.getOS() << '-';    
            } else if (sum >= 0.1) {
                action_data.getOS() << '.';    
            } else if (sum >= 0.0) {
                action_data.getOS() << ' ';
            }
        }
        action_data.getOS() << '\n';
    }
}

void writeUserImage(ActionData& action_data) {//check this
	std::ofstream filename(getString(action_data, "Output filename? "));
	action_data.getOutputImage().writeStream(filename);
	filename.close();
}

void copyImage(ActionData& action_data) {
	action_data.getOutputImage() = action_data.getInputImage1();
}

void readUserImage1(ActionData& action_data) {
	std::ifstream filename(getString(action_data, "Input filename? "));
	action_data.getInputImage1().readStream(filename);
	filename.close();
}

void readUserImage2(ActionData& action_data) {
	std::ifstream filename(getString(action_data, "Input filename? "));
	action_data.getInputImage2().readStream(filename);
	filename.close();
}

void copyOutputImageToImage1(ActionData& action_data) {
	action_data.getInputImage1() = action_data.getOutputImage();
}
