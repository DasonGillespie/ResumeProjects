#include "MandelbrotSet.h"

#include <cmath>

//Mandelbrot Set
MandelbrotSet::MandelbrotSet()
	: ComplexFractal() {

}

MandelbrotSet::MandelbrotSet(const int& height, const int& width, const double& min_x, const double& max_x, const double& min_y, const double& max_y)
	: ComplexFractal(height, width, min_x, max_x, min_y, max_y) {
	
}

MandelbrotSet::~MandelbrotSet() {

}

void MandelbrotSet::calculateNextPoint(const double x0, const double y0, const double& a, const double& b, double& x1, double &y1) const {
	x1 = x0 * x0 - y0 * y0 + a; 
	y1 = 2 * x0 * y0 + b;
}

int MandelbrotSet::calculatePlaneEscapeCount(const double& a, const double& b) const {
	int max_iterations = getMaxNumber();
	double x1, y1;
	double xc = a;
	double yc = b;
	for(int i = 0; i < max_iterations; i++) {
		x1 = xc;
		y1 = yc;
		double dist = sqrt(x1 * x1 + y1 * y1);
		if(dist > 2) {
			return i;
		} else {
			calculateNextPoint(x1, y1, a, b, xc, yc);
		}
	}
	return max_iterations;	
}

int MandelbrotSet::calculateNumber(const int& row, const int& column) const {
	if(row < 0 || row >= getHeight() || column < 0 || column >= getWidth()) {
		return -1;
	}
	double x;
	double y;
	calculatePlaneCoordinatesFromPixelCoordinates(row, column, x, y);
	int escape = calculatePlaneEscapeCount(x, y);
	return escape;
}

//Mandelbrot Power (Exam)
MandelbrotPower::MandelbrotPower()
	: MandelbrotSet(), mPower(2.0) {

}

MandelbrotPower::~MandelbrotPower() {

}

double MandelbrotPower::getPower() const {
	return mPower;
}

void MandelbrotPower::setPower(const double& power) {
	mPower = power;
}

void MandelbrotPower::calculateNextPoint(const double x0, const double y0, const double& a, const double& b, double& x1, double &y1) const {
	auto r = std::sqrt(x0 * x0 + y0 * y0);
	auto theta = std::atan2(y0, x0);
	x1 = std::pow(r, getPower()) * std::cos(getPower() * theta) + a;
	y1 = std::pow(r, getPower()) * std::sin(getPower() * theta) + b;
}
