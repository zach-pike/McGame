#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class World {
    public:
        World();
        ~World();
        int generateMesh(std::vector<GLfloat>& verticies, std::vector<GLfloat>& uvs);
};