#include "Image.h"
#include "image_menu.h"
#include "PPM.h"
#include "MenuData.h"
#include "ActionData.h"
#include <cstdlib>
#include <cmath>
#include "NumberGrid.h"
#include "ComplexFractal.h"
#include "JuliaSet.h"
#include "MandelbrotSet.h"

void diagonalQuadPattern(ActionData& action_data) {
	int height = getInteger(action_data, "Image height? ");
	int width = getInteger(action_data, "Image width? ");
	action_data.getInputImage1().setHeight(height);
	action_data.getInputImage1().setWidth(width);
	action_data.getInputImage1().setMaxColorValue(255);

	//loop through image vector using height and width
	for(int row = 0; row <= action_data.getInputImage1().getHeight(); row++) {
        	for(int column = 0; column <= action_data.getInputImage1().getWidth(); column++) {
            		if(action_data.getInputImage1().indexValid(row, column, 0)) {
				
				//red
                		if(row < action_data.getInputImage1().getHeight() / 2) {
                    			action_data.getInputImage1().setChannel(row, column, 0, 0);
                		} else {
                    			action_data.getInputImage1().setChannel(row, column, 0, 255);
                		}

				//green. uses provided equation
                		action_data.getInputImage1().setChannel(row, column, 1, (2 * row + 2 * column) % 256);

				//blue
                		if(column < action_data.getInputImage1().getWidth() / 2) {
                    			action_data.getInputImage1().setChannel(row, column, 2, 0);
                		} else {
                    			action_data.getInputImage1().setChannel(row, column, 2, 255);
           		     	}
            		}
        	}
    	}
}

void stripedDiagonalPattern(ActionData& action_data) {
	int height = getInteger(action_data, "Image height? ");
	int width = getInteger(action_data, "Image width? ");
	action_data.getInputImage1().setHeight(height);
	action_data.getInputImage1().setWidth(width);
	int value = ((height + width) / 3);
	if(value > 255) {
		action_data.getInputImage1().setMaxColorValue(255);
	}
	else {
		action_data.getInputImage1().setMaxColorValue(value);
	}

	for(int row = 0; row <= action_data.getInputImage1().getHeight(); row++) {
		for(int column = 0; column <= action_data.getInputImage1().getWidth(); column++) {
			if(action_data.getInputImage1().indexValid(row, column, 0)) {
				
				//red
				if(row < (action_data.getInputImage1().getHeight() / 2)) {
					//here
					action_data.getInputImage1().setChannel(row, column, 0, 0);
				}
				if(row >= (action_data.getInputImage1().getHeight() / 2) && 0 == row % 3) {
					action_data.getInputImage1().setChannel(row, column, 0, 0);
				}	
				if(row >= (action_data.getInputImage1().getHeight() / 2) && row % 3 != 0) {
					action_data.getInputImage1().setChannel(row, column, 0, action_data.getInputImage1().getMaxColorValue());
				}		

				//green
				int green_value = (row + action_data.getInputImage1().getWidth() - column - 1) % (action_data.getInputImage1().getMaxColorValue() + 1);
				action_data.getInputImage1().setChannel(row, column, 1, green_value);

				//blue
				if(column < row) {
					action_data.getInputImage1().setChannel(row, column, 2, 0);
				}
				if(column >= row) {
					action_data.getInputImage1().setChannel(row, column, 2, action_data.getInputImage1().getMaxColorValue());
				}
			}
		}
	}
}

void flagRomaniaPattern(ActionData& action_data) {
	
	int height = getInteger(action_data, "Image height? ");
	int width = (height * 3) / 2;
	action_data.getInputImage1().setHeight(height);
	action_data.getInputImage1().setWidth(width);
	action_data.getInputImage1().setMaxColorValue(255);

	for(int row = 0; row <= action_data.getInputImage1().getHeight(); row++) {
		for(int column = 0; column <= action_data.getInputImage1().getWidth(); column++) {
			if(action_data.getInputImage1().indexValid(row, column, 0)) {
				
				//first third
				if(column < (action_data.getInputImage1().getWidth() / 3)) {
					action_data.getInputImage1().setChannel(row, column, 0, 0);
					action_data.getInputImage1().setChannel(row, column, 1, 43);
					action_data.getInputImage1().setChannel(row, column, 2, 127);
				}

				//second third
				if(column >= (action_data.getInputImage1().getWidth() / 3) && column < (action_data.getInputImage1().getWidth() / 3) * 2) {
					action_data.getInputImage1().setChannel(row, column, 0, 252);
					action_data.getInputImage1().setChannel(row, column, 1, 209);
					action_data.getInputImage1().setChannel(row, column, 2, 22);
				}

				//third third
				if(column > (action_data.getInputImage1().getWidth() / 3) * 2) {
					action_data.getInputImage1().setChannel(row, column, 0, 206);
					action_data.getInputImage1().setChannel(row, column, 1, 17);
					action_data.getInputImage1().setChannel(row, column, 2, 38);
				}
			}
		}
	}
}

