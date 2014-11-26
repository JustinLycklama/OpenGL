// standard C++ libraries
#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Viewer.h"


int main(int argc, char *argv[]) {
    try {
		Viewer* viewer = new Viewer();
    } catch (const std::exception& e){
        std::cerr << "\n\nERROR: " << e.what() << std::endl;
		getchar();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}