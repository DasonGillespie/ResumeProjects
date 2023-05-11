#include "ColorTable.h"
#include <cstdlib>
#include <iostream>

//Color Class
Color::Color() 
	: mRed(0), mGreen(0), mBlue(0) {

}

Color::Color(const int& red, const int& green, const int& blue) 
	: mRed(red), mGreen(green), mBlue(blue) {

}

int Color::getRed() const {
	return mRed;
}

int Color::getGreen() const {
	return mGreen;
}

int Color::getBlue() const {
	return mBlue;
}

int Color::getChannel(const int& channel) const {
	switch(channel) {
	case 0:
		return mRed;
	case 1:
		return mGreen;
	case 2: 
		return mBlue;
	default:
		return -1;
	}
}

void Color::setRed(const int& value) {
	if(value >= 0) {
		mRed = value;
	}
}

void Color::setGreen(const int& value) {
	if(value >= 0) {
		mGreen = value;
	}
}

void Color::setBlue(const int& value) {
	if(value >= 0) {
		mBlue = value;
	}
}

void Color::setChannel(const int& channel, const int& value) {
	if(value >= 0) {
		switch(channel) {
		case 0:
			setRed(value);
			break;
		case 1:
			setGreen(value);
			break;
		case 2:
			setBlue(value);
			break;
		default:
			break;
		}
	}
}

void Color::invert(const int& max_color_value) {
	if(max_color_value >= mRed && max_color_value >= mGreen && max_color_value >= mBlue) {
		setRed(max_color_value - mRed);
		setGreen(max_color_value - mGreen);
		setBlue(max_color_value - mBlue);
	}
}

bool Color::operator==(const Color& rhs) const {
	if(getRed() == rhs.getRed() && getGreen() == rhs.getGreen() && getBlue() == rhs.getBlue()) {
		return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const Color& color) {
	return os << color.getRed() << ":" << color.getGreen() << ":" << color.getBlue();
}

//ColorTable Class

ColorTable::ColorTable(const int& num_color) 
	: mColors(num_color) {

}

int ColorTable::getNumberOfColors() const {
	return mColors.size();

}

void ColorTable::setNumberOfColors(const int& num_color) {
	mColors.resize(num_color);
}

const Color& ColorTable::operator[](const int& i) const {
	if(i > getNumberOfColors() - 1 || i < 0) {
		static Color ec( -1, -1, -1 );
		static Color c( -1, -1, -1 );
		c = ec;
		return c;
	}
	return mColors[i];
}

Color& ColorTable::operator[](const int& i) {
	if(i > getNumberOfColors() - 1 || i < 0) {
		static Color ec( -1, -1, -1 );
		static Color c( -1, -1, -1 );
		c = ec;
		return c;
	}
	return mColors[i];
}

void ColorTable::setRandomColor(const int& max_color_value, const int& position) {
	if(max_color_value >= 0 && position < getNumberOfColors() && position >= 0) {
		int random;
		for(int i = 0; i < 3; i++) {
			random = std::rand();
			random = random % (max_color_value + 1);
			mColors[position].setChannel(i, random);
		}
	}
}

double ColorTable::gradientSlope(const double y1, const double y2, const double x1, const double x2) const {
	double x = x2 - x1;
	double y = y2 - y1;
	double slope = y / x;
	return slope;
}

double ColorTable::gradientValue(const double y1, const double x1, const double slope, const double x) const {
	double x_dif = x - x1;
	double rise = x_dif * slope;
	double value = rise + y1;
	return value;
}

void ColorTable::insertGradient(const Color& color1, const Color& color2, const int& position1, const int& position2) {
	if(position2 > position1 && position1 >= 0 && position2 >= 0 && position1 < getNumberOfColors() && position2 < getNumberOfColors()) {

		//get the beginning and ending channel values
		double red1 = color1.getRed();
		double green1 = color1.getGreen();
		double blue1 = color1.getBlue();
		double red2 = color2.getRed();
		double green2 = color2.getGreen();
		double blue2 = color2.getBlue();

		//calcuate slopes
		double slope_red = gradientSlope(red1, red2, position1, position2);
		double slope_green = gradientSlope(green1, green2, position1, position2);
		double slope_blue = gradientSlope(blue1, blue2, position1, position2);

		//loop through mColors from the indexes of position1 to position2
		for(int i = position1; i < position2 + 1; i++) {

			//calculate values at current position i based on slopes found
			int red_value = gradientValue(red1, position1, slope_red, i);
			int green_value = gradientValue(green1, position1, slope_green, i);
			int blue_value = gradientValue(blue1, position1, slope_blue, i);

			//set color values(r,g,b) of the Color in mColors at position i
			mColors[i].setRed(red_value);
			mColors[i].setGreen(green_value);
			mColors[i].setBlue(blue_value);
		}
	}
}

int ColorTable::getMaxChannelValue() const {
	int max = 0;
	for(int i = 0; i < getNumberOfColors(); i++) {
		if(mColors[i].getRed() > max) {
			max = mColors[i].getRed();
		}
		if(mColors[i].getGreen() > max) {
			max = mColors[i].getGreen();
		}
		if(mColors[i].getBlue() > max) {
			max = mColors[i].getBlue();
		}
	}
	return max;
}
