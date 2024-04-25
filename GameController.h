#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"
#include <iostream>
using namespace std;

class GameController {
public:
    bool play(BlockFall& game, const string& commands_file); // Function that implements the gameplay
    void dropBlock(vector<vector<int>>& grid, Block* theBlock, bool gravity); // controller boolean ý oluþturucuaz 
    void placeBlock(vector<vector<int>>& grid, Block* theblock);
    void placeBlockForFunc(vector<vector<int>>& grid, size_t location, Block* theblock);
    bool controller(vector<vector<int>>& grid, Block* theblock, Block* secondblock);
    bool involvement(vector<vector<int>>& grid, Block* theblock);
    void variableChanger(Block* theblock);
    void printGrid(vector<vector<int>>& grid);
    void makeZero(vector<vector<int>>& grid, Block* theblock);
    void fillEmptySpaces(std::vector<std::vector<int>>& grid, Block* theblock);
    void setGridToZero(std::vector<std::vector<int>>& grid);
    void clearCompletedRows(std::vector<std::vector<int>>& grid);
    void deleteandadd(std::vector<std::vector<int>>& grid);
    bool nextblock(vector<vector<int>>& grid, Block* theblock);
    bool checkShapeInGrid(std::vector<std::vector<int>>& grid,  std::vector<std::vector<bool>>& shape);
    int location_x;
    int location_y;
};


#endif //PA2_GAMECONTROLLER_H
