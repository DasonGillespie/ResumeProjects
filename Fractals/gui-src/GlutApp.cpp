#include "GlutApp.h"
#include "glut_app.h"
#include "image_menu.h"

//-------------------------------------
GlutApp::GlutApp(int height, int width)
  : mHeight(height), mWidth(width), mActionData(mInputStream, mOutputStream), mMinX(-2.0), mMaxX(2.0), mMinY(-2.0), mMaxY(2.0), mA(-1.223), mB(0.262), mInteractionMode(IM_FRACTAL), mFractalMode(M_MANDELBROT), mMaxNumber(200), mColor1(84, 1, 63), mColor2(255, 255, 255), mColor3(10, 10, 10), mNumColor(32), mAntiAliasReductionCount(2), mAntiAlias(false) {

  configureMenu(mMenuData);
  mActionData.setGrid(new ComplexFractal);
  setColorTable();
  createFractal();

}

//--------------------------------------------
void GlutApp::setSize(int height, int width) {
  mHeight = height;
  mWidth = width;
}

//------------------------------
int GlutApp::getHeight() const {
  return mHeight;
}

//-----------------------------
int GlutApp::getWidth() const {
  return mWidth;
}

InteractionMode GlutApp::getInteractionMode() const {
	return mInteractionMode;
}

FractalMode GlutApp::getFractalMode() const {
	return mFractalMode;
}

//-----------------------
void GlutApp::display() {
  if(mInteractionMode == IM_FRACTAL) {
	  PPM& p = mActionData.getOutputImage();
	  double max = static_cast<double>(p.getMaxColorValue());
	  double r, g, b;
	  int row, column;
	  glBegin( GL_POINTS );
	  for(row = 0; row < p.getHeight(); row++) {
	    for(column = 0; column < p.getWidth(); column++) {
	      r = p.getChannel(row, column, 0) / max;
	      g = p.getChannel(row, column, 1) / max;
	      b = p.getChannel(row, column, 2) / max;
	      glColor3d(r, g, b);
	      glVertex2i(column, p.getHeight()-row-1);
	    }
	  }
	  glEnd( );
  } else if(mInteractionMode == IM_COLORTABLE) {
  	displayColorTable();
  } else if(mInteractionMode == IM_COLOR1) {
  	displayColorTable();
  } else if(mInteractionMode == IM_COLOR2) {
  	displayColorTable();
  } else if(mInteractionMode == IM_COLOR3) {
  	displayColorTable();
  }
}

//---------------------------
void GlutApp::createJulia() {

  selectJulia();

  configureGrid(200);

  juliaParameters(0.45, -0.32);

  fractalPlaneSize(-2.0, 2.0, -2.0, 2.0);
   
  fractalCalculate();

  gridApplyColorTable();
}

//---------------------------
void GlutApp::selectJulia() {
  mOutputStream.clear();
  mOutputStream.str("");
  mInputStream.clear();
  mInputStream.str("");

  takeAction("julia", mMenuData, mActionData);
}

//--------------------------------
void GlutApp::selectMandelbrot() {
  mOutputStream.clear();
  mOutputStream.str("");
  mInputStream.clear();
  mInputStream.str("");

  takeAction("mandelbrot", mMenuData, mActionData);
}

//------------------------------------
void GlutApp::selectComplexFractal() {
  mOutputStream.clear();
  mOutputStream.str("");
  mInputStream.clear();
  mInputStream.str("");

  takeAction("complex-fractal", mMenuData, mActionData);
}

//------------------------------------
void GlutApp::configureGrid(int max) {
  mOutputStream.clear();
  mInputStream.clear();
  mOutputStream.str("");
  mInputStream.str("");
  int height;
  int width;
  if(mAntiAlias == true) {
  	height = mHeight * mAntiAliasReductionCount;
  	width = mWidth * mAntiAliasReductionCount;
  } else {
  	height = mHeight;
	width = mWidth;
  }
  {
    std::stringstream tmp;
    tmp << height << " " << width << " " << max;
    mInputStream.str(tmp.str());
  }
  takeAction("grid", mMenuData, mActionData);
}

//-------------------------------------------------
void GlutApp::juliaParameters(double a, double b) {
  mOutputStream.clear();
  mInputStream.clear();
  mOutputStream.str("");
  mInputStream.str("");
  {
    std::stringstream tmp;
    tmp << a << " " << b;
    mInputStream.str(tmp.str());
  }
  takeAction("julia-parameters", mMenuData, mActionData);
}

