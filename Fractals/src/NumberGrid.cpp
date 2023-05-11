#include "NumberGrid.h"
#include "ComplexFractal.h"
#include <vector>

NumberGrid::NumberGrid() 
	: mHeight(300), mWidth(400), mMaxNumber(255), mGrid(mWidth * mHeight) {
		for(int row = 0; row < mHeight; row++) {
			for(int column; column < mWidth; column++) {
				mGrid[(row * mWidth) + column] = 0;
			}
		}
}

NumberGrid::NumberGrid(const int& height, const int& width) 
	: mHeight(height), mWidth(width), mMaxNumber(255), mGrid(mWidth * mHeight) {
		for(int row = 0; row < mHeight; row++) {
			for(int column; column < mWidth; column++) {
				mGrid[(row * mWidth) + column] = 0;
			}
		}	
}

NumberGrid::~NumberGrid() {
	
}

int NumberGrid::getHeight() const {
	return mHeight;
}

int NumberGrid::getWidth() const {
	return mWidth;
}

int NumberGrid::getMaxNumber() const {
	return mMaxNumber;
}

void NumberGrid::setGridSize(const int& height, const int& width) {
	if(height >= 2 && width >= 2) {
		mHeight = height;
		mWidth = width;
		mGrid.resize(width * height);
		//does nothing to the values in the grid
		//left undefined
	}
}

void NumberGrid::setMaxNumber(const int& number) {
	if(number > 0) {
		mMaxNumber = number;
		//values in grid that are greater after change are undefined
	}
}

const std::vector<int>& NumberGrid::getNumbers() const {
	return mGrid;
}

int NumberGrid::index(const int& row, const int& column) const {
	return (row * mWidth) + column;
}

bool NumberGrid::indexValid(const int& row, const int& column) const {
	int i = index(row, column);
	if(row >= 0 && row < mHeight && column >= 0 && column < mWidth && i <= mHeight * mWidth) {
		return true;
	}
	return false;
}

bool NumberGrid::numberValid(const int& number) const {
	if(number >= 0 && number <= mMaxNumber) {
		return true;
	}
	return false;
}

int NumberGrid::getNumber(const int& row, const int& column) const {
	if(!indexValid(row, column)) {
		return -1;
	}
	return mGrid[index(row, column)];
}

void NumberGrid::setNumber(const int& row, const int& column, const int& number) {
	if(numberValid(number) && indexValid(row, column)) {
		mGrid[index(row, column)] = number;
	}
}

void NumberGrid::setPPM(PPM& ppm) const {
	//configure the ppm object
	ppm.setHeight(mHeight);
	ppm.setWidth(mWidth);
	ppm.setMaxColorValue(63);

	//set pixel values based on number in grid
	for(int row = 0; row < mHeight; row++) {
		for(int column = 0; column < mWidth; column++) {
			int grid_value = mGrid[index(row, column)];
			if(grid_value == 0) {
				ppm.setPixel(row, column, 0, 0, 0);
			} else if(grid_value == mMaxNumber) {
				ppm.setPixel(row, column, 63, 31, 31);
			} else {
				switch(grid_value % 8) {

				case 0:
					ppm.setPixel(row, column, 63, 63, 63);
					break;
				case 1:
					ppm.setPixel(row, column, 63, 31, 31);
					break;
				case 2:
					ppm.setPixel(row, column, 63, 63, 31);
					break;
				case 3:
					ppm.setPixel(row, column, 31, 63, 31);
					break;
				case 4:
					ppm.setPixel(row, column, 0, 0, 0);
					break;
				case 5:
					ppm.setPixel(row, column, 31, 63, 63);
					break;
				case 6:
					ppm.setPixel(row, column, 31, 31, 63);
					break;
				case 7:
					ppm.setPixel(row, column, 63, 31, 63);
					break;
				default:
					break;
				}
			}
		}
	}
}

void NumberGrid::setPPM(PPM& ppm, const ColorTable& colors) const {
	if(colors.getNumberOfColors() >= 2) {
		ppm.setHeight(mHeight);
		ppm.setWidth(mWidth);
		ppm.setMaxColorValue(colors.getMaxChannelValue());
		for(int row = 0; row < ppm.getHeight(); row++) {
			for(int column = 0; column < ppm.getWidth(); column++) {
				int number = getNumber(row, column);
				if(number == mMaxNumber) {
					int red = colors[colors.getNumberOfColors() - 1].getRed();
					int green = colors[colors.getNumberOfColors() - 1].getGreen();
					int blue = colors[colors.getNumberOfColors() - 1].getBlue();

					ppm.setPixel(row, column, red, green, blue);
				} else {
					int red = colors[number % colors.getNumberOfColors()].getRed();
					int green = colors[number % colors.getNumberOfColors()].getGreen();
					int blue = colors[number % colors.getNumberOfColors()].getBlue();

					ppm.setPixel(row, column, red, green, blue);
					
				}
			}
		}
	}
}

void NumberGrid::calculateAllNumbers() {
	for(int row = 0; row < getHeight(); row++) {
		for(int column = 0; column < getWidth(); column++) {
			int number = calculateNumber(row, column);
			setNumber(row, column, number);
		}
	}
}

//Manhattan Numbers (Exam)
ManhattanNumbers::ManhattanNumbers()
	: NumberGrid(600, 800) {

}

ManhattanNumbers::ManhattanNumbers(const int& height, const int& width)
	: NumberGrid(height, width) {

}

ManhattanNumbers::~ManhattanNumbers() {

}

int ManhattanNumbers::calculateNumber(const int& row, const int& column) const {
	int center_y = getHeight() / 2;
	int center_x = getWidth() / 2;
	int M_dist = std::abs(center_y - row) + std::abs(center_x - column);
	return M_dist;
}
