#include "chunk.hpp"

#include <string>
#include <map>
#include <iostream>

#include "game/world/world.hpp"

// Chunk constructor
Chunk::Chunk(World& world, int xoffset, int zoffset): 
    world(world),
    xoffset(xoffset),
    zoffset(zoffset)
{}

// setBlock function
void Chunk::setBlock(int x, int y, int z, Block block) {
    blocks[x + y * CHUNK_X_SIZE + z * CHUNK_X_SIZE * CHUNK_Y_SIZE] = block;
}

// getBlock function
Block Chunk::getBlock(int x, int y, int z) const {
    return blocks[x + y * CHUNK_X_SIZE + z * CHUNK_X_SIZE * CHUNK_Y_SIZE];
}

template<typename T>
inline T Mod(T a, T b) {
    T ret = a % b;
    return ret >= 0 ? ret : ret + b;
}

// generateMesh function
void Chunk::generateMesh(std::vector<GLfloat>& verticies, std::vector<GLuint>& indicies, std::vector<GLfloat>& uvs, GLuint& verticies_c) const {
    //Loop through all the blocks in the chunk
    for (GLfloat y = 0; y < CHUNK_Y_SIZE; y++) {
         for (GLfloat x = 0; x < CHUNK_X_SIZE; x++) {
            for (GLfloat z = 0; z < CHUNK_Z_SIZE; z++) {
                // get the block
                Block block = getBlock(x, y, z);

                // // if the block is air skip it
                if (block.getType() == Block::BlockType::AIR) continue;

                // // get the block info
                Block::BlockInfo blockInfo = Block::getBlockInfo(block.getType());

                auto directionHelper = [&](glm::vec3 direction) {
                    //if block is in this chunk and the block is not air draw the face
                    int xtocheck = x + direction.x;
                    int ytocheck = y + direction.y;
                    int ztocheck = z + direction.z;
                    
                    if (xtocheck >= 0 && xtocheck < CHUNK_X_SIZE && ytocheck >= 0 && ytocheck < CHUNK_Y_SIZE && ztocheck >= 0 && ztocheck < CHUNK_Z_SIZE) {
                        Block blockToCheck = getBlock(xtocheck, ytocheck, ztocheck);
                        if (blockToCheck.getType() == Block::BlockType::AIR) {
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        // If block is lower or higher than the world assume it is visible
                        if (ytocheck < 0 || ytocheck >= CHUNK_Y_SIZE) {
                            return true;
                        }

                        // Get the chunk offset
                        int chunkx = xoffset + direction.x;
                        int chunkz = zoffset + direction.z;

                        // Get the world dimensions in chunks
                        auto worldSize = world.getWorldSize();

                        // If the desired chunk is outside the world assume it is visible
                        if (chunkx < 0 || chunkx >= worldSize.first || chunkz < 0 || chunkz >= worldSize.second) {
                            return true;
                        }

                        // Get the block x and z position in the chunk
                        int blockX = Mod(xtocheck, CHUNK_X_SIZE);
                        int blockZ = Mod(ztocheck, CHUNK_Z_SIZE);

                        // Get the chunk
                        Block block = world.getChunk(chunkx, chunkz).getBlock(blockX, ytocheck, blockZ);

                        // If the block is air assume it is visible
                        if (block.getType() == Block::BlockType::AIR) {
                            return true;
                        } else {
                            return false;
                        }
                    }
                };

                // Vertex position offsets
                int xblockoffset = xoffset * CHUNK_X_SIZE;
                int zblockoffset = zoffset * CHUNK_Z_SIZE;

                // Get the texture coordinates for the block
                auto textureCoords = blockInfo.textureCoords;

                // For block generation
                auto arrayHelper = [&](std::vector<GLfloat> f_verticies, std::vector<GLfloat> f_uvs, std::vector<GLuint> f_indicies) {
                    verticies.insert(verticies.end(), f_verticies.begin(), f_verticies.end());
                    indicies.insert(indicies.end(), f_indicies.begin(), f_indicies.end());
                    uvs.insert(uvs.end(), f_uvs.begin(), f_uvs.end());
                    verticies_c += 4;
                };

                // y+ face
                if (directionHelper(glm::vec3(0, 1, 0))) {
                    GLfloat uvx = textureCoords["y+"].first;
                    GLfloat uvy = textureCoords["y+"].second;

                    arrayHelper({
                        x +     xblockoffset, y, z +     zblockoffset,
                        x + 1 + xblockoffset, y, z +     zblockoffset,
                        x + 1 + xblockoffset, y, z + 1 + zblockoffset,
                        x +     xblockoffset, y, z + 1 + zblockoffset
                    },
                    {
                        uvx, uvy,
                        uvx + 1, uvy,
                        uvx + 1, uvy + 1,
                        uvx, uvy + 1
                    },
                    {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    });
                }
                if (directionHelper(glm::vec3(0, -1, 0))) {
                    GLfloat uvx = textureCoords["y-"].first;
                    GLfloat uvy = textureCoords["y-"].second;

                    arrayHelper({
                        x + 1 + xblockoffset, y - 1, z +     zblockoffset,
                        x +     xblockoffset, y - 1, z +     zblockoffset,
                        x +     xblockoffset, y - 1, z + 1 + zblockoffset,
                        x + 1 + xblockoffset, y - 1, z + 1 + zblockoffset,
                    },
                    {
                        uvx, uvy,
                        uvx + 1, uvy,
                        uvx + 1, uvy + 1,
                        uvx, uvy + 1
                    },
                    {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    });
                }
                if (directionHelper(glm::vec3(1, 0, 0))) {
                    GLfloat uvx = textureCoords["x+"].first;
                    GLfloat uvy = textureCoords["x+"].second;

                    arrayHelper({
                        x + 1 + xblockoffset, y - 1, z + 1 + zblockoffset,
                        x + 1 + xblockoffset, y,     z + 1 + zblockoffset,
                        x + 1 + xblockoffset, y,     z +     zblockoffset,
                        x + 1 + xblockoffset, y - 1, z +     zblockoffset,
                    },
                    {
                        uvx + 1, uvy + 1,
                        uvx + 1, uvy,
                        uvx, uvy,
                        uvx, uvy + 1,
                    },
                    {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    });
                }
                if (directionHelper(glm::vec3(-1, 0, 0))) {
                    GLfloat uvx = textureCoords["x-"].first;
                    GLfloat uvy = textureCoords["x-"].second;

                    arrayHelper({
                        x + xblockoffset, y - 1, z +     zblockoffset,
                        x + xblockoffset, y,     z +     zblockoffset,
                        x + xblockoffset, y,     z + 1 + zblockoffset,                        
                        x + xblockoffset, y - 1, z + 1 + zblockoffset,
                    },
                    {
                        uvx + 1, uvy + 1,
                        uvx + 1, uvy,
                        uvx, uvy,
                        uvx, uvy + 1,
                    },
                    {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    });
                }
                if (directionHelper(glm::vec3(0, 0, 1))) {
                    GLfloat uvx = textureCoords["z+"].first;
                    GLfloat uvy = textureCoords["z+"].second;

                    arrayHelper({
                        x +     xblockoffset, y,      z + 1 + zblockoffset,
                        x + 1 + xblockoffset, y,      z + 1 + zblockoffset,
                        x + 1 + xblockoffset, y - 1,  z + 1 + zblockoffset,
                        x +     xblockoffset, y - 1 , z + 1 + zblockoffset
                    },
                    {
                        uvx, uvy,
                        uvx + 1, uvy,
                        uvx + 1, uvy + 1,
                        uvx, uvy + 1,
                    },
                    {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    });
                }

                if (directionHelper(glm::vec3(0, 0, -1))) {
                    GLfloat uvx = textureCoords["z-"].first;
                    GLfloat uvy = textureCoords["z-"].second;

                    arrayHelper({
                        
                        x + 1 + xblockoffset, y - 1,  z +     zblockoffset,
                        x + 1 + xblockoffset, y,      z +     zblockoffset,
                        x +     xblockoffset, y,      z +     zblockoffset,
                        x +     xblockoffset, y - 1 , z +     zblockoffset,
                    },
                    {
                        uvx, uvy + 1,
                        uvx, uvy,
                        uvx + 1, uvy,
                        uvx + 1, uvy + 1,
                    },
                    {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    });
                }
            }
        }
    }
}

// Chunk destructor
Chunk::~Chunk() {

}

std::pair<int, int> Chunk::getPosition() const {
    return std::pair(xoffset, zoffset);
}