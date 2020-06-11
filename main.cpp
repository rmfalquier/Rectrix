#include "Glider.h"

#include <iostream>
#include <string>

#include <armadillo>

int main(){
    // BUILD THE GLIDER FROM A GLIDERPLAN INPUT FILE
    std::string glider_file_name{"./.References/test_glider.txt"};
    Glider analysis_glider {glider_file_name};
    analysis_glider.gnu_print();

    return 0;
}