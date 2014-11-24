// standard C++ libraries
#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Viewer.h"

int main(int argc, char *argv[]) {
    try {
		Viewer* viewer = new Viewer();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}