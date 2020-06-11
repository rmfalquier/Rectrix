#include "Canopy.h" //Don't forget the quotes here

// CONSTRUCTOR
Canopy::Canopy(std::vector<Cell> canopy_cells) 
    : cells{canopy_cells}{
}

// DESTRUCTOR
Canopy::~Canopy(){
}

// GET METHODS
std::vector<Cell> Canopy::get_cells(){
    return cells;
}

// SET METHODS


// OTHER METHODS