//--------------------------------------------------------------------------------------
void GlutApp::fractalPlaneSize(double x_min, double x_max, double y_min, double y_max) {
  mOutputStream.clear();
  mInputStream.clear();
  mOutputStream.str("");
  mInputStream.str("");
  {
    std::stringstream tmp;
    tmp << x_min << " " << x_max << " "<< y_min << " " << y_max;
    mInputStream.str(tmp.str());
  }
  takeAction("fractal-plane-size", mMenuData, mActionData);
}

//--------------------------------
void GlutApp::fractalCalculate() {
  mOutputStream.clear();
  mInputStream.clear();
  mOutputStream.str("");
  mInputStream.str("");
  takeAction("fractal-calculate", mMenuData, mActionData);
}

//-----------------------------------
void GlutApp::gridApplyColorTable() {
  mOutputStream.clear();
  mInputStream.clear();
  mOutputStream.str("");
  mInputStream.str("");
  takeAction("grid-apply-color-table", mMenuData, mActionData);
  applyAntiAlias();
}

//----------------------------
void GlutApp::createJulia2() {

  selectJulia();

  configureGrid(200);

  juliaParameters(-1.2, -0.32);

  fractalPlaneSize(-1.7, 1.7, -1.5, 1.5);
   
  fractalCalculate();

  gridApplyColorTable();
}

//--------------------------------
void GlutApp::createMandelbrot() {

  selectMandelbrot();

  configureGrid(200);

  fractalPlaneSize(-2.0, 2.0, -2.0, 2.0);
   
  fractalCalculate();

  gridApplyColorTable();
}

//---------------------------------
void GlutApp::createMandelbrot2() {
  selectMandelbrot();

  configureGrid(200);

  fractalPlaneSize(-0.13, -0.084, 0.91, 0.94);
   
  fractalCalculate();

  gridApplyColorTable();
}

//------------------------------------
void GlutApp::createComplexFractal() {
  selectComplexFractal();

  configureGrid(200);

  fractalPlaneSize(-2.0, 2.0, -2.0, 2.0);
   
  fractalCalculate();

  gridApplyColorTable();
}

//-------------------------------------
void GlutApp::createComplexFractal2() {
  selectComplexFractal();

  configureGrid(200);

  fractalPlaneSize(1.575, 1.875, 1.75, 2.0);
   
  fractalCalculate();

  gridApplyColorTable();
}

//-----------------------------------
void GlutApp::displayColorTable() {
  configureGrid(200);
  PPM& p = mActionData.getOutputImage();
  glBegin( GL_POINTS );
  ColorTable& colortable = mActionData.getTable();
  Color color = colortable[0];
  for(int row = 0; row < p.getHeight(); row++) {
    for(int column = 0; column < p.getWidth(); column++) {//mActionData.getTable().getNumberOfColors()
      int i = column * mNumColor / p.getWidth();
	  color = colortable[i];
      double r = color.getRed() / 255.0;
      double g = color.getGreen() / 255.0;
      double b = color.getBlue() / 255.0;
      glColor3d(r, g, b);
      glVertex2i(column, row);
    }
 }
  glEnd();
}

//	  PPM& p = mActionData.getOutputImage();
//	  double max = static_cast<double>(p.getMaxColorValue());
//	  double r, g, b;
//	  int row, column;
//	  glBegin( GL_POINTS );
//	  for(row = 0; row < p.getHeight(); row++) {
//	    for(column = 0; column < p.getWidth(); column++) {
//	      r = p.getChannel(row, column, 0) / max;
//	      g = p.getChannel(row, column, 1) / max;
//	      b = p.getChannel(row, column, 2) / max;
//	      glColor3d(r, g, b);
//	      glVertex2i(column, p.getHeight()-row-1);
//	    }
//	  }
//	  glEnd( );



//------------------------------------------------------
void GlutApp::setInteractionMode(InteractionMode mode) {
	mInteractionMode = mode;
}

//-----------------------------
void GlutApp::setColorTable() {
  mOutputStream.clear();
  mInputStream.clear();
  mOutputStream.str("");
  mInputStream.str("");
  {
	std::stringstream tmp;
	tmp << mNumColor;
	mInputStream.str(tmp.str());
  }
  takeAction("set-color-table-size", mMenuData, mActionData);

  //color1 to color2
  mOutputStream.clear();
  mInputStream.clear();
  mOutputStream.str("");
  mInputStream.str("");
  {
	std::stringstream tmp;
	tmp << 0 << " " << mColor1.getRed() << " " << mColor1.getGreen() << " " << mColor1.getBlue() << " " << mNumColor/2 << " " << mColor2.getRed() << " " << mColor2.getGreen() << " " << mColor2.getBlue();
	mInputStream.str(tmp.str());
  }
  takeAction("set-color-gradient", mMenuData, mActionData);

  //color2 to color3
  mOutputStream.clear();
  mInputStream.clear();
  mOutputStream.str("");
  mInputStream.str("");
  {
	std::stringstream tmp;
	tmp << mNumColor/2 << " " << mColor2.getRed() << " " << mColor2.getGreen() << " " << mColor2.getBlue() << " " << mNumColor - 1 << " " << mColor3.getRed() << " " << mColor3.getGreen() << " " << mColor3.getBlue();
	mInputStream.str(tmp.str());
  }
  takeAction("set-color-gradient", mMenuData, mActionData);
}

