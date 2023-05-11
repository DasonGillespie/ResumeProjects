#ifndef _COLOR_TABLE_
#define _COLOR_TABLE_

#include <vector>
#include <iostream>

class Color {
	public:
		//constructors
		Color();
		Color(const int& red, const int& green, const int& blue);

		//getters
		int getRed() const;
		int getGreen() const;
		int getBlue() const;
		int getChannel(const int& channel) const;

		//methods
		void setRed(const int& value);
		void setGreen(const int& value);
		void setBlue(const int& value);
		void setChannel(const int& channel, const int& value);
		void invert(const int& max_color_value);
		bool operator==(const Color& rhs) const;

	private:
		//data members
		int mRed;
		int mGreen;
		int mBlue;

};

class ColorTable {
	public:
		//constructors
		ColorTable(const int& num_color);

		//getters
		int getNumberOfColors() const;

		//methods
		void setNumberOfColors(const int& num_color);
		const Color& operator[](const int& i) const;
		Color& operator[](const int& i);
		void setRandomColor(const int& max_color_value, const int& position);
		double gradientSlope(const double y1, const double y2, const double x1, const double x2) const;
		double gradientValue(const double y1, const double x1, const double slope, const double x) const;
		void insertGradient(const Color& color1, const Color& color2, const int& position1, const int& position2);
		int getMaxChannelValue() const;
	
	private:
		//data members
		std::vector<Color> mColors;
};

//operators
std::ostream& operator<<(std::ostream& os, const Color& color);

#endif
