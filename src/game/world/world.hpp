#pragma once

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <map>

#include "game/world/chunk/chunk.hpp"
class World {
    private:
        std::map<std::pair<int, int>, Chunk> chunks;

    public:
        World();
        ~World();

        void addChunk(Chunk chunk);

        void setBlock(int x, int y, int z, Block block);
        Block getBlock(int x, int y, int z) const;

        bool isCoordInWorld(int x, int y, int z) const;

        void generateMesh(std::vector<GLfloat>& verticies, std::vector<GLuint>& indicies, std::vector<GLfloat>& uvs) const;
};