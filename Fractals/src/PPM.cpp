#include "PPM.h"
#include "Image.h"
#include "image_menu.h"

PPM::PPM() 
	: Image(), mMaxColor(1)	{

}

PPM::PPM(const int& height, const int& width) 
	:Image(height, width), mMaxColor(1) {

}

int PPM::getMaxColorValue() const {
	return mMaxColor;
}

bool PPM::valueValid(const int& value) const {
	if(value >= 0 && value <= mMaxColor) {
		return true;
	}
	return false;
}

void PPM::setMaxColorValue(const int& max_color_value) {
	if(max_color_value >= 1 && max_color_value <= 255) {
		mMaxColor = max_color_value;
	}
}

void PPM::setChannel(const int& row, const int& column, const int& channel, const int& value) {
	bool valid = valueValid(value);
	if(valid) {
		Image::setChannel(row, column, channel, value);
	}
}

void PPM::setPixel(const int& row, const int& column, const int& red, const int& green, const int& blue) {
	Image::setChannel(row, column, 0, red);
	Image::setChannel(row, column, 1, green);
	Image::setChannel(row, column, 2, blue);
		
}

void PPM::writeStream(std::ostream& os) const {
	os << "P6 " << Image::getWidth() << " "  << Image::getHeight() << " " << mMaxColor << "\n";
	
	for(int row = 0; row < Image::getHeight(); row++) {
		for(int column = 0; column < Image::getWidth(); column++) {
			unsigned char c = getChannel(row, column, 0);
			os.write((char*) &c, sizeof(c));
			c = getChannel(row, column, 1);
			os.write((char*) &c, sizeof(c));
			c = getChannel(row, column, 2);
			os.write((char*) &c, sizeof(c));
		}
	}
}

void PPM::readStream(std::istream& is) {
	std::string p6;
	is >> p6;

	int width;
	is >> width;
	Image::setWidth(width);

	int height;
	is >> height;
	Image::setHeight(height);

	int color;
	is >> color;
	setMaxColorValue(color);

	//read past newline
	unsigned char c;
	is.read((char *) &c, 1);


	for(int row = 0; row < Image::getHeight(); row++) {
		for(int column = 0; column < Image::getWidth(); column++) {
			for(int channel = 0; channel < 3; channel++) {
				is.read((char *) &c, 1);
				setChannel(row, column, channel, c);
			}
			
		}	
	}	
}

//operators
bool PPM::operator==(const PPM& rhs) const {
	if(getHeight() * getWidth() == rhs.getHeight() * rhs.getWidth()) {
		return true;
	} else {
		return false;
	}
}

bool PPM::operator!=(const PPM& rhs) const {
	if(getHeight() * getWidth() != rhs.getHeight() * rhs.getWidth()) {
		return true;
	} else {
		return false;
	}	
}

bool PPM::operator<(const PPM& rhs) const {
	if(getHeight() * getWidth() < rhs.getHeight() * rhs.getWidth()) {
		return true;
	} else {
		return false;
	}	
}

bool PPM::operator<=(const PPM& rhs) const {
	if(getHeight() * getWidth() <= rhs.getHeight() * rhs.getWidth()) {
		return true;
	} else {
		return false;
	}	
}

bool PPM::operator>(const PPM& rhs) const {
	if(getHeight() * getWidth() > rhs.getHeight() * rhs.getWidth()) {
		return true;
	} else {
		return false;
	}	
}

bool PPM::operator>=(const PPM& rhs) const {
	if(getHeight() * getWidth() >= rhs.getHeight() * rhs.getWidth()) {
		return true;
	} else {
		return false;
	}	
}

PPM& PPM::operator+=(const PPM& rhs) {
	for(int row = 0; row < getHeight(); row++) {
		for(int column = 0; column < getWidth(); column++) {
			for(int channel = 0; channel < 3; channel++) {
				int chanV1 = getChannel(row, column, channel);
				int chanV2 = rhs.getChannel(row, column, channel);
				int chanV = chanV1 + chanV2;
				if(chanV > getMaxColorValue()) {
					chanV = getMaxColorValue();
				}
				setChannel(row, column, channel, chanV); 
			}
		}
	}
	return *this;
}

PPM& PPM::operator-=(const PPM& rhs) {
	for(int row = 0; row < getHeight(); row++) {
		for(int column = 0; column < getWidth(); column++) {
			for(int channel = 0; channel < 3; channel++) {
				int chanV1 = getChannel(row, column, channel);
				int chanV2 = rhs.getChannel(row, column, channel);
				int chanV = chanV1 - chanV2;
				if(chanV > getMaxColorValue()) {
					chanV = getMaxColorValue();
				}
				if(chanV < 0) {
					chanV = 0;
				}
				setChannel(row, column, channel, chanV); 
			}
		}
	}
	return *this;
}

PPM& PPM::operator*=(const double& rhs) {
	for(int row = 0; row < getHeight(); row++) {
		for(int column = 0; column < getWidth(); column++) {
			for(int channel = 0; channel < 3; channel++) {
				int chanV1 = getChannel(row, column, channel);
				int chanV = chanV1 * rhs;
				if(chanV > getMaxColorValue()) {
					chanV = getMaxColorValue();
				}
				if(chanV < 0) {
					chanV = 0;
				}
				setChannel(row, column, channel, chanV); 
			}
		}
	}
	return *this;
}

