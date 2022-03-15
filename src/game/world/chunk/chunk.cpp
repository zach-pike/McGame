#include "chunk.hpp"

#include <string>
#include <unordered_map>
#include <iostream>

#include "game/world/world.hpp"

// Chunk constructor
Chunk::Chunk(World& world, int xoffset, int zoffset): 
    world(world),
    xoffset(xoffset),
    zoffset(zoffset)
{

    for (int x = 0; x < CHUNK_X_SIZE; x++) {
        for (int z = 0; z < CHUNK_Z_SIZE; z ++) {
            for (int y = 0; y < 1; y++) {
                setBlock(x, y, z, Block(Block::BlockType::DIRT));
            }
        }
    }
}

// setBlock function
void Chunk::setBlock(int x, int y, int z, Block block) {
    blocks[x + y * CHUNK_X_SIZE + z * CHUNK_X_SIZE * CHUNK_Y_SIZE] = block;
}

// getBlock function
Block Chunk::getBlock(int x, int y, int z) const {
    return blocks[x + y * CHUNK_X_SIZE + z * CHUNK_X_SIZE * CHUNK_Y_SIZE];
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

                std::unordered_map<std::string, bool> faceVisible;


                // check in every direction if the block is visible if the incremented value is less than 0 or greater than CHUNK_X_SIZE or CHUNK_Y_SIZE or CHUNK_Z_SIZE assume it is visible

                auto directionHelper = [&](glm::vec3 direction, std::string name) {
                    // To be fixed in next commit  
                };


                directionHelper(glm::vec3(1, 0, 0), "x+");
                directionHelper(glm::vec3(-1, 0, 0), "x-");
                directionHelper(glm::vec3(0, 1, 0), "y+");
                directionHelper(glm::vec3(0, -1, 0), "y-");
                directionHelper(glm::vec3(0, 0, 1), "z+");
                directionHelper(glm::vec3(0, 0, -1), "z-");

                int xblockoffset = xoffset * CHUNK_X_SIZE;
                int zblockoffset = zoffset * CHUNK_Z_SIZE;

                // Get the texture coordinates for the block
                auto textureCoords = blockInfo.textureCoords;

                auto arrayHelper = [&](std::vector<GLfloat> f_verticies, std::vector<GLfloat> f_uvs, std::vector<GLuint> f_indicies) {
                    verticies.insert(verticies.end(), f_verticies.begin(), f_verticies.end());
                    indicies.insert(indicies.end(), f_indicies.begin(), f_indicies.end());
                    uvs.insert(uvs.end(), f_uvs.begin(), f_uvs.end());

                    verticies_c += 4;
                };

                // y+ face
                if (faceVisible["y+"]) {
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
                if (faceVisible["y-"]) {
                    GLfloat uvx = textureCoords["y-"].first;
                    GLfloat uvy = textureCoords["y-"].second;

                    arrayHelper({
                        x +     xblockoffset, y - 1, z +     zblockoffset,
                        x + 1 + xblockoffset, y - 1, z +     zblockoffset,
                        x + 1 + xblockoffset, y - 1, z + 1 + zblockoffset,
                        x +     xblockoffset, y - 1, z + 1 + zblockoffset
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
                if (faceVisible["x+"]) {
                    GLfloat uvx = textureCoords["x+"].first;
                    GLfloat uvy = textureCoords["x+"].second;

                    arrayHelper({
                        x + 1 + xblockoffset, y,     z +     zblockoffset,
                        x + 1 + xblockoffset, y,     z + 1 + zblockoffset,
                        x + 1 + xblockoffset, y - 1, z + 1 + zblockoffset,
                        x + 1 + xblockoffset, y - 1, z +     zblockoffset
                    },
                    {
                        uvx + 1, uvy,
                        uvx, uvy,
                        uvx, uvy + 1,
                        uvx + 1, uvy + 1,
                    },
                    {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    });
                }
                if (faceVisible["x-"]) {
                    GLfloat uvx = textureCoords["x-"].first;
                    GLfloat uvy = textureCoords["x-"].second;

                    arrayHelper({
                        x + xblockoffset, y,     z +     zblockoffset,
                        x + xblockoffset, y,     z + 1 + zblockoffset,
                        x + xblockoffset, y - 1, z + 1 + zblockoffset,
                        x + xblockoffset, y - 1, z +     zblockoffset
                    },
                    {
                        uvx + 1, uvy,
                        uvx, uvy,
                        uvx, uvy + 1,
                        uvx + 1, uvy + 1,
                    },
                    {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    });
                }
                if (faceVisible["z+"]) {
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

                if (faceVisible["z-"]) {
                    GLfloat uvx = textureCoords["z-"].first;
                    GLfloat uvy = textureCoords["z-"].second;

                    arrayHelper({
                        x +     xblockoffset, y,     z + zblockoffset,
                        x + 1 + xblockoffset, y,     z + zblockoffset,
                        x + 1 + xblockoffset, y - 1, z + zblockoffset,
                        x +     xblockoffset, y - 1, z + zblockoffset
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
            }
        }
    }
}

// Chunk destructor
Chunk::~Chunk() {

}

std::pair<int, int> Chunk::getPosition() const {
    return std::pair(xoffset * CHUNK_X_SIZE, zoffset * CHUNK_Z_SIZE);
}