#include "Glider.h"

#include <iostream>
#include <string>

int main(){
    //  BUILD THE GLIDER FROM A GLIDERPLAN INPUT FILE
    std::string glider_file_name{"./.References/test_glider.txt"};
    Glider analysis_glider {glider_file_name};

    return 0;
}