#include "Gallery.h"

// CONSTRUCTOR
Gallery::Gallery(std::vector<Pgline> gallery_lines)
    : lines{gallery_lines} {
    }

// DESTRUCTOR
Gallery::~Gallery(){
}

// GET METHODS
std::vector<Pgline> Gallery::Get_Lines() const {
    return lines;
}

// SET METHODS


// OTHER METHODS

