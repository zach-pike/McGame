#include "chunk.hpp"

#include <string>
#include <unordered_map>
#include <iostream>

#include "game/world/world.hpp"

// Chunk constructor
Chunk::Chunk(World& world, int xoffset, int zoffset): 
    world(world),
    xoffset(xoffset * CHUNK_X_SIZE),
    zoffset(zoffset * CHUNK_Z_SIZE)
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
                    // if the value is within the chunk then check if the block is air
                    if (x + direction.x >= 0 && x + direction.x < CHUNK_X_SIZE && y + direction.y >= 0 && y + direction.y < CHUNK_Y_SIZE && z + direction.z >= 0 && z + direction.z < CHUNK_Z_SIZE) {
                        Block block = getBlock(x + direction.x, y + direction.y, z + direction.z);
                        if (block.getType() == Block::BlockType::AIR) {
                            faceVisible[name] = true;
                        } else {
                            faceVisible[name] = false;
                        }
                    } 
                    // if the value is outside the chunk then check if the coordinates are in the world if they are not the 
                    // face is visible
                    else if (!world.isCoordInWorld(x + direction.x + xoffset, y + direction.y, z + direction.z + zoffset)) {
                        faceVisible[name] = true;
                    }

                    // if the block coordinates are valid then check the correct neighbouring chunk
                    else {
                        Chunk chunk = world.getChunk((xoffset / CHUNK_X_SIZE) + direction.x, (zoffset / CHUNK_Z_SIZE) + direction.z);
                        Block block = chunk.getBlock(x + direction.x, y + direction.y, z + direction.z);
                        if (block.getType() == Block::BlockType::AIR) {
                            faceVisible[name] = true;
                        } else {
                            faceVisible[name] = false;
                        }
                    }
                };


                // check the x+ direction
                if (x + 1 < CHUNK_X_SIZE) {
                    Block block_x_plus = getBlock(x + 1, y, z);
                    if (block_x_plus.getType() == Block::BlockType::AIR) {
                        faceVisible["x+"] = true;
                    }
                } else {
                    faceVisible["x+"] = true;
                }
                

                // check the x- direction
                if (x - 1 >= 0) {
                    Block block = getBlock(x - 1, y, z);
                    if (block.getType() == Block::BlockType::AIR) {
                        faceVisible["x-"] = true;
                    }
                } else {
                    faceVisible["x-"] = true;
                }

                // check the y+ direction
                if (y + 1 < CHUNK_Y_SIZE) {
                    Block block = getBlock(x, y + 1, z);
                    if (block.getType() == Block::BlockType::AIR) {
                        faceVisible["y+"] = true;
                    }
                } else {
                    faceVisible["y+"] = true;
                }

                // check the y- direction
                if (y - 1 >= 0) {
                    Block block = getBlock(x, y - 1, z);
                    if (block.getType() == Block::BlockType::AIR) {
                        faceVisible["y-"] = true;
                    }
                } else {
                    faceVisible["y-"] = true;
                }

                // check the z+ direction
                if (z + 1 < CHUNK_Z_SIZE) {
                    Block block = getBlock(x, y, z + 1);
                    if (block.getType() == Block::BlockType::AIR) {
                        faceVisible["z+"] = true;
                    }
                } else {
                    faceVisible["z+"] = true;
                }

                // check the z- direction
                if (z - 1 >= 0) {
                    Block block = getBlock(x, y, z - 1);
                    if (block.getType() == Block::BlockType::AIR) {
                        faceVisible["z-"] = true;
                    }
                } else {
                    faceVisible["z-"] = true;
                }

                // lets add the verticies and indicies for each face (remember that we must add the verticies in a clockwise order)

                // Get the texture coordinates for the block
                auto textureCoords = blockInfo.textureCoords;

                // y+ face
                if (faceVisible["y+"]) {
                    GLfloat uvx = textureCoords["y+"].first;
                    GLfloat uvy = textureCoords["y+"].second;

                    GLfloat f_verticies[12] = {
                        x +     xoffset, y, z +     zoffset,
                        x + 1 + xoffset, y, z +     zoffset,
                        x + 1 + xoffset, y, z + 1 + zoffset,
                        x +     xoffset, y, z + 1 + zoffset
                    };

                    GLuint f_indicies[] = {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    };

                    GLfloat f_uvs[] = {
                        uvx, uvy,
                        uvx + 1, uvy,
                        uvx + 1, uvy + 1,
                        uvx, uvy + 1
                    };

                    verticies.insert(verticies.end(), f_verticies, f_verticies + 12);
                    indicies.insert(indicies.end(), f_indicies, f_indicies + 6);
                    uvs.insert(uvs.end(), f_uvs, f_uvs + 8);

                    verticies_c += 4;
                }

                if (faceVisible["y-"]) {
                    GLfloat uvx = textureCoords["y-"].first;
                    GLfloat uvy = textureCoords["y-"].second;

                    GLfloat f_verticies[12] = {
                        x +     xoffset, y - 1, z +     zoffset,
                        x + 1 + xoffset, y - 1, z +     zoffset,
                        x + 1 + xoffset, y - 1, z + 1 + zoffset,
                        x +     xoffset, y - 1, z + 1 + zoffset
                    };

                    GLuint f_indicies[] = {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    };

                    GLfloat f_uvs[] = {
                        uvx, uvy,
                        uvx + 1, uvy,
                        uvx + 1, uvy + 1,
                        uvx, uvy + 1
                    };

                    verticies.insert(verticies.end(), f_verticies, f_verticies + 12);
                    indicies.insert(indicies.end(), f_indicies, f_indicies + 6);
                    uvs.insert(uvs.end(), f_uvs, f_uvs + 8);

                    verticies_c += 4;
                }

                // x+ face
                if (faceVisible["x+"]) {
                    GLfloat uvx = textureCoords["x+"].first;
                    GLfloat uvy = textureCoords["x+"].second;

                    GLfloat f_verticies[12] = {
                        x + 1 + xoffset, y,     z +     zoffset,
                        x + 1 + xoffset, y,     z + 1 + zoffset,
                        x + 1 + xoffset, y - 1, z + 1 + zoffset,
                        x + 1 + xoffset, y - 1, z +     zoffset
                    };

                    GLuint f_indicies[] = {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    };

                    GLfloat f_uvs[] = {
                        uvx + 1, uvy,
                        uvx, uvy,
                        uvx, uvy + 1,
                        uvx + 1, uvy + 1,
                    };

                    verticies.insert(verticies.end(), f_verticies, f_verticies + 12);
                    indicies.insert(indicies.end(), f_indicies, f_indicies + 6);
                    uvs.insert(uvs.end(), f_uvs, f_uvs + 8);

                    verticies_c += 4;
                }

                if (faceVisible["x-"]) {
                    GLfloat uvx = textureCoords["x-"].first;
                    GLfloat uvy = textureCoords["x-"].second;

                    GLfloat f_verticies[12] = {
                        x + xoffset, y,     z +     zoffset,
                        x + xoffset, y,     z + 1 + zoffset,
                        x + xoffset, y - 1, z + 1 + zoffset,
                        x + xoffset, y - 1, z +     zoffset
                    };

                    GLuint f_indicies[] = {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    };

                    GLfloat f_uvs[] = {
                        uvx + 1, uvy,
                        uvx, uvy,
                        uvx, uvy + 1,
                        uvx + 1, uvy + 1,
                    };

                    verticies.insert(verticies.end(), f_verticies, f_verticies + 12);
                    indicies.insert(indicies.end(), f_indicies, f_indicies + 6);
                    uvs.insert(uvs.end(), f_uvs, f_uvs + 8);

                    verticies_c += 4;
                }

                // z+ face
                if (faceVisible["z+"]) {
                    GLfloat uvx = textureCoords["z+"].first;
                    GLfloat uvy = textureCoords["z+"].second;

                    GLfloat f_verticies[12] = {
                        x +     xoffset, y,      z + 1 + zoffset,
                        x + 1 + xoffset, y,      z + 1 + zoffset,
                        x + 1 + xoffset, y - 1,  z + 1 + zoffset,
                        x +     xoffset, y - 1 , z + 1 + zoffset
                    };

                    GLuint f_indicies[] = {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    };

                    GLfloat f_uvs[] = {
                        uvx, uvy,
                        uvx + 1, uvy,
                        uvx + 1, uvy + 1,
                        uvx, uvy + 1,
                    };

                    verticies.insert(verticies.end(), f_verticies, f_verticies + 12);
                    indicies.insert(indicies.end(), f_indicies, f_indicies + 6);
                    uvs.insert(uvs.end(), f_uvs, f_uvs + 8);

                    verticies_c += 4;
                }

                if (faceVisible["z-"]) {
                    GLfloat uvx = textureCoords["z-"].first;
                    GLfloat uvy = textureCoords["z-"].second;

                    GLfloat f_verticies[12] = {
                        x +     xoffset, y,     z + zoffset,
                        x + 1 + xoffset, y,     z + zoffset,
                        x + 1 + xoffset, y - 1, z + zoffset,
                        x +     xoffset, y - 1, z + zoffset
                    };

                    GLuint f_indicies[] = {
                        verticies_c + 0, verticies_c + 1, verticies_c + 2,
                        verticies_c + 2, verticies_c + 3, verticies_c + 0
                    };

                    GLfloat f_uvs[] = {
                        uvx, uvy,
                        uvx + 1, uvy,
                        uvx + 1, uvy + 1,
                        uvx, uvy + 1,
                    };

                    verticies.insert(verticies.end(), f_verticies, f_verticies + 12);
                    indicies.insert(indicies.end(), f_indicies, f_indicies + 6);
                    uvs.insert(uvs.end(), f_uvs, f_uvs + 8);

                    verticies_c += 4;
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