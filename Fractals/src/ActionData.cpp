#include "ActionData.h"
#include "ColorTable.h"

ActionData::ActionData(std::istream& is, std::ostream& os) 
	: mIS(is), mOS(os), mDone(false), mNumGrid(0), mColorTable(16) {
	for(int i = 0; i < 16; i++) {
		mColorTable[i] = Color(0, 0, 0);
	}
	Color color1(0, 255, 0);
	Color color2(255, 0, 255);

	mColorTable.insertGradient(color1, color2, 0, mColorTable.getNumberOfColors() - 1); 

	}
ActionData::~ActionData() {
	if(mNumGrid != 0) {
		delete mNumGrid;
	}
}

std::istream& ActionData::getIS() {
	return mIS;
}

std::ostream& ActionData::getOS() {
	return mOS;
}

PPM& ActionData::getInputImage1() {
	return mInputImage1;
}

PPM& ActionData::getInputImage2() {
	return mInputImage2;
}

PPM& ActionData::getOutputImage() {
	return mOutputImage;
}

bool ActionData::getDone() const {
	return mDone;
}

ColorTable& ActionData::getTable() {
return mColorTable;
}

void ActionData::setDone() {
	mDone = true;
}

NumberGrid& ActionData::getGrid() {
	return *mNumGrid;
}

void ActionData::setGrid(NumberGrid *grid) {
	if(mNumGrid != 0) {
		delete mNumGrid;
	}
	mNumGrid = grid;
}
