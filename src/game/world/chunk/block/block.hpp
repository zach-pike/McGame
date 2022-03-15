#pragma once

#include <map>

class Block {
    public:
        struct BlockInfo {
            std::map<std::string, std::pair<int, int>> textureCoords;
        };

        enum class BlockType {
            AIR,
            GRASS,
            STONE,
            DIRT
        };
    private:
        BlockType type;

    public:
        static BlockInfo getBlockInfo(BlockType blockType);

        BlockType getType();

        Block(BlockType blockType);
        Block();
};