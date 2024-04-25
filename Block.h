#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block* right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block* left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block* next_block = nullptr; // Pointer to the next block to appear in the game
    size_t variable = 0;
    bool operator==(const Block& other) const {
        // TODO: Overload the == operator to compare two blocks based on their shapes
        return this->shape == other.shape;
    }

    bool operator!=(const Block& other) const {
        // TODO: Overload the != operator to compare two blocks based on their shapes
        return !(*this == other);
       
    }
    Block& operator=(const Block& other) {
        if (this != &other) { 
            this->shape = other.shape;
            this->right_rotation = other.right_rotation;
            this->left_rotation = other.left_rotation;
            this->next_block = other.next_block;
            this->variable = other.variable;
        }
        return *this;
    }
    Block* copy() const {
        Block* newBlock = new Block(); 
        newBlock->shape = this->shape;
        newBlock->right_rotation = this->right_rotation;
        newBlock->left_rotation = this->left_rotation;
        newBlock->next_block = this->next_block;
        newBlock->variable = this->variable;

        return newBlock;
    }
    

};


#endif //PA2_BLOCK_H
