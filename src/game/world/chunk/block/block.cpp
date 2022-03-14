#include "block.hpp"

#include <map>
#include <string>

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

inline std::map<Block::BlockType, std::map<std::string, std::pair<int, int>>> getTextureCoords() {
    std::map<Block::BlockType, std::map<std::string, std::pair<int, int>>> textureCoords;

    // Grass
    textureCoords[Block::BlockType::GRASS]["y+"] = std::make_pair(0, 0);
    textureCoords[Block::BlockType::GRASS]["y-"] = std::make_pair(2, 0);
    textureCoords[Block::BlockType::GRASS]["x+"] = std::make_pair(3, 0);
    textureCoords[Block::BlockType::GRASS]["x-"] = std::make_pair(3, 0);
    textureCoords[Block::BlockType::GRASS]["z+"] = std::make_pair(3, 0);
    textureCoords[Block::BlockType::GRASS]["z-"] = std::make_pair(3, 0);

    return textureCoords;
}

// getBlockInfo function
Block::BlockInfo Block::getBlockInfo(BlockType blockType) {
    auto textureCoords = getTextureCoords();

    switch (blockType) {
        case BlockType::AIR:
            return BlockInfo{
                .textureCoords = textureCoords[BlockType::AIR]
            };
        case BlockType::GRASS:
            return BlockInfo{
                .textureCoords = textureCoords[BlockType::GRASS]
            };
        case BlockType::STONE:
            return BlockInfo{
                .textureCoords = textureCoords[BlockType::STONE]
            };
        default:
            return BlockInfo{
                .textureCoords = textureCoords[BlockType::AIR]
            };
    }
}