void setSize(ActionData& action_data) {
	int height = getInteger(action_data, "Height? ");
	int width = getInteger(action_data, "Width? ");

	action_data.getInputImage1().setHeight(height);
	action_data.getInputImage1().setWidth(width);
}

void setMaxColorValue(ActionData& action_data) {
	int value = getInteger(action_data, "Max color value? ");

	action_data.getInputImage1().setMaxColorValue(value);
}

void setChannel(ActionData& action_data) {
	int row = getInteger(action_data, "Row? ");
	int column = getInteger(action_data, "Column? ");
	int channel = getInteger(action_data, "Channel? ");
	int value = getInteger(action_data, "Value? ");

	action_data.getInputImage1().setChannel(row, column, channel, value);
}

void setPixel(ActionData& action_data) {
	int row = getInteger(action_data, "Row? ");
	int column = getInteger(action_data, "Column? ");
	int red = getInteger(action_data, "Red? ");
	int green = getInteger(action_data, "Green? ");
	int blue = getInteger(action_data, "Blue? ");
	
	action_data.getInputImage1().setChannel(row, column, 0, red);
	action_data.getInputImage1().setChannel(row, column, 1, green);
	action_data.getInputImage1().setChannel(row, column, 2, blue);
}

void clearAll(ActionData& action_data) {
	for(int row = 0; row < action_data.getInputImage1().getHeight(); row++) {
		for(int column = 0; column < action_data.getInputImage1().getWidth(); column++) {
			action_data.getInputImage1().setChannel(row, column, 0, 0);
			action_data.getInputImage1().setChannel(row, column, 1, 0);
			action_data.getInputImage1().setChannel(row, column, 2, 0);
		}
	} 
}

void drawCircle(ActionData& action_data) {
	int row = getInteger(action_data, "Center Row? ");
	int column = getInteger(action_data, "Center Column? ");
	int radius = getInteger(action_data, "Radius? ");
	int red = getInteger(action_data, "Red? ");
	int green = getInteger(action_data, "Green? ");
	int blue = getInteger(action_data, "Blue? ");
	for(int r = 0; r < action_data.getInputImage1().getHeight(); r++) {
		for(int c = 0; c < action_data.getInputImage1().getWidth(); c++) {
			int row_diff = abs(row - r);
			int col_diff = abs(column - c);
			double distance = std::sqrt((row_diff * row_diff) + (col_diff * col_diff));
			if(distance <= radius) {
				action_data.getInputImage1().setChannel(r, c, 0, red);
				action_data.getInputImage1().setChannel(r, c, 1, green);
				action_data.getInputImage1().setChannel(r, c, 2, blue);
			}
		}
	}
}

void drawBox(ActionData& action_data) {
	int TR = getInteger(action_data, "Top Row? ");
	int LC = getInteger(action_data, "Left Column? ");
	int BR = getInteger(action_data, "Bottom Row? ");
	int RC = getInteger(action_data, "Right Column? ");
	int red = getInteger(action_data, "Red? ");
	int green = getInteger(action_data, "Green? ");
	int blue = getInteger(action_data, "Blue? ");
	for(int r = 0; r < action_data.getInputImage1().getHeight(); r++) {
		for(int c = 0; c < action_data.getInputImage1().getWidth(); c++) {
			if(r >= TR && r <= BR && c <= RC && c >= LC) {
				action_data.getInputImage1().setChannel(r, c, 0, red);
				action_data.getInputImage1().setChannel(r, c, 1, green);
				action_data.getInputImage1().setChannel(r, c, 2, blue);
			}
		}
	}
}

void drawSquare(ActionData& action_data) {
	int row = getInteger(action_data, "Row? ");
	int column = getInteger(action_data, "Column? ");
	int size = getInteger(action_data, "Size? ");
	int red = getInteger(action_data, "Red? ");
	int green = getInteger(action_data, "Green? ");
	int blue = getInteger(action_data, "Blue? ");
	int top = row - (size/2);
	int bottom = row + (size/2);
	int left = column - (size/2);
	int right = column + (size/2);
	for(int r = 0; r < action_data.getInputImage1().getHeight(); r++) {
		for(int c = 0; c < action_data.getInputImage1().getWidth(); c++) {
			if(r >= top && r <= bottom && c >= left && c <= right) {
				action_data.getInputImage1().setChannel(r, c, 0, red);
				action_data.getInputImage1().setChannel(r, c, 1, green);
				action_data.getInputImage1().setChannel(r, c, 2, blue);
			}
		}
	}

}

