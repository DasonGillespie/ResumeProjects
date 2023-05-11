CXX := g++
CXXFLAGS := -std=c++11
XCCFLAGS := $(CXXFLAGS)  -Wall -Wpedantic -Wextra -Werror -g

all: hello questions_3 ascii_image image_file flag_romania ppm_menu

hello: hello.o
	g++ -std=c++11 -o hello hello.o
	
hello.o: hello.cpp
	g++ -std=c++11 -c hello.cpp

questions_3: questions_3.o image_output.o image_drawing.o user_io.o controllers.o Image.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.o ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o
	g++ -std=c++11 -o questions_3 questions_3.o image_output.o image_drawing.o user_io.o controllers.o Image.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.o ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o

questions_3.o: questions_3.cpp controllers.o image_menu.h
	g++ -std=c++11 -c questions_3.cpp

user_io.o: user_io.cpp image_menu.h ActionData.h 
	g++ -std=c++11 -c user_io.cpp

controllers.o: controllers.cpp Image.h PPM.h image_menu.h PPM.o NumberGrid.o NumberGrid.h
	g++ -std=c++11 -c controllers.cpp

ascii_image: ascii_image.o user_io.o controllers.o Image.o image_output.o image_drawing.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.h ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o
	g++ -std=c++11 -o ascii_image ascii_image.o user_io.o controllers.o Image.o image_output.o image_drawing.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.o ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o

ascii_image.o: ascii_image.cpp image_menu.h
	g++ -std=c++11 -c ascii_image.cpp

Image.o: Image.cpp Image.h
	g++ -std=c++11 -c Image.cpp

image_output.o: image_output.cpp image_menu.h Image.h PPM.h PPM.o
	g++ -std=c++11 -c image_output.cpp

image_drawing.o: image_drawing.cpp Image.h PPM.h PPM.o
	g++ -std=c++11 -c image_drawing.cpp

image_file: image_file.o user_io.o controllers.o Image.o image_output.o image_drawing.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.o ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o
	g++ -std=c++11 -o image_file image_file.o user_io.o controllers.o Image.o image_output.o image_drawing.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.o ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o

image_file.o: image_file.cpp image_menu.h Image.h PPM.h
	g++ -std=c++11 -c image_file.cpp

PPM.o: PPM.cpp Image.h PPM.h
	g++ -std=c++11 -c PPM.cpp

flag_romania: flag_romania.o user_io.o controllers.o Image.o image_output.o image_drawing.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.o ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o
	g++ -std=c++11 -o flag_romania flag_romania.o user_io.o controllers.o Image.o image_output.o image_drawing.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.o ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o

flag_romania.o: flag_romania.cpp image_menu.h ActionData.h
	g++ -std=c++11 -c flag_romania.cpp

ActionData.o: ActionData.cpp ActionData.h
	g++ -std=c++11 -c ActionData.cpp

MenuData.o: MenuData.cpp MenuData.h ActionData.h
	g++ -std=c++11 -c MenuData.cpp

ppm_menu: ppm_menu.o user_io.o controllers.o Image.o image_output.o image_drawing.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.o ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o
	g++ -std=c++11 -o ppm_menu ppm_menu.o user_io.o controllers.o Image.o image_output.o image_drawing.o PPM.o ActionData.o MenuData.o image_filters.o NumberGrid.o ColorTable.o ComplexFractal.o JuliaSet.o MandelbrotSet.o ThreadedGrid.o

ppm_menu.o: ppm_menu.cpp ActionData.o MenuData.o 
	g++ -std=c++11 -c ppm_menu.cpp

image_filters.o: image_filters.cpp image_menu.h PPM.h ActionData.h MenuData.h
	g++ -std=c++11 -c image_filters.cpp

NumberGrid.o: NumberGrid.cpp ActionData.o NumberGrid.h ComplexFractal.h
	g++ -std=c++11 -c NumberGrid.cpp

ColorTable.o: ColorTable.cpp NumberGrid.h ColorTable.h
	g++ -std=c++11 -c ColorTable.cpp

ComplexFractal.o: ComplexFractal.cpp NumberGrid.h ColorTable.h
	g++ -std=c++11 -c ComplexFractal.cpp

JuliaSet.o: JuliaSet.cpp ComplexFractal.h
	g++ -std=c++11 -c JuliaSet.cpp

MandelbrotSet.o: MandelbrotSet.cpp ComplexFractal.h
	g++ -std=c++11 -c MandelbrotSet.cpp

ThreadedGrid.o: ThreadedGrid.cpp NumberGrid.h
	g++ -std=c++11 -c ThreadedGrid.cpp

clean:
	-rm -f hello
	-rm -f questions_3
	-rm -f ascii_image
	-rm -f image_file
	-rm -f flag_romania
	-rm -f ppm_menu
	-rm -f *.o
	-rm -f *~
	-rm -f *.ppm
