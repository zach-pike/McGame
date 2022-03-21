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

std::map<Block::BlockType, std::map<std::string, std::pair<int, int>>> getTextureCoords() {
    std::map<Block::BlockType, std::map<std::string, std::pair<int, int>>> textureCoords;

    // Grass
    textureCoords[Block::BlockType::GRASS]["y+"] = std::make_pair(0, 0);
    textureCoords[Block::BlockType::GRASS]["y-"] = std::make_pair(2, 0);
    textureCoords[Block::BlockType::GRASS]["x+"] = std::make_pair(3, 0);
    textureCoords[Block::BlockType::GRASS]["x-"] = std::make_pair(3, 0);
    textureCoords[Block::BlockType::GRASS]["z+"] = std::make_pair(3, 0);
    textureCoords[Block::BlockType::GRASS]["z-"] = std::make_pair(3, 0);

    // Dirt
    textureCoords[Block::BlockType::DIRT]["y+"] = std::make_pair(2, 0);
    textureCoords[Block::BlockType::DIRT]["y-"] = std::make_pair(2, 0);
    textureCoords[Block::BlockType::DIRT]["x+"] = std::make_pair(2, 0);
    textureCoords[Block::BlockType::DIRT]["x-"] = std::make_pair(2, 0);
    textureCoords[Block::BlockType::DIRT]["z+"] = std::make_pair(2, 0);
    textureCoords[Block::BlockType::DIRT]["z-"] = std::make_pair(2, 0);

    // Stone
    textureCoords[Block::BlockType::STONE]["y+"] = std::make_pair(1, 0);
    textureCoords[Block::BlockType::STONE]["y-"] = std::make_pair(1, 0);
    textureCoords[Block::BlockType::STONE]["x+"] = std::make_pair(1, 0);
    textureCoords[Block::BlockType::STONE]["x-"] = std::make_pair(1, 0);
    textureCoords[Block::BlockType::STONE]["z+"] = std::make_pair(1, 0);
    textureCoords[Block::BlockType::STONE]["z-"] = std::make_pair(1, 0);

    // Diamond ore
    textureCoords[Block::BlockType::DIAMOND_ORE]["y+"] = std::make_pair(2, 3);
    textureCoords[Block::BlockType::DIAMOND_ORE]["y-"] = std::make_pair(2, 3);
    textureCoords[Block::BlockType::DIAMOND_ORE]["x+"] = std::make_pair(2, 3);
    textureCoords[Block::BlockType::DIAMOND_ORE]["x-"] = std::make_pair(2, 3);
    textureCoords[Block::BlockType::DIAMOND_ORE]["z+"] = std::make_pair(2, 3);
    textureCoords[Block::BlockType::DIAMOND_ORE]["z-"] = std::make_pair(2, 3);

    return textureCoords;
}

// getBlockInfo function
Block::BlockInfo Block::getBlockInfo(BlockType blockType) {
    auto textureCoords = getTextureCoords();

    return BlockInfo{
        .textureCoords = textureCoords[blockType]
    };
}