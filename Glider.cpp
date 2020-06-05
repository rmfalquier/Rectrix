#include "Glider.h" 

// Constructor
Glider::Glider(std::string glider_file_name) {
    // NAME THE GLIDER
    name = glider_file_name;
    
    // OPEN AND READ GLIDERPLAN FILE TO INITIALIZE GLIDER DATA STRUCTURE IN GIVEN FORMAT
    std::ifstream in_file;
    in_file.open(name);
    if (!in_file) {
        std::cerr << "Problem opening glider file" << std::endl;
    }
    else{
        // TEMPORARY STORAGE AND DIAGNOSTIC VARIABLES FOR FILE READING
        std::string line{};
        int line_counter {1};

        // BOOLEAN GOVERNORS FOR READ CASES
        bool new_cell {false};
        bool bridle_found {false};
        bool profile_section {true};

        // CANOPY SUB-CLASS RELATED VECTORS AND COUNTERS
        int profile_number {1}; // Gliderplan starts counting at 1
        std::vector<std::vector<double>> airfoil_coords;
        std::vector<Cell> canopy_cells;

        // GLIDERPLAN FILE READING LOOP
        // TODO: Need to add a bridle reading section here that reads line data into a single line and adds to container for line gallery
        // TODO: There's for sure a cleaner way to implement the logic on this loop, especially with regard to rotation for the center cell. 
        while (std::getline(in_file, line)) {
            // CHECK FOR DIFFERENT FILE SECTIONS AND FLAG THEM WITH BOOLEAN GOVERNORS TO
            // READ DATA BASED ON THE CURRENTLY FLAGGED GOVERNORS 
            if(line.find("profile") != std::string::npos){
                new_cell = true;
                
                // CREATE NEW CELL WITH AIRFOIL PROFILE COORDINATES AND ADD TO CELL VECTOR
                if(!airfoil_coords.empty()){
                    Cell temp_cell {profile_number, airfoil_coords};
                    canopy_cells.push_back(temp_cell);
                    ++profile_number;
                }
            }else if(line.find("bridle") != std::string::npos){
                bridle_found = true;
                profile_section = false;

                // CREATE LAST CELL WITH AIRFOIL PROFILE COORDINATES AND ADD TO CELL VECTOR
                Cell temp_cell {profile_number, airfoil_coords};
                canopy_cells.push_back(temp_cell);

            }else if(profile_section && !line.empty()){
                if(new_cell){
                    airfoil_coords.clear();
                    new_cell = false;
                }
                // CREATE TEMPORARY STORAGE FOR COORDINATES
                double temp_x_coord {};
                double temp_y_coord {};
                double temp_z_coord {};
                std::istringstream coord_stream {line};
                std::vector<double> point_coords;

                // READ COORDINATES FROM STREAM, PUSH TO VECTOR IN DESIRED ORDER, ADD TO AIRFOIL VECTOR
                coord_stream >> temp_y_coord >> temp_z_coord >> temp_x_coord;
                point_coords.push_back(temp_x_coord);
                point_coords.push_back(temp_y_coord);
                point_coords.push_back(temp_z_coord);
                airfoil_coords.push_back(point_coords);

            }else if (line.empty()){
                std::cout << "The following line number is empty: " << line_counter << std::endl; 
            }
            
            // DIAGNOSTICS UPDATES
            ++line_counter;
        }
        
        // INITIALIZE AND LINK SUB-CLASSES
        // TODO: Initialize Canopy -> Probably has to be done with a pointer
        canopy_ptr = std::make_unique<Canopy>(canopy_cells);
        // TODO: Initialize Line Gallery
        // TODO: Initialize Payload
    }
    in_file.close(); //This someimtes throws a super annoying intellisense error on VSCODE...
}

// DESTRUCTOR
Glider::~Glider(){
}

// GET METHODS

// SET METHODS

// OTHER METHODS
// TODO: Print data to a file, start early to perform comparisons / diagnostics