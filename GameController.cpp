#include "GameController.h"
#include <fstream>
#include <sstream>
bool GameController::play(BlockFall& game, const string& commands_file) {

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.
    ifstream commands(commands_file);
    string command;
    Block* initialBlock = game.initial_block;
    Block* newblock = initialBlock;
    Block* prevblock = newblock->copy();
    placeBlock(game.grid, initialBlock);
    
    while (getline(commands, command)) {

        if (command == "PRINT_GRID") {
            printGrid(game.grid);
            //game.print2dBlocks(newblock->shape);
           // std::cout << newblock->variable << "\n";
           // game.print2dBlocks(prevblock->shape);
          //  std::cout << prevblock->variable << "\n";
        }
        else if (command == "ROTATE_RIGHT") {
            if (controller(game.grid, newblock->right_rotation,prevblock)) {
                newblock = newblock->right_rotation;
                game.active_rotation = newblock;
                placeBlock(game.grid, newblock);
                if (nextblock(game.grid, newblock)) {
                    if (newblock->next_block) {
                        newblock = newblock->next_block;
                        if (!involvement(game.grid, newblock)) {
                            return false;
                        }
                       
                    }
                    else { return false; }
                }
               *prevblock = *newblock;
            }
            
        }
        else if (command == "ROTATE_LEFT") {
           
            if (controller(game.grid, newblock->left_rotation,prevblock)) {               
                newblock = newblock->left_rotation;
                game.active_rotation = newblock;
                placeBlock(game.grid, newblock);
                if (nextblock(game.grid, newblock)) {
                    if (newblock->next_block) {
                        newblock = newblock->next_block;
                        if (!involvement(game.grid, newblock)) {
                            return false;
                        }
                    }
                    else { return false; }
                }
                *prevblock = *newblock;
            }
           
        }
        else if (command == "MOVE_RIGHT") {
            if (newblock->variable + 1 <= game.grid[0].size()) {
                newblock->variable = newblock->variable + 1;
               // std:cout << prevblock->variable << "\n";
               
                if (controller(game.grid, newblock, prevblock)) {
                    placeBlock(game.grid, newblock);
                    variableChanger(newblock);
                    if (nextblock(game.grid, newblock)) {
                        if (newblock->next_block) {
                            newblock = newblock->next_block;
                            if (!involvement(game.grid, newblock)) {
                                std::cout << "ethemmm";
                                return false;
                            }
                        }
                        else {  return false; }
                    }
                    *prevblock = *newblock;
                }
                else {
                    newblock->variable = newblock->variable - 1 ;
         
                }
           }
        }
        else if (command == "MOVE_LEFT") {
            if (newblock->variable - 1 >= 0) {
                newblock->variable = newblock->variable - 1;
                if (controller(game.grid, newblock, prevblock)) {
                    placeBlock(game.grid, newblock);
                    variableChanger(newblock);
                    if (nextblock(game.grid, newblock)) {
                        std::cout << "ethemmm";
                        if (newblock->next_block) {
                            newblock = newblock->next_block;
                            if (!involvement(game.grid, newblock)) {
                                std::cout << "ethemmm";
                                return false;
                            }
                        }
                        else {  return false; }
                    }
                    *prevblock = *newblock;
                }
                else {
                    newblock->variable = newblock->variable + 1;

                }
            }
        }
        else if (command == "DROP") {
            dropBlock(game.grid, newblock,game.gravity_mode_on);
            printGrid(game.grid);
            if (checkShapeInGrid(game.grid, game.power_up)) {
                setGridToZero(game.grid);
            }
            if (newblock->next_block) {
                newblock = newblock->next_block;
                if (!involvement(game.grid, newblock)) {
                    
                    return false;
                }
            }
            else { std::cout << "ethem"; return false; }
            *prevblock = *newblock;
        }
        else if (command == "GRAVITY_SWITCH") {
            if (game.gravity_mode_on) {
                game.gravity_mode_on = false;
            }
            else {
                game.gravity_mode_on = true;
            }
            if (game.gravity_mode_on) {
                fillEmptySpaces(game.grid,newblock);
            }
            

        }
        else {
            std::cout << "Unknown command: " << command << "\n" << "\n";
        }
        
        clearCompletedRows(game.grid);
        


    }
    

    return true;
}
void GameController::dropBlock(vector<vector<int>>& grid, Block* theBlock, bool gravity) {
    int blockHeight = theBlock->shape.size();
    int blockWidth = theBlock->shape[0].size();
    size_t currentRow = 0;
    makeZero(grid, theBlock);
    if (!gravity) {
      while (currentRow + blockHeight < grid.size()) {
        bool canPlace = true;

        for (size_t i = 0; i < blockHeight; ++i) {
            for (size_t j = 0; j < blockWidth; ++j) {

                if (theBlock->shape[i][j] == true && (currentRow + i >= grid.size() || theBlock->variable + j >= grid[0].size())) {

                    canPlace = false;
                    break;
                }
                if (theBlock->shape[i][j] == true && i < blockHeight - 1 && theBlock->shape[i + 1][j] == 0 && grid[currentRow + i + 1][theBlock->variable + j] == 1) {
                    canPlace = false;
                    break;
                }
                if (theBlock->shape[i][j] == true && i == blockHeight - 1 && grid[currentRow + i + 1][theBlock->variable + j] == 1) {
                    canPlace = false;
                    break;
                }
            }
            if (!canPlace) {
                break;
            }
        }

        if (canPlace) {
            currentRow++;
        }
        else {
            break;
        }


      }
      
      placeBlockForFunc(grid, currentRow, theBlock);
    }
    else {
        
        for (size_t i = 0; i < blockHeight; ++i) {
            for (size_t j = 0; j < blockWidth; ++j) {
                if (theBlock->shape[i][j]) {
                    int fallDistance = 0;

                    while ( i + fallDistance + 1 < grid.size() && grid[i + fallDistance + 1][theBlock->variable + j] == 0) {
                        fallDistance++;
                    }

                    if (fallDistance > 0) {
                        
                        grid[i][theBlock->variable + j] = 0;
                        grid[i + fallDistance][theBlock->variable + j] = 1;
                    }
                }
            }
        }
    }
    
}
void GameController::placeBlock(vector<vector<int>>& grid, Block* theblock) {
    int blockHeight = theblock->shape.size();
    int blockWidth = theblock->shape[0].size();
    for (int i = 0; i < blockHeight; ++i) {
        for (int j = 0; j < blockWidth; ++j) {
            if (theblock->shape[i][j]) {
                grid[i][theblock->variable + j] = theblock->shape[i][j];
            }
        }
    }
}
void GameController::placeBlockForFunc(vector<vector<int>>& grid, size_t locationx, Block* theblock ) {
    int blockHeight = theblock->shape.size();
    int blockWidth = theblock->shape[0].size();
    for (int i = 0; i < blockHeight; ++i) {
        for (int j = 0; j < blockWidth; ++j) {
            if (theblock->shape[i][j]) {
                grid[locationx + i][theblock->variable + j] = theblock->shape[i][j];
            }
        }
    }
}
bool GameController::controller(vector<vector<int>>& grid, Block* theblock, Block* secondblock) {
    try {
        int blockHeight = theblock->shape.size();
        int blockWidth = theblock->shape[0].size();
        int gridHeight = grid.size();
        int gridWidth = grid[0].size();
        makeZero(grid, secondblock);
        //printGrid(grid);
        for (int i = 0; i < blockHeight; i++) {
            for (int j = 0; j < blockWidth; j++) {
                if (theblock->shape[i][j] == true) {
                    
                    if (theblock->variable + j < gridWidth && theblock->variable + j >= 0) {
                        
                        if (grid[i][theblock->variable + j] == true) {
                            placeBlock(grid, secondblock);
                            
                            return false;
                        }
                    }
                    else {
                        placeBlock(grid, secondblock);
                        
                        return false;
                    }
                    
                }
            }
        }
        return true; 
    }
    catch (const std::out_of_range& e) {
        return false;
    }
}
void GameController::printGrid(vector<vector<int>>& grid) {   
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {          
                std::cout << (grid[i][j] ? "1 " : "0 "); 
        }
        std::cout << endl ;
    }
    cout << "-----" << endl;
}
void GameController::makeZero(vector<vector<int>>& grid, Block* theblock) {
    int blockHeight = theblock->shape.size();
    int blockWidth = theblock->shape[0].size();
    for (int i = 0; i < blockHeight; ++i) {
        for (int j = 0; j < blockWidth; ++j) {
            if (theblock->shape[i][j]) {
                grid[i][theblock->variable + j] = 0;
            }
        }
    }
}

