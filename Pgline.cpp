#include "Pgline.h"

// CONSTRUCTOR
Pgline::Pgline(std::string line_id, arma::mat line_coords, std::string line_material)
    : id{line_id}, coords{line_coords}, material{line_material}{
    }

// DESTRUCTOR
Pgline::~Pgline(){}

// GET METHODS
arma::mat Pgline::get_coords(){
    return coords;
}

// SET METHODS


// OTHER METHODS

