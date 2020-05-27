#include "Glider.h" 

// Constructor
Glider::Glider(std::string glider_file_name) {
    // Name the Glider
    name = glider_file_name;
    
    //Open, read, and copy the Gliderplan File - Algorithm accounts for Gliderplan's standard output format
    std::ifstream in_file;
    in_file.open(name);
    if (!in_file) {
        std::cerr << "Problem opening glider file" << std::endl;
    }
    else{
        std::string line{};
        while (std::getline(in_file, line)) {
            std::istringstream test_stream {line};
            double test_dbl {};
            // TODO: Need to modify this inner loop so that it can compile the individual coordinate vectors into a cell, then into canopy etc.
            if(test_stream >> test_dbl){
                double temp_x_coord {};
                double temp_y_coord {};
                double temp_z_coord {};
                std::istringstream line_stream {line};
                std::vector<double> point_coords;
                std::vector<std::vector<double>> airfoil_coords; //! Due to scope, this is only valid once a loop, see above

                line_stream >> temp_y_coord >> temp_z_coord >> temp_x_coord;
                point_coords.push_back(temp_x_coord);
                point_coords.push_back(temp_y_coord);
                point_coords.push_back(temp_z_coord);
                airfoil_coords.push_back(point_coords);

                std::cout << temp_x_coord << temp_y_coord << temp_z_coord << std::endl;
            }else{
                std::cout << line << std::endl; 
            }
        }
    }
    in_file.close(); //This is throwing such an annoying intellisense error on VSCODE...
}

// Destructor
Glider::~Glider(){
}

// Get Methods

// Set Methods

// Other Methods