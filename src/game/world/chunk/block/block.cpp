#include "block.hpp"

// Block constructor
Block::Block(BlockType blockType) {
    this->type = blockType;
}

// Other constructor
Block::Block() {
    this->type = BlockType::AIR;
}

Block::BlockType Block::getType() {
    return this->type;
}

// getBlockInfo function
Block::BlockInfo Block::getBlockInfo(BlockType blockType) {
    switch (blockType) {
        case BlockType::AIR:
            return BlockInfo{0, 0};
        case BlockType::GRASS:
            return BlockInfo{0, 0};
        case BlockType::STONE:
            return BlockInfo{1, 0};
        default:
            return BlockInfo{15, 9};
    }
}