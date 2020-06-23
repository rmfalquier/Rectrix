#include "Glider.h" 

// CONSTRUCTOR AND ASSOCIATED METHODS
Glider::Glider(std::string glider_file_name) {
    // NAME THE GLIDER
    name = Extract_Glider_Name(glider_file_name);
    
    // CREATE POINTERS FOR NECCESSARY DATA, THEN OPEN AND READ GLIDERPLAN FILE TO INITIALIZE GLIDER DATA STRUCTURE IN GIVEN FORMAT
    std::unique_ptr<std::vector<Pgline>> gallery_lines = std::make_unique<std::vector<Pgline>>();
    std::unique_ptr<std::vector<Cell>> canopy_cells = std::make_unique<std::vector<Cell>>();
    Read_Gliderplan(glider_file_name, canopy_cells, gallery_lines);

    // INITIALIZE SUB-CLASSES WITH EXTRACTED DATA
    canopy_ptr = std::make_unique<Canopy>(*canopy_cells);
    gallery_ptr = std::make_unique<Gallery>(*gallery_lines); 
    // TODO: Initialize Payload

    // GENERATE SUCCESS MESSAGE AND OUTPUT GENERAL INITIALIZED DATA
}

// TODO: PRINT AND VERIFY CENTER LINE GEOMETRY POST TRANSLATION AND ROTATION!!!!
void Glider::Read_Gliderplan(const std::string &glider_file_name, 
                             std::unique_ptr<std::vector<Cell>> &canopy_cells, 
                             std::unique_ptr<std::vector<Pgline>> &gallery_lines){
    std::ifstream in_file;
    in_file.open(glider_file_name);
    if (!in_file) {
        std::cerr << "Read_Gliderplan() -> Problem opening glider file: " << glider_file_name << std::endl;
    }
    else{
        // EXTRACT THE DATA NECCESSARY TO TRANSLATE ALL TO LE BODY FIXED FoR
        std::vector<double> translators {LE_Origin_Translators(glider_file_name)};
        double x_le {translators.at(0)};
        double z_le {translators.at(1)};
        double ctr_incidence {translators.at(2)};
        arma::mat rotation_matrix = {{ cos(ctr_incidence), 0, sin(ctr_incidence)},
                                     {          0        , 1,          0        },
                                     {-sin(ctr_incidence), 0, cos(ctr_incidence)}};
        
        // TEMPORARY STORAGE FOR FILE READING
        std::string line{};

        // BOOLEAN GOVERNORS FOR READ CASES
        bool new_cell {false};
        bool bridle_found {false};

        // CANOPY SUB-CLASS RELATED VARIABLES
        int profile_number {1}; // Gliderplan starts counting at 1
        arma::mat airfoil_coords(3,0);

        // GLIDERPLAN FILE READING LOOP:
        while (std::getline(in_file, line)) {
            if(line.find("profile") != std::string::npos){
                // MODIFY BOOLEANS ACCORDINGLY
                new_cell = true;
                
                // CREATE NEW CELL WITH ROTATED AIRFOIL PROFILE COORDINATES AND ADD TO CELL VECTOR
                if(!airfoil_coords.empty()){
                    airfoil_coords = rotation_matrix*airfoil_coords;
                    Cell temp_cell {profile_number, airfoil_coords};
                    canopy_cells->push_back(temp_cell);
                    ++profile_number;
                }

            }else if(line.find("bridle") != std::string::npos){
                // MODIFY BOOLEANS ACCORDINGLY
                bridle_found = true;

                // CREATE LAST CELL WITH ROTATED AIRFOIL PROFILE COORDINATES AND ADD TO CELL VECTOR
                airfoil_coords = rotation_matrix*airfoil_coords;
                Cell temp_cell {profile_number, airfoil_coords};
                canopy_cells->push_back(temp_cell);

            }else if(!bridle_found && !line.empty()){
                // MODIFY BOOLEANS AND CONTAINER ACCORDINGLY
                if(new_cell){
                    airfoil_coords.clear();
                    airfoil_coords.set_size(3,0);
                    new_cell = false;
                }

                // CREATE TEMPORARY STREAM AND STORAGE FOR COORDINATES
                std::istringstream coord_stream {line};
                double temp_x {};
                double temp_y {};
                double temp_z {};

                // READ COORDINATES FROM STREAM, PUSH TO AIRFOIL COORDINATE MATRIX IN DESIRED ORDER
                airfoil_coords.resize(3,(airfoil_coords.n_cols+1));
                coord_stream >> temp_y 
                             >> temp_z
                             >> temp_x;

                // TRANSLATE COORDINATES TO LE BODY FIXED FoR
                airfoil_coords(0,airfoil_coords.n_cols-1) = temp_x - x_le;
                airfoil_coords(1,airfoil_coords.n_cols-1) = -1*temp_y; 
                airfoil_coords(2,airfoil_coords.n_cols-1) = temp_z - z_le;

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

                // TRANSLATE AND ROTATE COORDINATES TO LE BODY FIXED FoR
                temp_line_coords.row(0) = temp_line_coords.row(0) - x_le;
                temp_line_coords.row(1) = -1*temp_line_coords.row(1);
                temp_line_coords.row(2) = temp_line_coords.row(2) - z_le;
                temp_line_coords = rotation_matrix*temp_line_coords;

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
    return extracted_name;
}

// TODO: VERIFY CENTER INCIDENCE ANGLE WITH MATLAB OR MANUALLY
std::vector<double> Glider::LE_Origin_Translators(const std::string &glider_file_name){
    // EXTRACT THE DATA NECCESSARY TO TRANSLATE ALL TO LE BODY FIXED FoR
    std::vector<double> translators {};   
    
    // FILE READING
    std::ifstream in_file;
    in_file.open(glider_file_name);
    if (!in_file) {
        std::cerr << "LE_Origin_Translators() -> Problem opening glider file: " << glider_file_name << std::endl;
    }
    else{
        // TEMPORARY STORAGE AND DIAGNOSTIC VARIABLES FOR FILE READING
        std::string line{};

        // BOOLEAN GOVERNORS FOR READ CASES
        bool bridle_found {false};
        bool center_cell_found {false};

        // CANOPY SUB-CLASS RELATED VARIABLES
        arma::mat airfoil_coords(3,0);

        // GLIDERPLAN FILE READING LOOP:
        while (!bridle_found && (std::getline(in_file, line))) {
            std::istringstream coord_stream {line};
            double coordinate_catcher;

            if(line.find("bridle") != std::string::npos){
                bridle_found = true;
                if(!center_cell_found){
                    std::cout << "LE_Origin_Translators() -> Error, center cell not found!" << std::endl; 
                }

            }else if(coord_stream >> coordinate_catcher){
                if(ceil(coordinate_catcher) == coordinate_catcher){   
                    // READ COORDINATES FROM STREAM, PUSH TO AIRFOIL COORDINATE MATRIX IN DESIRED ORDER
                    airfoil_coords.resize(3,(airfoil_coords.n_cols+1));
                    airfoil_coords(1,airfoil_coords.n_cols-1) = coordinate_catcher;
                    coord_stream >> airfoil_coords(2,airfoil_coords.n_cols-1) 
                                 >> airfoil_coords(0,airfoil_coords.n_cols-1);

                    // UPDATE CENTER CELL FLAG
                    if(!center_cell_found){
                        center_cell_found = true;
                    }
                }
            }
        }
        // EXTRACT COORDINATES AND CALCULATE TRANSLATORS
        double x_le {airfoil_coords.row(0).max()};
        auto x_le_idx {airfoil_coords.row(0).index_max()};
        double z_le {airfoil_coords.at(2,x_le_idx)};
        
        double x_te {airfoil_coords.row(0).min()};
        auto x_te_idx {airfoil_coords.row(0).index_min()};
        double z_te {airfoil_coords.at(2,x_te_idx)};

        double ctr_incidence = atan((z_le - z_te)/(x_le - x_te));
        // std::cout << ctr_incidence*180/pi << std::endl; 

        translators.push_back(x_le);
        translators.push_back(z_le);
        translators.push_back(ctr_incidence);
    }
    // FILE CLOSING AND RETURN
    in_file.close();
    return translators;
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