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
        Game& gameobj;
        float speed = 0.5f;
        bool mouse_locked = true;
        bool mouse_locked_last = false;

    public:
        Camera& getCamera();
        glm::vec3 getPosition();

        void setPosition(glm::vec3 position);

        void update();

        Player(Game& game);
        ~Player();
};