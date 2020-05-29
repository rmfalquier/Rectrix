//------------------------------------------------------------------------------------//
// GLIDER CLASS
// A Glider class is a data structure comprised of a canopy, line gallery, and payload,
// which are three separate data structures. It is constructed by supplying the name of 
// the data file that (in string format) that contains the GliderPlan output *.txt file 
// with the 3D data for the glider. The class' constructor will extract it and calculate 
// all relevant data for flight mechanics analytics. The class includes the following 
// key data about the glider:
//  -Name: 
//------------------------------------------------------------------------------------//


#ifndef _GLIDER_H_
#define _GLIDER_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <string>
#include <vector>

class Glider {
private:
    // IDENTIFIERS
    std::string name;

    // COMPONENTS
public:
// CONSTRUCTOR
Glider(std::string glider_file_name);
// DESTRUCTOR
~Glider();

// GET METHODS

// SET METHODS 

// OTHER METHODS
};

#endif