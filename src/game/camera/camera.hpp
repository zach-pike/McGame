#pragma once

#include <glm/glm.hpp>

class Camera {
    private:
        glm::vec3 lookvector;
        double pitch = 0;
        double yaw = 0;

    public:
        Camera();
        ~Camera();

        glm::vec3 getLookVector();

        glm::vec3 getRightVector();

        glm::vec2 getPitchYaw();
        void setPitchYaw(glm::vec2 rotation);
};
