#ifndef _NUMBER_GRID_H_
#define _NUMBER_GRID_H_

#include <vector>
#include "PPM.h"
#include "ColorTable.h"

class NumberGrid {
	public:
		//constructors
		NumberGrid();
		NumberGrid(const int& height, const int& width);

		//deconstructors
		virtual ~NumberGrid();

		//methods
		int getHeight() const;
		int getWidth() const;
		int getMaxNumber() const;
		virtual void setGridSize(const int& height, const int& width);
		void setMaxNumber(const int& number);
		const std::vector<int>& getNumbers() const;
		int index(const int& row, const int& column) const;
		bool indexValid(const int& row, const int& column) const;
		bool numberValid(const int& number) const;
		int getNumber(const int& row, const int& column) const;
		void setNumber(const int& row, const int& column, const int& number);
		void setPPM(PPM& ppm) const;
		void setPPM(PPM& ppm, const ColorTable& colors) const;
		virtual int calculateNumber(const int& row, const int& column) const = 0;
		virtual void calculateAllNumbers();

	private:
		//data members
		int mHeight;
		int mWidth;
		int mMaxNumber;
		std::vector<int> mGrid;
};

class ManhattanNumbers: public NumberGrid {
	public:
		//constructors
		ManhattanNumbers();
		ManhattanNumbers(const int& height, const int& width);
		virtual ~ManhattanNumbers();

		//methods
		int calculateNumber(const int& row, const int& column) const;

	private:
		//data members
		//no data members
};

#endif
