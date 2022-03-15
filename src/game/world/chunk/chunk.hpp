#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "block/block.hpp"

#define CHUNK_X_SIZE 16
#define CHUNK_Y_SIZE 256
#define CHUNK_Z_SIZE 16

#define CHUNK_SIZE (CHUNK_X_SIZE * CHUNK_Y_SIZE * CHUNK_Z_SIZE)

class World;

class Chunk {
    private:
        std::array<Block, CHUNK_SIZE> blocks;
        World& world;

        const int xoffset;
        const int zoffset;

    public:
        Chunk(World& world, int xoffset, int zoffset);
        ~Chunk();

        std::pair<int, int> getPosition() const;

        void setBlock(int x, int y, int z, Block block);
        Block getBlock(int x, int y, int z) const;

        void generateMesh(std::vector<GLfloat>& verticies, std::vector<GLuint>& indicies, std::vector<GLfloat>& uvs, GLuint& verticies_c) const;
};