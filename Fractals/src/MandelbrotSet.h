#ifndef _MANDELBROT_SET_
#define _MANDELBROT_SET_

#include "ComplexFractal.h"

class MandelbrotSet: public ComplexFractal {
	public:
		//constructors
		MandelbrotSet();
		MandelbrotSet(const int& height, const int& width, const double& min_x, const double& max_x, const double& min_y, const double& max_y);
		virtual ~MandelbrotSet();

		//methods
		virtual void calculateNextPoint(const double x0, const double y0, const double& a, const double& b, double& x1, double &y1) const;
		int calculatePlaneEscapeCount(const double& a, const double& b) const;
		virtual int calculateNumber(const int& row, const int& column) const;

	private:
		//data members
		//no data members

};

class MandelbrotPower: public MandelbrotSet {
	public:
		//constructors
		MandelbrotPower();
		virtual ~MandelbrotPower();

		//methods
		double getPower() const;
		void setPower(const double& power);
		virtual void calculateNextPoint(const double x0, const double y0, const double& a, const double& b, double& x1, double &y1) const;

	private:
		//data members
		double mPower;
};

#endif
