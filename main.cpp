#include "Glider.h"

#include <iostream>
#include <string>

#include <armadillo>

// TODO: Overload << operator to display glider data
// TODO: Comments!

int main(){
    // BUILD THE GLIDER FROM A GLIDERPLAN INPUT FILE
    std::string glider_file_name{"./.References/test_glider.txt"};
    Glider analysis_glider {glider_file_name};

    // OUTPUT FILES FOR GNUPLOT
    analysis_glider.GNU_Print();

    return 0;
}