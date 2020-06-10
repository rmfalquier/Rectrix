#include "Cell.h" 

// CONSTRUCTOR
Cell::Cell(int profile_number, arma::mat airfoil_coords)
    : cell_number{profile_number}, coords{airfoil_coords} {
    }

// DESTRUCTOR
Cell::~Cell(){
}

// GET METHODS

// SET METHODS 

// OTHER METHODS