//--------------------------------------
void GlutApp::decreaseColorTableSize() {
	if(mNumColor > 10) {
		mNumColor = mNumColor / 1.1;
		setColorTable();
		gridApplyColorTable();
	}
}

//--------------------------------------
void GlutApp::increaseColorTableSize() {
	if(mNumColor < 1024) {
		mNumColor = mNumColor * 1.1;
		setColorTable();
		gridApplyColorTable();
	}
}

//----------------------------
void GlutApp::randomColor1() {
	std::srand(std::time(0));
	int red = std::rand() % 256;
	int green = std::rand() % 256;
	int blue = std::rand() % 256;

	mColor1.setRed(red);
	mColor1.setGreen(green);
	mColor1.setBlue(blue);

	setColorTable();
	gridApplyColorTable();
}

//----------------------------
void GlutApp::randomColor2() {
	std::srand(std::time(0));
	int red = std::rand() % 256;
	int green = std::rand() % 256;
	int blue = std::rand() % 256;

	mColor2.setRed(red);
	mColor2.setGreen(green);
	mColor2.setBlue(blue);

	setColorTable();
	gridApplyColorTable();

}

//----------------------
void GlutApp::zoomIn() {
	double dx = (1.0 - 0.9) * (mMaxX - mMinX) / 2.0;
	double dy = (1.0 - 0.9) * (mMaxY - mMinY) / 2.0;
	mMinX += dx;
	mMaxX -= dx;
	mMinY += dy;
	mMaxY -= dy;
}

//-----------------------
void GlutApp::zoomOut() {
	double dx = (1.0 - 0.9) * (mMaxX - mMinX) / 2.0;
	double dy = (1.0 - 0.9) * (mMaxY - mMinY) / 2.0;
	double tempMinX = mMinX - dx;
	double tempMaxX = mMaxX + dx;
	double tempMinY = mMinY - dy;
	double tempMaxY = mMaxY + dy;
	if(tempMinX >= -2.0 && tempMinX <= 2.0 && tempMaxX >= -2.0 && tempMaxX <= 2.0 && tempMinY >= -2.0 && tempMinY <= 2.0 && tempMaxY >=-2.0 && tempMaxY <= 2.0) {
		mMinX -= dx;
		mMaxX += dx;
		mMinY -= dy;
		mMaxY += dy;
	}
}

//------------------------
void GlutApp::moveLeft() {
	double dx = (1.0 - 0.9) * (mMaxX - mMinX) / 2.0;
	double tempX = mMinX - dx;
	if(tempX >= -2.0) {
		mMinX -= dx;
		mMaxX -= dx;
	}
}

//-------------------------
void GlutApp::moveRight() {
	double dx = (1.0 - 0.9) * (mMaxX - mMinX) / 2.0;
	double tempX = mMaxX + dx;
	if(tempX <= 2.0) {
		mMinX += dx;
		mMaxX += dx;
	}
}

//------------------------
void GlutApp::moveDown() {
	double dy = (1.0 - 0.9) * (mMaxY - mMinY) / 2.0;
	double tempY = mMinY - dy;
	if(tempY >= -2.0) {
		mMinY -= dy;
		mMaxY -= dy;
	}
}

//----------------------
void GlutApp::moveUp() {
	double dy = (1.0 - 0.9) * (mMaxY - mMinY) / 2.0;
	double tempY = mMaxY + dy;
	if(tempY <= 2.0) {
		mMinY += dy;
		mMaxY += dy;
	}
}

//----------------------------------------------
void GlutApp::setFractalMode(FractalMode mode) {
	setInteractionMode(IM_FRACTAL);
	mFractalMode = mode;
}

//---------------------------------
void GlutApp::increaseMaxNumber() {
	if(mMaxNumber < 2048) {
		mMaxNumber *= 1.1;
	}
}

//---------------------------------
void GlutApp::decreaseMaxNumber() {
	if(mMaxNumber > 11) {
		mMaxNumber /= 1.1;
	}
}

//-----------------------------------
void GlutApp::setAB(int x, int y) {
	if(mFractalMode == M_MANDELBROT) {
		NumberGrid *ptr = &mActionData.getGrid();
		ComplexFractal *CF = dynamic_cast<ComplexFractal *>(ptr);
		if(CF != 0) {
			double delta_x = CF->getDeltaX();
			double delta_y = CF->getDeltaY();
			mA = mMinX + x * delta_x;
			mB = mMinY + y * delta_y;
		}
	}
}