void GameController::variableChanger(Block* theblock) {
    Block* current = theblock->right_rotation;
    while (current != theblock) {
        current->variable = theblock->variable;
        current = current->right_rotation;
    }   
    
}
bool GameController::nextblock(vector<vector<int>>& grid , Block* theblock) {
    size_t blockHeight = theblock->shape.size();
    size_t blockWidth = theblock->shape[0].size();
    for (size_t i = 0; i < blockHeight;i++) {
        for (size_t j = 0; j < blockWidth; j++) {
            if (i != blockHeight - 1 && theblock->shape[i][j] == true) {
                if (grid[i + 1][theblock->variable + j] == 1 && !theblock->shape[i+1][j]) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
    }
    return false;
}
bool GameController::involvement(vector<vector<int>>& grid, Block* theblock) {
    int blockHeight = theblock->shape.size();
    int blockWidth = theblock->shape[0].size();
    int gridHeight = grid.size();
    int gridWidth = grid[0].size();
    for (int i = 0; i < blockHeight; i++) {
        for (int j = 0; j < blockWidth; j++) {
            if (theblock->shape[i][j] && grid[i][j]) {
                return 0;

            }
        }
    }
   // placeBlock(grid, theblock);
}
void GameController::clearCompletedRows(std::vector<std::vector<int>>& grid) {
   
    std::vector<int> rowstoerase;
    for (size_t i = 0; i < grid.size(); ++i) {
        bool rowfilled = true;
        for (int cell : grid[i]) {
            if (cell == 0) {
                rowfilled = false;
                break;
            }
        }

        if (rowfilled) {
           
            rowstoerase.push_back(i);
     
        }

    }
    for (int rowIdx : rowstoerase) {

        
        grid.erase(grid.begin() + rowIdx);
        grid.insert(grid.begin(), std::vector<int>(grid[0].size(), 0));
    }


}
void GameController::fillEmptySpaces(std::vector<std::vector<int>>& grid, Block* theblock) {
    int rows = grid.size();
    int cols = grid[0].size();
    makeZero(grid, theblock);
    for (int i = 0; i < cols; i++) {
        int counter = 0;
        for (int j = 0; j < rows; j++) {
            if (grid[j][i] == 1) {
                counter++;
                grid[j][i] = 0;
            }
        }
        std::cout << counter << "\n";
        for (int j = rows - 1; j >= 0; j--) {
            if (counter != 0) {
                grid[j][i] = 1;
                counter--;
            }
        }
    }
    
}
void GameController::setGridToZero(std::vector<std::vector<int>>& grid) {
    size_t rows = grid.size();
    size_t cols = grid[0].size();

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            grid[i][j] = 0;
        }
    }
}
void GameController::deleteandadd(std::vector<std::vector<int>>& grid) {
    if (!grid.empty()) {
        grid.pop_back(); 
        grid.insert(grid.begin(), std::vector<int>(grid[0].size(), 0));
    }
}
bool GameController::checkShapeInGrid(std::vector<std::vector<int>>& grid, std::vector<std::vector<bool>>& shape) {
    size_t gridRows = grid.size();
    size_t gridCols = grid[0].size();
    size_t shapeRows = shape.size();
    size_t shapeCols = shape[0].size();

    for (size_t i = 0; i <= gridRows - shapeRows; ++i) {
        for (size_t j = 0; j <= gridCols - shapeCols; ++j) {
            bool found = true;

            for (size_t si = 0; si < shapeRows; ++si) {
                for (size_t sj = 0; sj < shapeCols; ++sj) {
                    if ((shape[si][sj] == 1 && grid[i + si][j + sj] != 1) || (!shape[si][sj] && grid[i+si][j+sj] != 0)) {
                        found = false;
                        break;
                    }
                }
                if (!found) {
                    break;
                }
            }

            if (found) {
                return true; 
            }
        }
    }
    return false; 
}