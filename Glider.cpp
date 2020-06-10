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
        arma::mat airfoil_coords(3,0);
        std::vector<Cell> canopy_cells;

        // GLIDERPLAN FILE READING LOOP:
        // CHECK FOR DIFFERENT FILE SECTIONS AND FLAG THEM WITH BOOLEAN GOVERNORS TO
        // READ DATA BASED ON THE CURRENTLY FLAGGED GOVERNORS 
        // TODO: Need to add a bridle reading section here that reads line data into a single line and adds to container for line gallery
        // TODO: There's for sure a cleaner way to implement the logic on this loop, especially with regard to rotation for the center cell. 
        while (std::getline(in_file, line)) {
            if(line.find("profile") != std::string::npos){
                // MODIFY BOOLEANS ACCORDINGLY
                new_cell = true;
                
                // CREATE NEW CELL WITH AIRFOIL PROFILE COORDINATES AND ADD TO CELL VECTOR
                if(!airfoil_coords.empty()){
                    Cell temp_cell {profile_number, airfoil_coords};
                    canopy_cells.push_back(temp_cell);
                    ++profile_number;
                }

            }else if(line.find("bridle") != std::string::npos){
                // MODIFY BOOLEANS ACCORDINGLY
                bridle_found = true;
                profile_section = false;

                // CREATE LAST CELL WITH AIRFOIL PROFILE COORDINATES AND ADD TO CELL VECTOR
                Cell temp_cell {profile_number, airfoil_coords};
                canopy_cells.push_back(temp_cell);

            }else if(profile_section && !line.empty()){
                // MODIFY BOOLEANS AND CONTAINER ACCORDINGLY
                if(new_cell){
                    airfoil_coords.clear();
                    airfoil_coords.set_size(3,0);
                    new_cell = false;
                }

                // CREATE TEMPORARY STREAM FOR COORDINATES
                std::istringstream coord_stream {line};

                // READ COORDINATES FROM STREAM, PUSH TO AIRFOIL COORDINATE MATRIX IN DESIRED ORDER
                airfoil_coords.resize(3,(airfoil_coords.n_cols+1));
                coord_stream >> airfoil_coords(1,airfoil_coords.n_cols-1) 
                             >> airfoil_coords(2,airfoil_coords.n_cols-1) 
                             >> airfoil_coords(0,airfoil_coords.n_cols-1);

            }else if (bridle_found) {
                // std::cout << "We hit the bridle section on line: " << line_counter << std::endl; 
            
            }else if (line.empty()){
                // std::cout << "The following line number is empty: " << line_counter << std::endl; 
            }
            
            // DIAGNOSTICS VARIABLE UPDATES
            ++line_counter;
        }
        
        // INITIALIZE AND LINK SUB-CLASSES
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