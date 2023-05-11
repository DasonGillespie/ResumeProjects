#include "NumberGrid.h"
#include "ComplexFractal.h"
#include "ThreadedGrid.h"
#include <cmath>

//constructors
ComplexFractal::ComplexFractal() 
	: ThreadedGrid(201, 301), mMinX(-1.5), mMinY(-1), mMaxX(1.5), mMaxY(1), mDeltaX(0.01), mDeltaY(0.01){
	
}

ComplexFractal::ComplexFractal(const int& height, const int& width, const double& min_x, const double& max_x, const double& min_y, const double& max_y) 
	: ThreadedGrid(height, width), mMinX(min_x), mMinY(min_y), mMaxX(max_x), mMaxY(max_y), mDeltaX(), mDeltaY() {
	setDeltas(calculateDeltaX(), calculateDeltaY());
}

ComplexFractal::~ComplexFractal() {

}

//getters
double ComplexFractal::getMinX() const {
	return mMinX;
}

double ComplexFractal::getMaxX() const {
	return mMaxX;
}

double ComplexFractal::getMinY() const {
	return mMinY;
}

double ComplexFractal::getMaxY() const {
	return mMaxY;
}

double ComplexFractal::getDeltaX() const {
	return mDeltaX;
}

double ComplexFractal::getDeltaY() const {
	return mDeltaY;
}

//methods
void ComplexFractal::setGridSize(const int& height, const int& width) {
	if(height >= 2 && width >= 2) {
		NumberGrid::setGridSize(height, width);
		setDeltas(calculateDeltaX(), calculateDeltaY());
	}	
}

void ComplexFractal::setPlaneSize(const double& min_x, const double& max_x, const double& min_y, const double& max_y) {
	if(min_x >= -2 && min_x <= 2 && min_y >= -2 && min_y <= 2) {
		if(max_x >= -2 && max_x <= 2 && max_y >= -2 && max_y <= 2) {
			if(min_x != max_x && min_y != max_y) {
				if(min_x > max_x) {
					mMinX = max_x;
					mMaxX = min_x;
				} else {
					mMinX = min_x;
					mMaxX = max_x;
				}

				if(min_y > max_y) {
					mMinY = max_y;
					mMaxY = min_y;
				} else {
					mMinY = min_y;
					mMaxY = max_y;
				}

				setDeltas(calculateDeltaX(), calculateDeltaY());
			}
		}
	}
}

void ComplexFractal::setDeltas(const double& delta_x, const double& delta_y) {
	if(delta_x >= 0 && delta_y >= 0) {
		mDeltaX = delta_x;
		mDeltaY = delta_y;
	}
}

double ComplexFractal::calculateDeltaX() const {
	double delta = (mMaxX - mMinX)/double(getWidth() - 1);
	return delta;
}

double ComplexFractal::calculateDeltaY() const {
	double delta = (mMaxY - mMinY)/double(getHeight() - 1);
	return delta;
}

double ComplexFractal::calculatePlaneXFromPixelColumn(const int& column) const {
	if(column < 0 || column >= getWidth()) {
		return 0;
	}
	return getMinX() + (getDeltaX() * column);
}

double ComplexFractal::calculatePlaneYFromPixelRow(const int& row) const {
	if(row < 0 || row >= getHeight()) {
		return 0;
	}
	return getMaxY() - (getDeltaY() * row);
}

void ComplexFractal::calculatePlaneCoordinatesFromPixelCoordinates(const int& row, const int& column, double& x, double& y) const {
	x = calculatePlaneXFromPixelColumn(column);
	y = calculatePlaneYFromPixelRow(row);
	//if(x == 0 || y == 0) {
	//	x = 0;
	//	y = 0;
	//}
}

int ComplexFractal::calculateNumber(const int& row, const int& column) const {
	if(column < 0 || column >= getWidth() || row < 0 || row >= getHeight()) {
		return -1;
	}	
	double x;
	double y;
	calculatePlaneCoordinatesFromPixelCoordinates(row, column, x, y);
	return std::abs(getMaxNumber() * std::sin(10 * x) * std::cos(10 * y));
}
