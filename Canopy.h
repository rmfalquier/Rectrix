//------------------------------------------------------------------------------------//
// CANOPY CLASS
// A:
//  -Name: 
//------------------------------------------------------------------------------------//

#ifndef _CANOPY_H_
#define _CANOPY_H_

#include "Cell.h"

#include <vector>

#include <armadillo>

class Canopy {
private:
    // IDENTIFIERS

    // COMPONENTS
    std::vector<Cell> cells;
public:
    // CONSTRUCTORS
    Canopy(std::vector<Cell> canopy_cells);

    // DESTRUCTOR
    ~Canopy();

    // GET METHODS
    std::vector<Cell> get_cells();

    // SET METHODS 


    // OTHER METHODS


};

#endif