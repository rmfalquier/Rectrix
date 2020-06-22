//------------------------------------------------------------------------------------//
// GALLERY CLASS
// A:
//  -Name: 
//------------------------------------------------------------------------------------//

#ifndef _GALLERY_H_
#define _GALLERY_H_

#include "Pgline.h"

#include <iostream>
#include <vector>

// #include <armadillo>

class Gallery {
private:
    // IDENTIFIERS

    // COMPONENTS
    std::vector<Pgline> lines;
public:
    // CONSTRUCTOR
    Gallery(std::vector<Pgline> gallery_lines);

    // DESTRUCTOR
    ~Gallery();

    // GET METHODS
    std::vector<Pgline> Get_Lines() const;

    // SET METHODS 


    // OTHER METHODS


};

#endif