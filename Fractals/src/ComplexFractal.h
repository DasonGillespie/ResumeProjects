#ifndef _COMPLEX_FRACTAL_
#define _COMPLEX_FRACTAL_

#include "NumberGrid.h"
#include "ThreadedGrid.h"

class ComplexFractal: public ThreadedGrid {
	public:
		//constructors
		ComplexFractal();
		ComplexFractal(const int& height, const int& width, const double& min_x, const double& max_x, const double& min_y, const double& max_y);
		virtual ~ComplexFractal();

		//getters
		double getMinX() const;
		double getMaxX() const;
		double getMinY() const;
		double getMaxY() const;
		double getDeltaX() const;
		double getDeltaY() const;

		//methods
		virtual void setGridSize(const int& height, const int& width);
		void setPlaneSize(const double& min_x, const double& max_x, const double& min_y, const double& max_y);
		void setDeltas(const double& delta_x, const double& delta_y);
		double calculateDeltaX() const;
		double calculateDeltaY() const;
		double calculatePlaneXFromPixelColumn(const int& column) const;
		double calculatePlaneYFromPixelRow(const int& row) const;
		void calculatePlaneCoordinatesFromPixelCoordinates(const int& row, const int& column, double& x, double& y) const;
		virtual int calculateNumber(const int& row, const int& column) const;

	private:
		//data members
		double mMinX;
		double mMinY;
		double mMaxX;
		double mMaxY;
		double mDeltaX;
		double mDeltaY;

		
};

#endif
