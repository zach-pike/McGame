#include "chunk.hpp"

#include <string>
#include <unordered_map>

// Chunk constructor
Chunk::Chunk() {
    blocks.fill(Block(Block::BlockType::AIR));

    for (int x = 0; x < CHUNK_X_SIZE; x++) {
        for (int z = 0; z < CHUNK_Z_SIZE; z ++) {
            for (int y = 0; y < CHUNK_Y_SIZE; y++) {
                setBlock(x, y, z, Block(Block::BlockType::GRASS));
            }
        }
    }
}

// setBlock function
void Chunk::setBlock(int x, int y, int z, Block block) {
    blocks[x + y * CHUNK_X_SIZE + z * CHUNK_X_SIZE * CHUNK_Y_SIZE] = block;
}

// getBlock function
Block Chunk::getBlock(int x, int y, int z) {
    return blocks[x + y * CHUNK_X_SIZE + z * CHUNK_X_SIZE * CHUNK_Y_SIZE];
}

// generateMesh function
void Chunk::generateMesh(std::vector<GLfloat>& verticies, std::vector<GLuint>& indicies, std::vector<GLfloat>& uvs) {
    int n_of_verticies = 0;
    //Loop through all the blocks in the chunk
    for (int y = 0; y < CHUNK_Y_SIZE; y++) {
         for (int x = 0; x < CHUNK_X_SIZE; x++) {
            for (int z = 0; z < CHUNK_Z_SIZE; z++) {

                // get the block
                Block block = getBlock(x, y, z);

                // // if the block is air skip it
                if (block.getType() == Block::BlockType::AIR) continue;

                // // get the block info
                Block::BlockInfo blockInfo = Block::getBlockInfo(block.getType());

                // // get the block's uv coordinates
                int uvx = blockInfo.uvx;
                int uvy = blockInfo.uvy;

                std::unordered_map<std::string, bool> faceVisible;

                // check in every direction if the block is visible if the incremented value is less than 0 or greater than CHUNK_X_SIZE or CHUNK_Y_SIZE or CHUNK_Z_SIZE assume it is visible

                // check the x+ direction
                if (x + 1 < CHUNK_X_SIZE) {
                    Block block = getBlock(x + 1, y, z);
                    if (block.getType() == Block::BlockType::AIR) {
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

                // y+ face
                if (faceVisible["y+"]) {
                    GLfloat f_verticies[12] = {
                        x, y, z,
                        x + 1, y, z,
                        x + 1, y, z + 1,
                        x, y, z + 1
                    };

                    GLuint f_indicies[] = {
                        n_of_verticies + 0, n_of_verticies + 1, n_of_verticies + 2,
                        n_of_verticies + 2, n_of_verticies + 3, n_of_verticies + 0
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

                    n_of_verticies += 4;
                }

                if (faceVisible["y-"]) {
                    GLfloat f_verticies[12] = {
                        x, y - 1, z,
                        x + 1, y - 1, z,
                        x + 1, y - 1, z + 1,
                        x, y - 1, z + 1
                    };

                    GLuint f_indicies[] = {
                        n_of_verticies + 0, n_of_verticies + 1, n_of_verticies + 2,
                        n_of_verticies + 2, n_of_verticies + 3, n_of_verticies + 0
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

                    n_of_verticies += 4;
                }

                // x+ face
                if (faceVisible["x+"]) {
                    GLfloat f_verticies[12] = {
                        1 + x, y, z,
                        1 + x, y, z + 1,
                        1 + x, y - 1, z + 1,
                        1 + x, y - 1, z
                    };

                    GLuint f_indicies[] = {
                        n_of_verticies + 0, n_of_verticies + 1, n_of_verticies + 2,
                        n_of_verticies + 2, n_of_verticies + 3, n_of_verticies + 0
                    };

                    GLfloat f_uvs[] = {
                        uvx, uvy,
                        uvx, uvy + 1,
                        uvx + 1, uvy + 1,
                        uvx + 1, uvy
                    };

                    verticies.insert(verticies.end(), f_verticies, f_verticies + 12);
                    indicies.insert(indicies.end(), f_indicies, f_indicies + 6);
                    uvs.insert(uvs.end(), f_uvs, f_uvs + 8);

                    n_of_verticies += 4;
                }

                if (faceVisible["x-"]) {
                    GLfloat f_verticies[12] = {
                        x , y, z,
                        x, y, z + 1,
                        x, y - 1, z + 1,
                        x, y - 1, z
                    };

                    GLuint f_indicies[] = {
                        n_of_verticies + 0, n_of_verticies + 1, n_of_verticies + 2,
                        n_of_verticies + 2, n_of_verticies + 3, n_of_verticies + 0
                    };

                    GLfloat f_uvs[] = {
                        uvx, uvy,
                        uvx, uvy + 1,
                        uvx + 1, uvy + 1,
                        uvx + 1, uvy
                    };

                    verticies.insert(verticies.end(), f_verticies, f_verticies + 12);
                    indicies.insert(indicies.end(), f_indicies, f_indicies + 6);
                    uvs.insert(uvs.end(), f_uvs, f_uvs + 8);

                    n_of_verticies += 4;
                }

                // z+ face
                if (faceVisible["z+"]) {
                    GLfloat f_verticies[12] = {
                        x, y, z + 1,
                        x + 1, y, z + 1,
                        x + 1, y - 1, z + 1,
                        x, y - 1, z + 1
                    };

                    GLuint f_indicies[] = {
                        n_of_verticies + 0, n_of_verticies + 1, n_of_verticies + 2,
                        n_of_verticies + 2, n_of_verticies + 3, n_of_verticies + 0
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

                    n_of_verticies += 4;
                }

                if (faceVisible["z-"]) {
                    GLfloat f_verticies[12] = {
                        x, y, z,
                        x + 1, y, z,
                        x + 1, y - 1, z,
                        x, y - 1, z
                    };

                    GLuint f_indicies[] = {
                        n_of_verticies + 0, n_of_verticies + 1, n_of_verticies + 2,
                        n_of_verticies + 2, n_of_verticies + 3, n_of_verticies + 0
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

                    n_of_verticies += 4;
                }
            }
        }
    }
    
    // int n_of_verticies = 0;

    // auto add_face = [&](glm::vec3 pos) {
    //     int x = pos.x;
    //     int y = pos.y;
    //     int z = pos.z;

    //     GLfloat f_verticies[] = {
    //         x, y, z,
    //         x + 1, y, z,
    //         x + 1, y, z + 1,
    //         x, y, z + 1
    //     };

    //     GLuint f_indicies[] = {
    //         n_of_verticies + 0, n_of_verticies +  1, n_of_verticies +  2,
    //         n_of_verticies + 2, n_of_verticies + 3, n_of_verticies + 0
    //     };

    //     GLfloat uv[] = {
    //         0, 0,
    //         1, 0,
    //         1, 1,
    //         0, 1
    //     };

    //     verticies.insert(verticies.end(), f_verticies, f_verticies + 12);
    //     indicies.insert(indicies.end(), f_indicies, f_indicies + 6);
    //     uvs.insert(uvs.end(), uv, uv + 8);

    //     n_of_verticies += 4;
    // };

    // add_face(glm::vec3(0, 0, 0));
    // add_face(glm::vec3(1, 0, 0));
    // add_face(glm::vec3(2, 0, 0));
    // add_face(glm::vec3(3, 0, 0));
}

// Chunk destructor
Chunk::~Chunk() {
}