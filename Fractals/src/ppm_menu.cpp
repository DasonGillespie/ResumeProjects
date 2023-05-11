#include "image_menu.h"
#include <ctime>
#include <cstdlib>

int main() {
	std::srand(std::time(0));
	return imageMenu(std::cin, std::cout);
}
