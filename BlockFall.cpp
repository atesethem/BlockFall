#include "BlockFall.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string& leaderboard_file_name, const string& player_name) : gravity_mode_on(
    gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}


void BlockFall::read_blocks(const string& input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
    vector<vector<vector<bool>>> blocks;  // block larýn tipleri bool
    ifstream file(input_file);
    if (file.is_open()) {
        string line;
        vector<vector<bool>> current_block;
        while (getline(file, line)) {
            if (line.empty()) {
                if (!current_block.empty()) { // burasý da boþ olabilir bilemezsin boþ eklemesin.
                    blocks.push_back(current_block);
                    current_block.clear();
                }
            }
            else {
                vector<bool>row;
                for (char truth : line) {
                    if (truth == '[' || truth == ']') {
                        continue;
                    }
                    else if (truth == '0') {
                        row.push_back(false);
                    }
                    else if (truth == '1') {
                        row.push_back(true);
                    }
                }
                current_block.push_back(row);

            }

        }
        file.close();
        if (!current_block.empty()) {
            blocks.push_back(current_block);
        }

        power_up = current_block;
        initial_block = new Block();
        active_rotation = new Block();
        initial_block->shape = blocks[0];
        Block* previous = new Block();
        previous->shape = blocks[0];

        for (int i = 1; i < blocks.size(); i++) {
            Block* current = new Block();
            vector<vector<bool>> newrotated = blocks[i - 1];
            Block* prevrotatedshape = new Block();
            current->shape = blocks[i];
            previous->next_block = current;
            for (size_t j = 0; j < 4; j++) {
                vector<vector<bool>> rotated(newrotated[0].size(), vector<bool>(newrotated.size(), false));
                Block* rotatedshape = new Block();
                for (int k = 0; k < newrotated.size(); ++k) {
                    for (int l = 0; l < newrotated[0].size(); ++l) {
                        rotated[l][newrotated.size() - 1 - k] = newrotated[k][l];
                    }
                }
                newrotated = rotated;
                rotatedshape->shape = rotated;
                if (rotatedshape->shape != previous->shape) {
                    if (previous->right_rotation == nullptr) {
                        previous->right_rotation = rotatedshape;
                        rotatedshape->left_rotation = previous;
                        rotatedshape->next_block = current;
                        prevrotatedshape = rotatedshape;



                    }
                    else {
                        prevrotatedshape->right_rotation = rotatedshape;
                        rotatedshape->left_rotation = prevrotatedshape;
                        rotatedshape->next_block = current;
                        prevrotatedshape = rotatedshape;

                    }
                }
                else if (rotatedshape->shape == previous->shape && previous->right_rotation == nullptr) {
                    previous->left_rotation = previous;
                    previous->right_rotation = previous;
                    delete rotatedshape;
                    break;
                }
                else if (rotatedshape->shape == previous->shape && previous->right_rotation != nullptr) {
                    prevrotatedshape->right_rotation = previous;
                    previous->left_rotation = prevrotatedshape;
                    delete rotatedshape;

                    break;
                }


            }
            active_rotation = previous;

            if (i == 1) {
                initial_block = previous;
            }
            if (i != blocks.size() - 1) {
                previous = current;
            }
            if (i == blocks.size() - 1) {
                while (active_rotation->next_block != nullptr) {
                    active_rotation->next_block = nullptr;
                }

            }


        }


    }

}

void BlockFall::initialize_grid(const string& input_file) {
    ifstream file(input_file);
    if (file.is_open()) {
        string line;
        vector<vector<int>> temp_grid;
        while (getline(file, line)) {
            istringstream iss(line);
            vector<int> row;
            int value;
            while (iss >> value) {
                row.push_back(value);
            }
            temp_grid.push_back(row);
        }
        file.close();

        rows = temp_grid.size();
        cols = temp_grid[0].size();
        grid = temp_grid;
    }
}
void BlockFall::printBlocks(const vector<vector<vector<bool>>>& blocks) {
    for (const auto& block : blocks) {
        for (const auto& row : block) {
            for (bool value : row) {
                cout << (value ? "1" : "0") << " ";
            }
            cout << endl;
        }
        cout << "-----" << endl;
    }
}

void BlockFall::print2dBlocks(const vector<vector<bool>>& blocks) {

    if (blocks.empty()) {
        std::cout << "wewe";
    }
    for (const auto& row : blocks) {
        for (bool value : row) {
            cout << (value ? "1" : "0") << " ";
        }
        cout << endl;
    }
    cout << "-----" << endl;
}



BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks


    Block* current = initial_block;
    while (current != nullptr) {
        Block* temp = current;
        Block* rotation = current->right_rotation;
        while (rotation != nullptr && rotation != current) {
            Block* temp2 = rotation;
            rotation = rotation->right_rotation;
            delete temp2;
        }
        current = current->next_block;
        delete temp;
    }


    power_up.clear();


}