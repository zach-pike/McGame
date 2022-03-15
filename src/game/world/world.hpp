#pragma once

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <map>

#include "game/world/chunk/chunk.hpp"

class Game;
class World {
    private:
        std::map<std::pair<int, int>, Chunk> chunks;
        const int chunk_x_count;
        const int chunk_z_count;

    public:
        World(int chunk_x_count, int chunk_z_count);
        ~World();

        void addChunk(Chunk chunk);
        Chunk getChunk(int x, int z) const;

        void setBlock(int x, int y, int z, Block block);
        Block getBlock(int x, int y, int z) const;

        bool isCoordInWorld(int x, int y, int z) const;

        void generateMesh(std::vector<GLfloat>& verticies, std::vector<GLuint>& indicies, std::vector<GLfloat>& uvs) const;
};