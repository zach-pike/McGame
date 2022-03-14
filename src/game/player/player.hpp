#pragma once

#include "game/camera/camera.hpp"
#include <glm/glm.hpp>

// CPP moment
class Game;
struct GLFWwindow;

class Player {
    private:
        Camera camera;
        glm::vec3 position;
        glm::vec2 oldMousePos;

    public:
        Camera& getCamera();
        glm::vec3 getPosition();

        void setPosition(glm::vec3 position);

        void update(GLFWwindow* window);

        Player(Game& game);
        ~Player();
};