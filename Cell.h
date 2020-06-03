//------------------------------------------------------------------------------------//
// CELL CLASS
// A:
//  -Name: 
//------------------------------------------------------------------------------------//

#ifndef _Cell_H_
#define _Cell_H_

#include <iostream>
#include <vector>

class Cell {
private:
    // IDENTIFIERS
    int cell_number;

    // COMPONENTS
    std::vector<std::vector<double>> coords; //Vector of XYZ-Coordinate Vectors describing the Cell
    // double ang_inc;
    // double ang_sweep;
    // std::vector<double> pos;
    // double local_cg;
    // double local_c;
public:
    // CONSTRUCTOR
    Cell(int profile_number, std::vector<std::vector<double>> airfoil_coords);

    // DESTRUCTOR
    ~Cell();

    // GET METHODS


    // SET METHODS 


    // OTHER METHODS


};

#endif