#include "world.hpp"

// World constructor
World::World() {

}

// World destructor
World::~World() {

}

void World::addChunk(Chunk chunk) {
    this->chunks.insert(std::pair<std::pair<int, int>, Chunk>(chunk.getPosition(), chunk));
}

void World::setBlock(int x, int y, int z, Block block) {
    int chunkx = x / CHUNK_X_SIZE;
    int chunkz = z / CHUNK_Z_SIZE;

    int xoffset = x % CHUNK_X_SIZE;
    int zoffset = z % CHUNK_Z_SIZE;

    std::pair<int, int> position = std::pair<int, int>(chunkx, chunkz);

    if (this->chunks.find(position) == this->chunks.end()) {
        this->chunks.at(position).setBlock(xoffset, y, zoffset, block);
    }
}

Block World::getBlock(int x, int y, int z) const {
    int chunkx = x / CHUNK_X_SIZE;
    int chunkz = z / CHUNK_Z_SIZE;

    int xoffset = x % CHUNK_X_SIZE;
    int zoffset = z % CHUNK_Z_SIZE;

    std::pair<int, int> position = std::pair<int, int>(chunkx, chunkz);

    if (this->chunks.find(position) == this->chunks.end()) {
        throw std::runtime_error("Chunk not found");
    } else {
        return this->chunks.at(position).getBlock(xoffset, y, zoffset);
    }
}

bool World::isCoordInWorld(int x, int y, int z) const {
    if (y > CHUNK_Y_SIZE || y < 0) {
        return false;
    }

    int chunkx = x / CHUNK_X_SIZE;
    int chunkz = z / CHUNK_Z_SIZE;

    // Check if the chunk exists
    std::pair<int, int> position = std::pair<int, int>(chunkx, chunkz);

    if (this->chunks.find(position) == this->chunks.end()) {
        return false;
    }

    return true;
}

void World::generateMesh(std::vector<GLfloat>& verticies, std::vector<GLuint>& indicies, std::vector<GLfloat>& uvs) const {
    GLuint verticies_c = 0;
    for (auto chunk : this->chunks) {
        chunk.second.generateMesh(verticies, indicies, uvs, verticies_c);
    }
}