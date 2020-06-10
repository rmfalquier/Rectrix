//------------------------------------------------------------------------------------//
// CELL CLASS
// A:
//  -Name: 
//------------------------------------------------------------------------------------//

#ifndef _Cell_H_
#define _Cell_H_

#include <iostream>
#include <vector>

#include <armadillo>

class Cell {
private:
    // IDENTIFIERS
    int cell_number;

    // COMPONENTS
    arma::mat coords; //XYZ-Coordinate Matrix describing the Cell where x, y, and z coordinates are each in a row
    // double ang_inc;
    // double ang_sweep;
    // std::vector<double> pos;
    // double local_cg;
    // double local_c;
public:
    // CONSTRUCTOR
    Cell(int profile_number, arma::mat airfoil_coords);

    // DESTRUCTOR
    ~Cell();

    // GET METHODS


    // SET METHODS 


    // OTHER METHODS


};

#endif