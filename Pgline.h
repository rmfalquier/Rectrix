//------------------------------------------------------------------------------------//
// LINE CLASS
// A:
//  -Name: 
//------------------------------------------------------------------------------------//

#ifndef _PGLINE_H_
#define _PGLINE_H_

#include <iostream>
#include <string>

#include <armadillo>

class Pgline {
private:
    // IDENTFIERS
    std::string id;

    // COMPONENTS
    arma::mat coords; //XYZ-Coordinate Matrix for line coordinates - x, y, and z coordinates each in a row - start point on first column, end point on second
    std::string material;
public:
    // CONSTRUCTOR
    Pgline(std::string line_id, arma::mat line_coords, std::string line_material);

    // DESTRUCTOR
    ~Pgline();

    // GET METHODS
    arma::mat get_coords();

    // SET METHODS 


    // OTHER METHODS


};

#endif