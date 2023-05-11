#include "JuliaSet.h"

#include <cmath>

//constructors
JuliaSet::JuliaSet() 
	: ComplexFractal(), mA(-0.650492), mB(-0.478235) {
	setMaxNumber(255);
}

JuliaSet::JuliaSet(const int& height, const int& width, const double& min_x, const double& max_x, const double& min_y, const double& max_y, const double& a, const double& b) 
	: ComplexFractal(height, width, min_x, max_x, min_y, max_y), mA(a), mB(b) {
	//didnt say in the instructions, but you might need to set max number
}

JuliaSet::~JuliaSet() {

}

double JuliaSet::getA() const {
	return mA;
}

double JuliaSet::getB() const {
	return mB;
}

void JuliaSet::setParameters(const double& a, const double& b) {
	if(a >= -2 && a <= 2 && b >= -2 && b <= 2) {
		mA = a;
		mB = b;
	}
}

void JuliaSet::calculateNextPoint(const double x0, const double y0, double& x1, double &y1) const {
	x1 = x0 * x0 - y0 * y0 + mA;
	y1 = 2 * x0 * y0 + mB;
}

int JuliaSet::calculatePlaneEscapeCount(const double& x0, const double& y0) const {
	int max_iterations = getMaxNumber();
	double x1, y1;
	double xc = x0;
	double yc = y0;
	for(int i = 0; i <= max_iterations; i++) {
		x1 = xc;
		y1 = yc;
		double dist = sqrt(x1 * x1 + y1 * y1);
		if(dist > 2) {
			return i;
		} else {
			calculateNextPoint(x1, y1, xc, yc);
		}
	}
	return max_iterations;
}

int JuliaSet::calculateNumber(const int& row, const int& column) const {
	if(row < 0 || row >= getHeight() || column < 0 || column >= getWidth()) {
		return -1;
	}
	double x;
	double y;
	calculatePlaneCoordinatesFromPixelCoordinates(row, column, x, y);
	int escape = calculatePlaneEscapeCount(x, y);
	return escape;
}
