//------------------------------------------------------------------------------------//
// CANOPY CLASS
// A:
//  -Name: 
//------------------------------------------------------------------------------------//

#ifndef _CANOPY_H_
#define _CANOPY_H_

#include "Cell.h"

#include <vector>

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


    // SET METHODS 


    // OTHER METHODS


};

#endif