PPM& PPM::operator/=(const double& rhs) {
	for(int row = 0; row < getHeight(); row++) {
		for(int column = 0; column < getWidth(); column++) {
			for(int channel = 0; channel < 3; channel++) {
				int chanV1 = getChannel(row, column, channel);
				int chanV = chanV1 / rhs;
				if(chanV > getMaxColorValue()) {
					chanV = getMaxColorValue();
				}
				if(chanV < 0) {
					chanV = 0;
				}
				setChannel(row, column, channel, chanV); 
			}
		}
	}
	return *this;
}

PPM PPM::operator+(const PPM& rhs) const {
	PPM p = *this;
	p += rhs;
	return p;
}

PPM PPM::operator-(const PPM& rhs) const {
	PPM p = *this;
	p -= rhs;
	return p;
}

PPM PPM::operator*(const double& rhs) const {
	PPM p = *this;
	p *= rhs;
	return p;
}

PPM PPM::operator/(const double& rhs) const {
	PPM p = *this;
	p /= rhs;
	return p;
}

//more methods
void PPM::grayFromChannel(PPM& dst, const int& src_channel) const {
	dst.setHeight(getHeight());
	dst.setWidth(getWidth());
	dst.setMaxColorValue(getMaxColorValue());
	for(int row = 0; row < getHeight(); row++) {
		for(int column = 0; column < getWidth(); column++) {
			int val = getChannel(row, column, src_channel);
			dst.setChannel(row, column, 0, val);	
			dst.setChannel(row, column, 1, val);	
			dst.setChannel(row, column, 2, val);	
		}
	}
}

void PPM::grayFromRed(PPM& dst) const {
	grayFromChannel(dst, 0);
}

void PPM::grayFromGreen(PPM& dst) const {
	grayFromChannel(dst, 1);
}

void PPM::grayFromBlue(PPM& dst) const {
	grayFromChannel(dst, 2);
}

double PPM::linearColorimetricPixelValue(const int& row, const int& column) const {
	int red = getChannel(row, column, 0);
	int green = getChannel(row, column, 1);
	int blue = getChannel(row, column, 2);
	double brightness = (0.2126 * red) + (0.7152 * green) + (0.0722 * blue);
	return brightness;
}

void PPM::grayFromLinearColorimetric(PPM& dst) const {
	dst.setHeight(getHeight());
	dst.setWidth(getWidth());
	dst.setMaxColorValue(getMaxColorValue());
	for(int row = 0; row < getHeight(); row++) {
		for(int column = 0; column < getWidth(); column++) {
			int val = linearColorimetricPixelValue(row, column);
			dst.setChannel(row, column, 0, val);	
			dst.setChannel(row, column, 1, val);	
			dst.setChannel(row, column, 2, val);	
		}
	}
}

void PPM::orangeFilter(PPM& dst) const {
	dst.setHeight(getHeight());
	dst.setWidth(getWidth());
	dst.setMaxColorValue(getMaxColorValue());
	for(int row = 0; row < getHeight(); row++) {
		for(int column = 0; column < getWidth(); column++) {
			//get current channel values
			int old_red = getChannel(row, column, 0);
			int old_green = getChannel(row, column, 1);
			int old_blue = getChannel(row, column, 2);

			//calculate new values
			int new_red = 2*(2*old_red+old_green)/3; 
			int new_green = 2*(2*old_red+old_green)/6;
			int new_blue = old_blue/2;

			//check if greater than max value
			if(new_red > getMaxColorValue()) {
				new_red = getMaxColorValue();
			}

			if(new_green > getMaxColorValue()) {
				new_green = getMaxColorValue();
			}

			if(new_blue > getMaxColorValue()) {
				new_blue = getMaxColorValue();
			}

			//set output values
			dst.setChannel(row, column, 0, new_red);	
			dst.setChannel(row, column, 1, new_green);	
			dst.setChannel(row, column, 2, new_blue);	
		}
	}	
}

int PPM::antiAliasPixelValue(int n, int row, int column, int channel) const {
	int total = 0;
	int row_start = row * n;
	int row_end = (row + 1) * n;
	int column_start = column * n;
	int column_end = (column + 1) * n;
	for(int r = row_start; r < row_end; r++) {
		for(int c = column_start; c < column_end; c++) {
			total += getChannel(r, c, channel);
		}
	}
	return (total/(n*n));
}

void PPM::antiAlias(const int& n, PPM& dst) const {
	dst.setHeight(getHeight() / n);
	dst.setWidth(getWidth() / n);
	dst.setMaxColorValue(getMaxColorValue());

	for(int row = 0; row < dst.getHeight(); row++) {
		for(int column = 0; column < dst.getWidth(); column++) {
			for(int channel = 0; channel < 3; channel++) {
				int value = antiAliasPixelValue(n, row, column, channel);
				dst.setChannel(row, column, channel, value);
			}
		}
	}
}
