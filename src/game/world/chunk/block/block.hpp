#pragma once

class Block {
    public:
        struct BlockInfo {
            int uvx;
            int uvy;
        };

        enum class BlockType {
            AIR,
            GRASS,
            STONE
        };
    private:
        BlockType type;

    public:
        static BlockInfo getBlockInfo(BlockType blockType);

        BlockType getType();

        Block(BlockType blockType);
        Block();
};