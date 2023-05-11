#ifndef _ACTION_DATA_
#define _ACTION_DATA_

#include <iostream>
#include "PPM.h"
#include "NumberGrid.h"

class ActionData {
	public:
		//constructor
		ActionData(std::istream& is, std::ostream& os);

		//deconstructor
		~ActionData();

		//methods
		std::istream& getIS();
		std::ostream& getOS();
		PPM& getInputImage1();
		PPM& getInputImage2();
		PPM& getOutputImage();
		bool getDone() const;
		ColorTable& getTable();
		void setDone();
		NumberGrid& getGrid();
		void setGrid(NumberGrid *grid);

	private:
		//data members
		std::istream& mIS;
		std::ostream& mOS;
		PPM mInputImage1;
		PPM mInputImage2;
		PPM mOutputImage;
		bool mDone;
		NumberGrid *mNumGrid;
		ColorTable mColorTable;

};

#endif
