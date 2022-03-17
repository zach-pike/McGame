#pragma once

#include "world/world.hpp"
#include <glm/glm.hpp>

// CPP moment
struct GLFWwindow;

class Game {
    private:
        int windowWidth;
        int windowHeight;

        const float near = 0.1f;
        const float far = 1000.0f;

        float fov = 70.0f;

        GLFWwindow* window;
        // World world;

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 modelMatrix;

        glm::mat4 mvpMatrix;

        void setProjectionMatrix(glm::mat4 projectionMatrix);
        void setViewMatrix(glm::mat4 viewMatrix);
        void setModelMatrix(glm::mat4 modelMatrix);

        void recalculateMvpMatrix();

        GLuint programID;

    public:
        Game();
        ~Game();

        void setFov(float fov);
        float getFov();

        void setWindowSize(int width, int height);
        int getWindowWidth();
        int getWindowHeight();

        GLFWwindow* getWindow();

        void run();
};