//--------------------------
void GlutApp::resetPlane() {
	mMinX = -2.0;
	mMaxX = 2.0;
	mMinY = -2.0;
	mMaxY = 2.0;
}

//-----------------------------
void GlutApp::createFractal() {
	if(mFractalMode == M_JULIA) {
	    selectJulia();
		configureGrid(mMaxNumber);
		juliaParameters(mA, mB);
		fractalPlaneSize(mMinX, mMaxX, mMinY, mMaxY);
		fractalCalculate();
		gridApplyColorTable();
	} else if(mFractalMode == M_MANDELBROT) {
	    selectMandelbrot();
		configureGrid(mMaxNumber);
		fractalPlaneSize(mMinX, mMaxX, mMinY, mMaxY);
		fractalCalculate();
		gridApplyColorTable();
	} else if(mFractalMode == M_COMPLEX) {
	    selectComplexFractal();
		configureGrid(mMaxNumber);
		fractalPlaneSize(mMinX, mMaxX, mMinY, mMaxY);
		fractalCalculate();
		gridApplyColorTable();
	}
}

void GlutApp::increaseChannel(Color& color, int channel) {
	int new_value;
	switch(channel) {
		case 0:
				new_value = color.getRed() + 10;
				if(new_value > 255) {
					new_value = 255;
				}
				color.setRed(new_value);
				break;
		case 1:
				new_value = color.getGreen() + 10;
				if(new_value > 255) {
					new_value = 255;
				}
				color.setGreen(new_value);
				break;
		case 2:
				new_value = color.getBlue() + 10;
				if(new_value > 255) {
					new_value = 255;
				}
				color.setBlue(new_value);
				break;
		default:
				break;
	}
	setColorTable();
	gridApplyColorTable();
}

void GlutApp::decreaseChannel(Color& color, int channel) {
	int new_value;
	switch(channel) {
		case 0:
				new_value = color.getRed() - 10;
				if(new_value < 0) {
					new_value = 0;
				}
				color.setRed(new_value);
				break;
		case 1:
				new_value = color.getGreen() - 10;
				if(new_value < 0) {
					new_value = 0;
				}
				color.setGreen(new_value);
				break;
		case 2:
				new_value = color.getBlue() - 10;
				if(new_value < 0) {
					new_value = 0;
				}
				color.setBlue(new_value);
				break;
		default:
				break;
	}
	setColorTable();
	gridApplyColorTable();

}

Color& GlutApp::fetchColor() {
	switch(mInteractionMode) {
		case IM_COLOR1:
				return mColor1;
		case IM_COLOR2:
				return mColor2;
		case IM_COLOR3:
				return mColor3;
		default:
				static Color ec(-1, -1, -1);
				static Color c(-1, -1, -1);
				c = ec;
				return c;
	}
}

void GlutApp::increaseRed() {
	increaseChannel(fetchColor(), 0);
}

void GlutApp::decreaseRed() {
	decreaseChannel(fetchColor(), 0);
}

void GlutApp::increaseGreen() {
	increaseChannel(fetchColor(), 1);
}

void GlutApp::decreaseGreen() {
	decreaseChannel(fetchColor(), 1);
}

void GlutApp::increaseBlue() {
	increaseChannel(fetchColor(), 2);
}

void GlutApp::decreaseBlue() {
	decreaseChannel(fetchColor(), 2);
}

void GlutApp::copyOutputToInput1() {
	takeAction("copyo1", mMenuData, mActionData);
}

void GlutApp::antiAlias(int reduction_count) {
	mOutputStream.clear();
  	mOutputStream.str("");
  	mInputStream.clear();
  	mInputStream.str("");
  {
    std::stringstream tmp;
    tmp << reduction_count;
    mInputStream.str(tmp.str());
  }

  	takeAction("anti-alias", mMenuData, mActionData);
}

void GlutApp::applyAntiAlias() {
	if(mAntiAlias == true) {
		copyOutputToInput1();
		antiAlias(mAntiAliasReductionCount);
	}
}

void GlutApp::toggleAntiAlias() {
	if(mAntiAlias == true) {
		mAntiAlias = false;
	} else {
		mAntiAlias = true;
	}
}

void GlutApp::increaseAntiAliasReductionCount() {
	mAntiAliasReductionCount += 1;
}

void GlutApp::decreaseAntiAliasReductionCount() {
	mAntiAliasReductionCount -= 1;
	if(mAntiAliasReductionCount < 2) {
		mAntiAliasReductionCount = 2;
	}
}