void configureGrid(ActionData& action_data) {
	int height = getInteger(action_data, "Grid Height? ");
	int width = getInteger(action_data, "Grid Width? ");
	int max_number = getInteger(action_data, "Grid Max Value? ");

	action_data.getGrid().setGridSize(height, width);
	action_data.getGrid().setMaxNumber(max_number);
}

void setGrid(ActionData& action_data) {
	int row = getInteger(action_data, "Grid Row? ");
	int column = getInteger(action_data, "Grid Column? ");
	int value = getInteger(action_data, "Grid Value? ");
	action_data.getGrid().setNumber(row, column, value);
}

void applyGrid(ActionData& action_data) {
	action_data.getGrid().setPPM(action_data.getOutputImage());
}

void setColorTableSize(ActionData& action_data) {
	int size = getInteger(action_data, "Size? ");
	action_data.getTable().setNumberOfColors(size);
}

void setColor(ActionData& action_data) {
	ColorTable& table = action_data.getTable();
	int position = getInteger(action_data, "Position? ");
	int red = getInteger(action_data, "Red? ");
	int green = getInteger(action_data, "Green? ");
	int blue = getInteger(action_data, "Blue? ");
	table[position].setRed(red);
	table[position].setGreen(green);
	table[position].setBlue(blue);
}

void setRandomColor(ActionData& action_data) {
	int position = getInteger(action_data, "Position? ");
	action_data.getTable().setRandomColor(255, position);
}

void setColorGradient(ActionData& action_data) {
	int position1 = getInteger(action_data, "First position? ");
	int red1 = getInteger(action_data, "First red? ");
	int green1 = getInteger(action_data, "First green? ");
	int blue1 = getInteger(action_data, "First blue? ");
	int position2 = getInteger(action_data, "Second position? ");
	int red2 = getInteger(action_data, "Second red? ");
	int green2 = getInteger(action_data, "Second green? ");
	int blue2 = getInteger(action_data, "Second blue? ");

	Color color1(red1, green1, blue1);
	Color color2(red2, green2, blue2);

	action_data.getTable().insertGradient(color1, color2, position1, position2);
}

void applyGridColorTable(ActionData& action_data) {
	action_data.getGrid().setPPM(action_data.getOutputImage(), action_data.getTable());
}

void setFractalPlaneSize(ActionData& action_data) {
	NumberGrid *ptr = &action_data.getGrid();
	ComplexFractal *CF = dynamic_cast<ComplexFractal *>(ptr);
	if(CF != 0) {
		double min_x = getDouble(action_data, "Min X? ");
		double max_x = getDouble(action_data, "Max X? ");
		double min_y = getDouble(action_data, "Min Y? ");
		double max_y = getDouble(action_data, "Max Y? ");
		CF->setPlaneSize(min_x, max_x, min_y, max_y);
	} else {
		action_data.getOS() << "Not a ComplexFractal object. Can't set plane size." << std::endl;
	}
}

void calculateFractal(ActionData& action_data) {
	action_data.getGrid().calculateAllNumbers();
}

void setJuliaParameters(ActionData& action_data) {
	NumberGrid *ptr = &action_data.getGrid();
	JuliaSet *JS = dynamic_cast<JuliaSet *>(ptr);
	if(JS != 0) {
		double A = getDouble(action_data, "Parameter a? ");
		double B = getDouble(action_data, "Parameter b? ");
		JS->setParameters(A, B);
	} else {
		action_data.getOS() << "Not a JuliaSet object. Can't set parameters." << std::endl;
	}

}

void setMandelbrotPower(ActionData& action_data) {
	NumberGrid *ptr = &action_data.getGrid();
	MandelbrotPower *MP = dynamic_cast<MandelbrotPower *>(ptr);
	if(MP != 0) {
		double power = getDouble(action_data, "Power? ");
		MP->setPower(power);
	} else {
		action_data.getOS() << "Not a MandelbrotPower object. Can’t set power." << std::endl;
	}
}

void calculateFractalSingleThread(ActionData& action_data) {
	action_data.getGrid().NumberGrid::calculateAllNumbers();
}
