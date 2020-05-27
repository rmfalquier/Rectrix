#include "Glider.h"

#include <iostream>
#include <string>

int main(){
    // ! NEED TO COMMENT ALL MY SHIT
    // TODO: 1) I need to understand what reference frame I am reading the file in
    // TODO: 2) Build the glider by reading the file
    // * BUILD THE GLIDER FROM A GLIDERPLAN INPUT FILE
    std::string glider_file_name{"./.References/test_glider.txt"};
    Glider analysis_glider {glider_file_name};

    return 0;
}