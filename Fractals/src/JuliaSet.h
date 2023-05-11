#ifndef _JULIA_SET_
#define _JULIA_SET_

#include "ComplexFractal.h"

class JuliaSet: public ComplexFractal {
	public:
		//constructors
		JuliaSet();
		JuliaSet(const int& height, const int& width, const double& min_x, const double& max_x, const double& min_y, const double& max_y, const double& a, const double& b);
		virtual ~JuliaSet();

		//methods
		double getA() const;
		double getB() const;
		void setParameters(const double& a, const double& b);
		virtual void calculateNextPoint(const double x0, const double y0, double& x1, double &y1) const;
		virtual int calculatePlaneEscapeCount(const double& x0, const double& y0) const;
		virtual int calculateNumber(const int& row, const int& column) const;

	private:
		//data members
		double mA;
		double mB;

};

#endif
