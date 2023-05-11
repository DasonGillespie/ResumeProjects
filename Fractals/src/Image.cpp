#include "Image.h"
#include "image_menu.h"

Image::Image()
	: mHeight(), mWidth() {
}

Image::Image(const int& height, const int& width)
	: mHeight(height), mWidth(width) {
		mBox.resize(mHeight * mWidth * 3);
	}

int Image::getHeight() const {
	return mHeight;
}

int Image::getWidth() const {
	return mWidth;
}

bool Image::indexValid(const int& row, const int& column, const int& channel) const {
	if((row < mHeight) && (column < mWidth) && (row >= 0) && (column >= 0) && (channel >= 0) && (channel <= 2)) {
        	return true;
    	}
    	return false;
}

int Image::index(const int& row, const int& column, const int& channel) const {
	int index = (row * mWidth * 3 + column * 3) + channel;
	return index;
}

int Image::getChannel(const int& row, const int& column, const int& channel) const {
	int i = index(row, column, channel);
	if(indexValid(row, column, channel) == false) {
		return -1;
	} else {
		return mBox[i];
	}
}

void Image::setHeight(const int& height) {
	if(height >= 0) {
        	mHeight = height;
        	mBox.resize(mWidth * 3 * mHeight);
    }
}

void Image::setWidth(const int& width) {
	if(width >= 0) {
        	mWidth = width;
        	mBox.resize(mWidth * 3 * mHeight);
    }
}

void Image::setChannel(const int& row, const int& column, const int& channel, const int& value) {
    	if(indexValid(row, column, channel)) {
		int i = index(row, column, channel);
        	mBox[i] = value;
    }
}
