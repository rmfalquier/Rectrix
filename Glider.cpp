#include "Glider.h" 

// CONSTRUCTOR AND ASSOCIATED METHODS
Glider::Glider(std::string glider_file_name) {
    // NAME THE GLIDER
    name = Extract_Glider_Name(glider_file_name);
    
    // CREATE POINTERS FOR NECCESSARY DATA, THEN OPEN AND READ GLIDERPLAN FILE TO INITIALIZE GLIDER DATA STRUCTURE IN GIVEN FORMAT
    std::unique_ptr<std::vector<Pgline>> gallery_lines = std::make_unique<std::vector<Pgline>>();
    std::unique_ptr<std::vector<Cell>> canopy_cells = std::make_unique<std::vector<Cell>>();
    Read_Gliderplan(glider_file_name, canopy_cells, gallery_lines);
    // TODO: Add coordinate system transfer to Read_Gliderplan

    // INITIALIZE SUB-CLASSES WITH EXTRACTED DATA
    canopy_ptr = std::make_unique<Canopy>(*canopy_cells);
    gallery_ptr = std::make_unique<Gallery>(*gallery_lines); 
    // TODO: Initialize Payload
}

void Glider::Read_Gliderplan(const std::string &glider_file_name, 
                             std::unique_ptr<std::vector<Cell>> &canopy_cells, 
                             std::unique_ptr<std::vector<Pgline>> &gallery_lines){
    std::ifstream in_file;
    in_file.open(glider_file_name);
    if (!in_file) {
        std::cerr << "Problem opening glider file: " << glider_file_name << std::endl;
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

        // GLIDERPLAN FILE READING LOOP:
        // TODO: There's for sure a cleaner way to implement the logic on this loop, especially with regard to rotation for the center cell. 
        while (std::getline(in_file, line)) {
            if(line.find("profile") != std::string::npos){
                // MODIFY BOOLEANS ACCORDINGLY
                new_cell = true;
                
                // CREATE NEW CELL WITH AIRFOIL PROFILE COORDINATES AND ADD TO CELL VECTOR
                if(!airfoil_coords.empty()){
                    Cell temp_cell {profile_number, airfoil_coords};
                    canopy_cells->push_back(temp_cell);
                    ++profile_number;
                }

            }else if(line.find("bridle") != std::string::npos){
                // MODIFY BOOLEANS ACCORDINGLY
                bridle_found = true;

                // CREATE LAST CELL WITH AIRFOIL PROFILE COORDINATES AND ADD TO CELL VECTOR
                Cell temp_cell {profile_number, airfoil_coords};
                canopy_cells->push_back(temp_cell);

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
                gallery_lines->push_back(temp_line);
            }else if (line.empty()){
                // DO NOTHING
            }

            // DIAGNOSTICS VARIABLE UPDATES
            ++line_counter;
        }
        
        
    }
    in_file.close(); //This someimtes throws a super annoying intellisense error on VSCODE...
}

std::string Glider::Extract_Glider_Name(const std::string &glider_file_name){
    std::string extracted_name {};
    if(glider_file_name.find(".txt") != std::string::npos){
        auto const pos1 = glider_file_name.find_last_of('/');
        auto const pos2 = glider_file_name.find_last_of('.');
        auto const count {pos2 - pos1 - 1};
        extracted_name = glider_file_name.substr(pos1+1, count);
    }else{
        extracted_name = glider_file_name;
    }
    std::cout << extracted_name << std::endl; 
    return extracted_name;
}

// DESTRUCTOR
Glider::~Glider(){
}

// GET METHODS

// SET METHODS

// DATA OUTPUT METHODS
void Glider::GNU_Print() const {
    //Print Canopy File
    std::string canopy_file_name {"./.output/" + name + "_canopy.rctx"};
    std::ofstream canopy_file{canopy_file_name};
    if (!canopy_file) {
        std::cerr << "Error creating canopy file" << std::endl;
    }else{
        std::vector<Cell> cells{canopy_ptr->Get_Cells()};
        for (const auto current_cell:cells){
            arma::mat current_coords{current_cell.Get_Coords()};
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
    std::string gallery_file_name {"./.output/" + name + "_gallery.rctx"};
    std::ofstream gallery_file{gallery_file_name};
    if (!gallery_file) {
        std::cerr << "Error creating gallery file" << std::endl;
    }else{
        std::vector<Pgline> lines{gallery_ptr->Get_Lines()};
        for (const auto current_line:lines){
            arma::mat current_coords{current_line.Get_Coords()};
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

            // Mirror Side
            gallery_file << current_coords(0,0)
                        << " " 
                        << -current_coords(1,0) 
                        << " " 
                        << current_coords(2,0) 
                        << std::endl;
            gallery_file << current_coords(0,1)
                        << " " 
                        << -current_coords(1,1) 
                        << " " 
                        << current_coords(2,1) 
                        << std::endl;
            gallery_file << "\n" << std::endl;
        }
    }
    gallery_file.close();
}