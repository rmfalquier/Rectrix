#include "Canopy.h" //Don't forget the quotes here

// CONSTRUCTOR
Canopy::Canopy(std::vector<Cell> canopy_cells) 
    : cells{canopy_cells}{
    std::cout << "Apparent Success in creation of canopy with " << cells.size() << " cells." << std::endl; 
}

// DESTRUCTOR
Canopy::~Canopy(){
}

// GET METHODS


// SET METHODS


// OTHER METHODS