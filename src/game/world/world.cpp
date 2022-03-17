#include "world.hpp"

#include "external/PerlinNoise.hpp"

// World constructor
World::World(int chunk_x_count, int chunk_z_count)
    : chunk_x_count(chunk_x_count), chunk_z_count(chunk_z_count) {
    // Initialise the world
    for (int x = 0; x < chunk_x_count; x++) {
        for (int z = 0; z < chunk_z_count; z++) {
            addChunk(Chunk(*this, x, z));
        }
    }

    srand(time(NULL));

    // Generate the world
    const siv::PerlinNoise::seed_type seed = rand();

	const siv::PerlinNoise perlin{ seed };

    for (int x = 0; x < chunk_x_count * CHUNK_X_SIZE; x++) {
        for (int z = 0; z < chunk_z_count * CHUNK_Z_SIZE; z++) {
            double noise = perlin.octave2D_01((double)x * 0.01, (double)z * 0.01, 20);

            int y = noise * 10;

            for (int i = 0; i < y; i++) {
                setBlock(x, i, z, Block::BlockType::DIRT);
            }

            setBlock(x, y, z, Block::BlockType::GRASS);
        }
    }
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

    if (this->chunks.find(position) != this->chunks.end()) {
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

Chunk World::getChunk(int x, int z) const {
    return chunks.at(std::pair<int, int>(x, z));
}

// Get world size
std::pair<int, int> World::getWorldSize() const {
    return std::pair<int, int>(this->chunk_x_count, this->chunk_z_count);
}