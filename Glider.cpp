#include "Glider.h" 

// Constructor
Glider::Glider(std::string glider_file_name) {
    // NAME THE GLIDER
    name = glider_file_name;
    
    // OPEN AND READ GLIDERPLAN FILE TO INITIALIZE GLIDER DATA STRUCTURE IN GIVEN FORMAT
    // TODO: MOVE ALL THIS TO A FUNCTION
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

        // CANOPY SUB-CLASS RELATED VARIABLES
        int profile_number {1}; // Gliderplan starts counting at 1
        arma::mat airfoil_coords(3,0);
        std::vector<Cell> canopy_cells;

        // LINE GALLERY SUB-CLASS RELATED VARIABLES
        std::vector<Pgline> gallery_lines;

        // GLIDERPLAN FILE READING LOOP:
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

                // CREATE LAST CELL WITH AIRFOIL PROFILE COORDINATES AND ADD TO CELL VECTOR
                Cell temp_cell {profile_number, airfoil_coords};
                canopy_cells.push_back(temp_cell);

            }else if(!bridle_found && !line.empty()){
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

            }else if (bridle_found && !line.empty()) {
                // ! WARNING - BRIDLE CURRENTLY DOES NOT READ THE LAST TWO ENTRIES AFTER MATERIAL
                // CREATE TEMPORARY STORAGE AND STREAM FOR COORDINATES
                std::istringstream coord_stream {line};
                std::string temp_line_id {};
                arma::mat temp_line_coords(3,2);
                std::string temp_line_material {};

                // READ COORDINATES FROM STREAM, AND PUT IN TEMPORARY STORAGE
                coord_stream >> temp_line_id
                             >> temp_line_coords(1,0) 
                             >> temp_line_coords(2,0) 
                             >> temp_line_coords(0,0) 
                             >> temp_line_coords(1,1) 
                             >> temp_line_coords(2,1) 
                             >> temp_line_coords(0,1)
                             >> temp_line_material;

                // EDIT INPUTS FOR COMPATIBILITY
                temp_line_id.erase(std::find(temp_line_id.begin(), temp_line_id.end(), '\"'));
                temp_line_id.erase(std::find(temp_line_id.begin(), temp_line_id.end(), '\"'));
                
                if(temp_line_material.back()=='\"'){
                    temp_line_material.erase(std::find(temp_line_material.begin(), temp_line_material.end(), '\"'));
                    temp_line_material.erase(std::find(temp_line_material.begin(), temp_line_material.end(), '\"'));
                }else{
                    temp_line_material.erase(std::find(temp_line_material.begin(), temp_line_material.end(), '\"'));
                }
                if(temp_line_material.size()==0){
                    temp_line_material = "NA";
                }

                // CREATE NEW LINE AND ADD TO LINE VECTOR
                Pgline temp_line {temp_line_id, temp_line_coords, temp_line_material};
                gallery_lines.push_back(temp_line);
            }else if (line.empty()){
                // DO NOTHING
            }

            // DIAGNOSTICS VARIABLE UPDATES
            ++line_counter;
        }
        
        // INITIALIZE AND LINK SUB-CLASSES
        canopy_ptr = std::make_unique<Canopy>(canopy_cells);
        gallery_ptr = std::make_unique<Gallery>(gallery_lines); 
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
// TODO: Make modifications for const 
// TODO: Format for cleaner output
void Glider::gnu_print(){
    //Print Canopy File
    std::ofstream canopy_file{"./.output/canopy.rctx"};
    if (!canopy_file) {
        std::cerr << "Error creating canopy file" << std::endl;
    }else{
        std::vector<Cell> cells{canopy_ptr->get_cells()};
        for (auto current_cell:cells){
            arma::mat current_coords{current_cell.get_coords()};
            for(long long unsigned int i = 0; i<current_coords.n_cols; i++){ //Try with an iterator
                canopy_file << current_coords(0,i)
                          << " " 
                          << current_coords(1,i) 
                          << " " 
                          << current_coords(2,i) 
                          << std::endl;
            }
            canopy_file << "\n" << std::endl;
        }
    }
    canopy_file.close();

    // Print Gallery File
    // TODO: Mirror
    std::ofstream gallery_file{"./.output/gallery.rctx"};
    if (!gallery_file) {
        std::cerr << "Error creating gallery file" << std::endl;
    }else{
        std::vector<Pgline> lines{gallery_ptr->get_lines()};
        for (auto current_line:lines){
            arma::mat current_coords{current_line.get_coords()};
            gallery_file << current_coords(0,0)
                        << " " 
                        << current_coords(1,0) 
                        << " " 
                        << current_coords(2,0) 
                        << std::endl;
            gallery_file << current_coords(0,1)
                        << " " 
                        << current_coords(1,1) 
                        << " " 
                        << current_coords(2,1) 
                        << std::endl;
            gallery_file << "\n" << std::endl;
        }
    }
    gallery_file.close();
}